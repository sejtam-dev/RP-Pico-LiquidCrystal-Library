#include "liquid_crystal.h"

LiquidCrystal::LiquidCrystal(const LiquidCrystalPins pins, const LiquidCrystalSize size) : m_pins(pins), m_size(size)
{
	// Check 8 or 4-bit data transfering
	if (pins.data0 == 0 && pins.data1 == 0 && pins.data2 == 0 && pins.data3 == 0)
		m_fourBitMode = true;
}

void LiquidCrystal::init(uint8_t charSize)
{
	// Startup functions
	m_displayFunction = LCD_4BITMODE | LCD_1LINE | charSize;

	// If 8-bit mode, set function to 8-bit
	if (!m_fourBitMode)
		m_displayFunction |= LCD_8BITMODE;

	// 1 or 2 line Display (4 line display is 2 line)
	if (getHeight() > 1)
		m_displayFunction |= LCD_2LINE;

	// Display offsets
	//
	// Row 1 => 0x00, 0x01, 0x02...
	// Row 2 => 0x40, 0x41, 0x42...
	// Row 3 => 0x20, 0x21, 0x22...
	// Row 4 => 0x60, 0x61, 0x62...
	const uint8_t width = getWidth();
	m_rowOffset[0] = 0x00;
	m_rowOffset[1] = 0x40;
	m_rowOffset[2] = 0x00 + width;
	m_rowOffset[3] = 0x40 + width;

	// Define pins
	// RS Pin
	gpio_init(m_pins.rsPin);
	gpio_set_dir(m_pins.rsPin, GPIO_OUT);

	// Enable Pin
	gpio_init(m_pins.enabledPin);
	gpio_set_dir(m_pins.enabledPin, GPIO_OUT);

	// Data pins
	const uint8_t* ppins = pinsPointer();
	for (int pin = (m_fourBitMode ? 4 : 0); pin < 8; ++pin)
	{
		gpio_init(ppins[pin]);
		gpio_set_dir(ppins[pin], GPIO_OUT);
	}

	sleep_us(50000);

	gpio_put(m_pins.rsPin, PIN_OFF);
	gpio_put(m_pins.enabledPin, PIN_OFF);

	// First bits
	if (m_fourBitMode)
	{
		write4bits(0x03);
		sleep_us(4500);

		write4bits(0x03);
		sleep_us(4500);

		write4bits(0x03);
		sleep_us(150);

		write4bits(0x02);
	}
	else
	{
		command(LCD_FUNCTIONSET | m_displayFunction);
		sleep_us(4500);

		command(LCD_FUNCTIONSET | m_displayFunction);
		sleep_us(150);

		command(LCD_FUNCTIONSET | m_displayFunction);
	}

	// Startup settings
	command(LCD_FUNCTIONSET | m_displayFunction);

	m_displayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	display();
	clear();

	m_displayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	command(LCD_ENTRYMODESET | m_displayMode);
}

void LiquidCrystal::clear()
{
	command(LCD_CLEARDISPLAY);
	sleep_us(2000);
}
void LiquidCrystal::home()
{
	command(LCD_RETURNHOME);
	sleep_us(2000);
}

void LiquidCrystal::setCursor(uint8_t column, uint8_t row)
{
	const uint8_t width = getWidth();
	if (column >= width)
		column = width;

	const uint8_t height = getHeight();
	if (row >= height)
		row = height;

	m_cursorX = column;
	m_cursorY = row;

	command(LCD_SETDDRAMADDR | (column + m_rowOffset[row]));
}
void LiquidCrystal::move(uint8_t x, uint8_t y, bool sameLine)
{
	m_cursorX++;
	if (m_cursorX >= getWidth())
	{
		m_cursorX = 0;

		if (sameLine == false)
		{
			m_cursorY++;
			if (m_cursorY >= getHeight())
			{
				m_cursorY = 0;
			}
		}

		setCursor(m_cursorX, m_cursorY);
	}
}

void LiquidCrystal::print(const char* text)
{
	setCursor(m_cursorX, m_cursorY);

	while (*text)
	{
		write((uint8_t) *text++);

		move(1, 0, false);
	}

	setCursor(m_cursorX, m_cursorY);
}
void LiquidCrystal::printInLine(const char* text)
{
	setCursor(m_cursorX, m_cursorY);

	while (*text)
	{
		write((uint8_t) *text++);

		move(1, 0, true);
	}

	setCursor(m_cursorX, m_cursorY);
}

void LiquidCrystal::print(const uint8_t byte)
{
	setCursor(m_cursorX, m_cursorY);

	write(byte);

	m_cursorX++;
	setCursor(m_cursorX, m_cursorY);
}
void LiquidCrystal::print(const uint8_t* bytes, bool deleteArray)
{
	while(*bytes) {
		print(*bytes++);
	}

	if(deleteArray)
		delete[] bytes;
}

void LiquidCrystal::noDisplay()
{
	m_displayControl &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | m_displayControl);
}
void LiquidCrystal::display()
{
	m_displayControl |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | m_displayControl);
}

void LiquidCrystal::noCursor()
{
	m_displayControl &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | m_displayControl);
}
void LiquidCrystal::cursor()
{
	m_displayControl |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | m_displayControl);
}

void LiquidCrystal::noBlink()
{
	m_displayControl &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | m_displayControl);
}
void LiquidCrystal::blink()
{
	m_displayControl |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | m_displayControl);
}

void LiquidCrystal::scrollDisplayLeft()
{
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystal::scrollDisplayRight()
{
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void LiquidCrystal::leftToRight()
{
	m_displayMode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | m_displayMode);
}
void LiquidCrystal::rightToLeft()
{
	m_displayMode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | m_displayMode);
}

void LiquidCrystal::autoscroll()
{
	m_displayMode |= LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | m_displayMode);
}
void LiquidCrystal::noAutoscroll()
{
	m_displayMode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | m_displayMode);
}

void LiquidCrystal::createChar(uint8_t location, uint8_t charmap[])
{
	location &= 0x7;
	command(LCD_SETCGRAMADDR | (location << 3));
	for (int i = 0; i < 8; i++) {
		write(charmap[i]);
	}
}

void LiquidCrystal::command(uint8_t value)
{
	send(value, PIN_OFF);
}
void LiquidCrystal::write(uint8_t value)
{
	send(value, PIN_ON);
}
void LiquidCrystal::send(uint8_t value, uint8_t mode)
{
	gpio_put(m_pins.rsPin, mode);

	if (m_fourBitMode)
	{
		write4bits(value >> 4);
		write4bits(value);
	}
	else
	{
		write8bits(value);
	}
}

void LiquidCrystal::pulseEnable()
{
	gpio_put(m_pins.enabledPin, PIN_OFF);
	sleep_us(1);

	gpio_put(m_pins.enabledPin, PIN_ON);
	sleep_us(1);

	gpio_put(m_pins.enabledPin, PIN_OFF);
	sleep_us(100);
}
void LiquidCrystal::write4bits(uint8_t value)
{
	const uint8_t* ppins = pinsPointer();
	for (int pin = 0; pin < 4; ++pin)
	{
		gpio_put(ppins[pin + 4], (value >> pin) & 0x01);
	}

	pulseEnable();
}
void LiquidCrystal::write8bits(uint8_t value)
{
	const uint8_t* ppins = pinsPointer();
	for (int pin = 0; pin < 8; ++pin)
	{
		gpio_put(ppins[pin], (value >> pin) & 0x01);
	}

	pulseEnable();
}