#include "LiquidCrystal.h"
#include <iostream>
#include <chrono>
#include <thread>

	LiquidCrystal lcd; //no need to describe pins. You can if you want but it'll not have any effects

	void delay(int msec);
	int main();
	void setup();
	void loop();

	uint8_t smiley[8] = {
	  0b00000,
	  0b10001,
	  0b00000,
	  0b00000,
	  0b10001,
	  0b01110,
	  0b00000,
	};

	void delay(int msec)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(msec));
	}

	int main()
	{
		setup();
		return (0);
	}

	void setup() 
	{
		lcd.createChar(0, smiley);
		lcd.begin(16, 2);
		loop();
	}

	void loop() 
	{
		while (1)
		{	
			int count = 33;
			char ascii = 0x00 + 33;    //Starting from 34th

			while (count != 235)
			{
				lcd.setCursor(0, 0);
				lcd.write(0xE0);
				lcd.print("DECIMAL = ");
				lcd.print(count);
				lcd.setCursor(0, 1);
				lcd.print("ASCII = ");
				lcd.print(ascii);
				count++;
				ascii++;
				delay(500);
				lcd.clear();
			}
		}
	}