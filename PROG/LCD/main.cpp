#include "LiquidCrystal.h"
#include <iostream>
#include <chrono>
#include <thread>

	LiquidCrystal lcd; //no need to describe pins. You can if you want but it'll not have any effects

	void delay(int msec);
	int main();
	void setup();
	void loop();

	void delay(int msec)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(msec));
	}

	int main()
	{
		setup();
	}

	void setup() 
	{
		lcd.begin(16, 2);
		loop();
	}

	void loop() 
	{
		while (1)
		{	
			lcd.setCursor(0, 0);
			lcd.print("I Love LCDs !");
			lcd.setCursor(0, 1);
			lcd.print("@guigur");

			// Turn off the cursor:
			lcd.noCursor();
			delay(500);
			// Turn on the cursor:
			lcd.cursor();
			delay(500);
			lcd.clear();
		}
	}