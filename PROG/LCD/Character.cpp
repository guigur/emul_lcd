#include "Character.h"
#include <iostream>

Character::Character(const std::map<char, std::vector<uint8_t>> &charset, const char &lcd_char)
{
	_charset = charset;
	_lcd_char = lcd_char;
	init();
}

void Character::init()
{
	_width = 5;
	_height = 8;
	setPixSize(3, 3);
	_colorBackground = sf::Color(125, 158, 51);
	_colorLetter = sf::Color(0, 49 , 0);

	searchAndSet();
}

void Character::searchAndSet()
{
	if (_charset.find(_lcd_char) == _charset.end())
	{
		_character = { {0},{0},{0},{0},{0},{0},{0},{0} };
	}
	else 
	{
		_character = _charset[_lcd_char];
	}
}

void Character::setChar(const char &lcd_char)
{
	_lcd_char = lcd_char;
	searchAndSet();
}

void Character::setPixSize(const float sizePixX, const float sizePixY)
{
	_sizePixX = sizePixX;

	if (sizePixY == -1)
		_sizePixY = sizePixX;
	else
		_sizePixY = sizePixY;
}

sf::Vector2f Character::getPixSize()
{
	sf::Vector2f size(_sizePixX, _sizePixY);
	return size;
}

//////////////////////////////////////////// PRIVATE STUFF ////////////////////////////////////////////
void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RectangleShape pix;
	sf::Vector2f currentPosition = getPosition();
	int scanGrid = 1;
	sf::Vector2f size(_sizePixX - scanGrid, _sizePixY - scanGrid);

	pix.setSize(size);

	for (unsigned int i = 0; i < _height; i++)
	{
		for (unsigned int j = 0; j < _width; j++)
		{
			if ((_character.at(i) >> j) & 1)
			{
				pix.setFillColor(_colorLetter);
			}
			else
			{
				pix.setFillColor(_colorBackground);
			}
			
			pix.setPosition(abs((int)j - (int)_width + 1) * (float)_sizePixX + currentPosition.x, 
				i * (float)_sizePixY + currentPosition.y - scanGrid); //flip the pixel order
			target.draw(pix);
		}
	}

}
