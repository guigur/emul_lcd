#include "Hardware.h"
#include <algorithm> 

Hardware::Hardware(const sf::Vector2u &size)
{
	_size = size;
	_spaceBetweenLines = 45;
	_numberOfCharsInLine = 16;
	_numberOfCollumns = 2;

	for (int i = 0; i < _numberOfCollumns; i++) //init the vector
	{
		linesStr.push_back("");
	}

	if (!_texture.loadFromFile("medias/lcd.png"))
	{
		std::cout << "cannot load texture" << std::endl;
		exit(-1);
	}

	_body.setSize((sf::Vector2f)size);
	_body.setTexture(&_texture);
}

void Hardware::setText(const std::string &str, const unsigned int &line)
{
	linesStr.at(line) = str;
}

void Hardware::drawLine(sf::RenderTarget& target, int line) const
{
	unsigned int spaceCharacters = 1;

	sf::Vector2f startPos(68, 100);

	Character charos;
	charos.setPixSize(5);

	for (unsigned int i = 0; i < _numberOfCharsInLine && i < linesStr.at(line).length(); i++)
	{
		charos.setChar(linesStr.at(line).at(i));
		charos.setPosition(startPos.x + (charos.getPixSize().x * 5 + spaceCharacters) * i, startPos.y + 0 + (line * _spaceBetweenLines));
		target.draw(charos);
	}
}

void Hardware::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_body);
	for (int i = 0; i < _numberOfCollumns; i++) //draw each line
	{
		drawLine(target, i);
	}
}
