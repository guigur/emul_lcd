#include "LiquidCrystal.h"
#include <iostream>
//#include <Arduino.h>
#include <Windows.h>
#include <chrono>
#include <thread>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

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
		// set up the LCD's number of columns and rows:
		lcd.begin(16, 2);
		loop();
	}

	void loop() 
	{

		while (1)
		{
			time_t t;

			
			lcd.setCursor(0, 0); // set the cursor to (0,0):
			// print from 0 to 9:
			
			
			for (int thisChar = 0; thisChar < 10; thisChar++) {
				srand((unsigned)time(&t));

			  lcd.print(rand() % 10);
			  delay(500);
				
			}

			lcd.setCursor(0, 1); // set the cursor to (16,1):
			// set the display to automatically scroll:
			//lcd.autoscroll();
			// print from 0 to 9:
			for (int thisChar = 0; thisChar < 10; thisChar++) {
			  lcd.print(thisChar);
			  delay(500);
			}
			// turn off automatic scrolling
			//lcd.noAutoscroll();

			// clear screen for the next loop:
			lcd.clear();
			
		}
	}