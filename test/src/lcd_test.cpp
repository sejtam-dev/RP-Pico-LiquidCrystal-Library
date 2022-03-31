#include "lcd_test.h"

LiquidCrystal lcd({0, 0, 0, 0, 21, 20, 19, 18, 17, 16}, LiquidCrystalSize::W20xH04);

int main()
{
	lcd.init();

	// First Line of LCD
	lcd.print("Hello World!");

	// Second Line of LCD
	lcd.setCursor(4, 1);
	lcd.print("Testing Library!");

	// Third Line of LCD
	lcd.setCursor(18, 2);
	lcd.print("Testing new line");

	sleep_ms(2000);
	lcd.clear();

	// Scrolling
	lcd.setCursor(0, 0);
	lcd.print("Scroll left!!");
	lcd.scrollDisplayLeft();
	sleep_ms(2000);
	lcd.scrollDisplayRight();

	lcd.setCursor(0, 0);
	lcd.clear();
	lcd.print("Scroll Right!!");
	lcd.scrollDisplayRight();
	sleep_ms(2000);
	lcd.scrollDisplayLeft();

	sleep_ms(2000);
	lcd.clear();

	// Blink & Cursor
	lcd.setCursor(0, 0);
	lcd.blink();
	sleep_ms(2000);
	lcd.noBlink();

	sleep_ms(2000);

	lcd.cursor();
	sleep_ms(2000);
	lcd.noCursor();

	sleep_ms(2000);
	lcd.clear();

	lcd.setCursor(0, 0);
	lcd.print("Test finished!");

}