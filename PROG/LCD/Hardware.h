#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Character.h"

class Hardware : public sf::RectangleShape
{
public:
	Hardware(const sf::Vector2u &size);
	void setText(const std::string &str, const unsigned int &line = 0);
private:
	void drawLine(sf::RenderTarget &target, int line) const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;
	sf::Vector2u _size;
	unsigned int _spaceBetweenLines;
	unsigned int _numberOfCharsInLine;
	unsigned int _numberOfCollumns;
	sf::Texture _texture;
	std::vector<std::string> linesStr;
	sf::RectangleShape _body;
};

#endif // !HARDWARE_H_


