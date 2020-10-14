#include "LiquidCrystal.h"
#include "Hardware.h"

Hardware hard;

LiquidCrystal::LiquidCrystal()
{

}

LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
	uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
	uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
	init(0, rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7);
}

LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t enable,
	uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
	uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
	init(0, rs, 255, enable, d0, d1, d2, d3, d4, d5, d6, d7);
}

LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
	uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
	init(1, rs, rw, enable, d0, d1, d2, d3, 0, 0, 0, 0);
}

LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t enable,
	uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
	init(1, rs, 255, enable, d0, d1, d2, d3, 0, 0, 0, 0);
}

//not very usefull
void LiquidCrystal::init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
	uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
	uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
	_rs_pin = rs;
	_rw_pin = rw;
	_enable_pin = enable;

	_data_pins[0] = d0;
	_data_pins[1] = d1;
	_data_pins[2] = d2;
	_data_pins[3] = d3;
	_data_pins[4] = d4;
	_data_pins[5] = d5;
	_data_pins[6] = d6;
	_data_pins[7] = d7;

	if (fourbitmode)
		_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	else
		_displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;

	begin(16, 1);
}


void LiquidCrystal::begin(uint8_t cols, uint8_t rows, uint8_t charsize)
{
	_numlines = rows;

	hard.setNumberOfCollums(cols);
	hard.setNumberOfRows(rows);
	hard.setText("I LOVE LCDs", 0);
	hard.setText("-> guigur.com <-", 1);
	//delayMicroseconds(50000);

}

void LiquidCrystal::clear()
{
	hard.clear();
	command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
	//delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidCrystal::home()
{

}

void LiquidCrystal::noDisplay()
{

}

void LiquidCrystal::display()
{

}

void LiquidCrystal::noBlink()
{

}

void LiquidCrystal::blink()
{

}

void LiquidCrystal::noCursor()
{

}

void LiquidCrystal::cursor()
{

}

void LiquidCrystal::scrollDisplayLeft()
{

}

void LiquidCrystal::scrollDisplayRight()
{

}

void LiquidCrystal::leftToRight()
{

}

void LiquidCrystal::rightToLeft()
{

}

void LiquidCrystal::autoscroll()
{

}

void LiquidCrystal::noAutoscroll()
{

}

void LiquidCrystal::setRowOffsets(int row1, int row2, int row3, int row4)
{

}

void LiquidCrystal::createChar(uint8_t, uint8_t[])
{

}

void LiquidCrystal::setCursor(uint8_t, uint8_t)
{

}

/*virtual size_t write(uint8_t)
{

}*/

void LiquidCrystal::command(uint8_t)
{

}
/*
template<typename T>
void LiquidCrystal::print(const T& str)
{

}
*/
