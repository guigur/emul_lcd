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

	void setCursor(const unsigned int &col, const unsigned int &row);
	void print(const std::string &str);
	void setNumberOfCollums(const unsigned int &noc);
	unsigned int getNumberOfCollums();
	void setNumberOfRows(const unsigned int &nor);
	unsigned int getNumberOfRows();

private:
	void drawLine(sf::RenderTarget &target, int line) const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;
	void gui_thread();
	void fillWithBlanks(const unsigned int &line);

	sf::Vector2u _cursor;
	sf::RenderWindow *_window;
	sf::Vector2u _size;

	unsigned int _numberOfCollums;
	unsigned int _numberOfRows;

	unsigned int _spaceBetweenLines;

	sf::Texture _texture;
	std::vector<std::string> _linesStr;
	sf::RectangleShape _body;
};

#endif // !HARDWARE_H_


