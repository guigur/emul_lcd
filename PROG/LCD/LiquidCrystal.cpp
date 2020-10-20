#include "LiquidCrystal.h"
#include "Hardware.h"
#include <string>
#include <sstream>

Hardware hard;

LiquidCrystal::LiquidCrystal()
{

}

LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
	uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
	uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
	begin(16, 1);
}

LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t enable,
	uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
	uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
	begin(16, 1);
}

LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
	uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
	begin(16, 1);
}

LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t enable,
	uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
	begin(16, 1);
}

void LiquidCrystal::init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
	uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
	uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
	begin(16, 1);
}


void LiquidCrystal::begin(uint8_t cols, uint8_t rows, uint8_t charsize)
{
	_numlines = rows;

	hard.setNumberOfCollums(cols);
	hard.setNumberOfRows(rows);
	std::this_thread::sleep_for(std::chrono::microseconds(50000)); // this command takes a long time!
}

void LiquidCrystal::clear()
{
	hard.clear(); //command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
	std::this_thread::sleep_for(std::chrono::microseconds(2000)); // this command takes a long time!
}

void LiquidCrystal::home()
{
	hard.returnHome(); //command(LCD_RETURNHOME);  // set cursor position to zero
	std::this_thread::sleep_for(std::chrono::microseconds(2000)); // this command takes a long time!
}

void LiquidCrystal::noDisplay()
{
	hard.display(false);
}

void LiquidCrystal::display()
{
	hard.display(true);
}

void LiquidCrystal::noBlink()
{
	hard.blink(false);
}

void LiquidCrystal::blink()
{
	hard.blink(true);
}

void LiquidCrystal::noCursor()
{
	hard.cursor(false);
}

void LiquidCrystal::cursor()
{
	hard.cursor(true);
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

// This will 'right justify' text from the cursor
void LiquidCrystal::autoscroll()
{
	//_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	//command(LCD_ENTRYMODESET | _displaymode);
	//hard.setMode()
}

// This will 'left justify' text from the cursor
void LiquidCrystal::noAutoscroll()
{
	//_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	//command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystal::setRowOffsets(int row1, int row2, int row3, int row4)
{

}

void LiquidCrystal::createChar(uint8_t location, uint8_t charmap[])
{
	if (location > 7 || location < 0) //only location from 0 to 7 are availables
		return;
	hard.createChar(location, charmap);
}

void LiquidCrystal::setCursor(uint8_t col, uint8_t row)
{
	hard.setCursor(col, row);
}

void LiquidCrystal::command(uint8_t)
{
	std::cout << "not implemented yet" << std::endl;
}

template<typename T>
void LiquidCrystal::print(T str)
{
	std::stringstream ss;
	ss << str;
	hard.print(ss.str());
}

template void LiquidCrystal::print<int>(int);
template void LiquidCrystal::print<char>(char);
template void LiquidCrystal::print<char*>(char*);
template void LiquidCrystal::print<const char*>(const char*);
template void LiquidCrystal::print<double>(double);


void LiquidCrystal::write(uint8_t value)
{
	hard.write(value);
}

//////////////////////////////////////////// PRIVATE STUFF ////////////////////////////////////////////
