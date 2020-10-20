#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "Character.h"

class Hardware : public sf::RectangleShape
{
public:
	Hardware();
	void clear();
	void returnHome();

	void setText(const std::string &str, const unsigned int &line);
	void display(bool state);
	void blink(bool state);
	void cursor(bool state);
	void setCursor(const unsigned int &col, const unsigned int &row);
	void createChar(const uint8_t &pos, uint8_t *character);
	void write(const uint8_t &character);
	void print(const std::string &str);
	void setNumberOfCollums(const unsigned int &noc);
	unsigned int getNumberOfCollums();
	void setNumberOfRows(const unsigned int &nor);
	unsigned int getNumberOfRows();
	void debugChar(const uint8_t c);

private:
	void drawLine(sf::RenderTarget &target, int line) const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;
	void gui_thread();
	void fillWithBlanks(const unsigned int &line);
	void setupCharset();

	sf::Vector2u _cursor;
	sf::RenderWindow *_window;
	sf::Vector2u _size;

	unsigned int _numberOfCollums;
	unsigned int _numberOfRows;
	unsigned int _spaceBetweenLines;

	float _fOSC = 270000.f; //in Hz
	bool _display;
	bool _cursorState;
	bool _cursorBlink;

	sf::Time _blickingRate;
	sf::Clock *_clock; // starts the clock
	sf::Texture _texture;
	std::vector<std::string> _linesStr;
	sf::RectangleShape _body;

	std::map<char, std::vector<uint8_t>> _charset;
};

#endif // !HARDWARE_H_


