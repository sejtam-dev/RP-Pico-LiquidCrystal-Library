#pragma once

#include "pico.h"
#include "pico/time.h"
#include "pico/types.h"
#include "hardware/gpio.h"
#include "pico/stdio.h"

// ## COMMANDS ##
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

#ifndef PIN_ON
#define PIN_ON 1
#endif

#ifndef PIN_OFF
#define PIN_OFF 0
#endif

struct LiquidCrystalPins
{
	uint8_t data0, data1, data2, data3, data4, data5, data6, data7, rsPin, enabledPin;
};

enum class LiquidCrystalSize
{
	W16xH02, W20xH04
};

class LiquidCrystal {

private:
	const LiquidCrystalPins m_pins;
	const LiquidCrystalSize m_size;
	uint8_t m_rowOffset[4];

	bool m_fourBitMode = false;
	uint8_t m_displayFunction;
	uint8_t m_displayControl;
	uint8_t m_displayMode;

	uint8_t m_cursorX;
	uint8_t m_cursorY;

	uint8_t* pinsPointer() const
	{
		return (uint8_t*)&m_pins;
	}

	void send(uint8_t value, uint8_t mode);

	void pulseEnable();
	void write4bits(uint8_t value);
	void write8bits(uint8_t value);

public:
	LiquidCrystal(LiquidCrystalPins pins, LiquidCrystalSize size = LiquidCrystalSize::W16xH02);

	uint8_t getWidth() const
	{
		switch (m_size)
		{
		case LiquidCrystalSize::W20xH04:
			return 20;
		case LiquidCrystalSize::W16xH02:
		default:
			return 16;
		}
	}
	uint8_t getHeight() const
	{
		switch (m_size)
		{
		case LiquidCrystalSize::W20xH04:
			return 4;
		case LiquidCrystalSize::W16xH02:
		default:
			return 2;
		}
	}

	void init(uint8_t charSize = LCD_5x8DOTS);

	void command(uint8_t value);
	void write(uint8_t value);

	void clear();
	void home();

	void move(uint8_t x, uint8_t y, bool inLine = false);
	void setCursor(uint8_t column, uint8_t row);

	void print(const char* text);
	void printInLine(const char* text);

	void print(uint8_t byte);
	void print(const uint8_t* bytes, bool deleteArray = false);

	void noDisplay();
	void display();

	void noCursor();
	void cursor();

	void noBlink();
	void blink();

	void scrollDisplayLeft();
	void scrollDisplayRight();

	void leftToRight();
	void rightToLeft();

	void autoscroll();
	void noAutoscroll();

	void createChar(uint8_t location, uint8_t charmap[]);
};