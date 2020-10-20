#ifndef LIQUIDCRYSTAL_H_
#define LIQUIDCRYSTAL_H_

#include <stdint.h> //types

class LiquidCrystal
{
	public:
		LiquidCrystal(); //debug purposes
		LiquidCrystal(uint8_t rs, uint8_t enable,
			uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
			uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
		LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
			uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
			uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
		LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
			uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
		LiquidCrystal(uint8_t rs, uint8_t enable,
			uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);

		void init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
			uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
			uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);

		void begin(uint8_t cols, uint8_t rows, uint8_t charsize = 0);

		void clear();
		void home();

		void noDisplay();
		void display();
		void noBlink();
		void blink();
		void noCursor();
		void cursor();
		void scrollDisplayLeft();
		void scrollDisplayRight();
		void leftToRight();
		void rightToLeft();
		void autoscroll();
		void noAutoscroll();

		void setRowOffsets(int row1, int row2, int row3, int row4);
		void createChar(uint8_t, uint8_t[]);
		void setCursor(uint8_t, uint8_t);
		void write(uint8_t);
		void command(uint8_t);

		template<typename T>
		void print(T str);
private:
	/*void send(uint8_t, uint8_t);
	void write4bits(uint8_t);
	void write8bits(uint8_t);
	void pulseEnable();
	*/
	uint8_t _rs_pin; // LOW: command.  HIGH: character.
	uint8_t _rw_pin; // LOW: write to LCD.  HIGH: read from LCD.
	uint8_t _enable_pin; // activated by a HIGH pulse.
	uint8_t _data_pins[8];

	uint8_t _displayfunction;
	uint8_t _displaycontrol;
	uint8_t _displaymode;

	uint8_t _initialized;

	uint8_t _numlines;
	uint8_t _row_offsets[4];
};

#endif // !LIQUIDCRYSTAL_H_
