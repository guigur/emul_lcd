#include "Hardware.h"
#include <algorithm> 

Hardware::Hardware()
{

	if (!_texture.loadFromFile("medias/lcd.png"))
	{
		std::cout << "cannot load texture" << std::endl;
		exit(-1);
	}

	// map a 100x100 textured rectangle to the shape

	_body.setSize(sf::Vector2f(847/2, 380/2));
	_body.setTexture(&_texture);
	

	if (!_font.loadFromFile("medias/ArialCE.ttf"))
	{
		std::cout << "cannot load font" << std::endl;
		exit(-1);
	}
}

void Hardware::setText(const std::string &str)
{
	_ligne1 = str;
}

void Hardware::drawLine(sf::RenderTarget& target, int line)
{
	unsigned int spaceCharacters = 2;

	sf::Vector2f startPos(53, 64);

	Character charos;
	charos.setPixSize(5, 5);

	for (unsigned int i = 0; i < 15 && i < _ligne1.length(); i++)
	{
		charos.setChar(_ligne1.at(i));
		charos.setPosition(startPos.x + (charos.getPixSize().x * 5 + spaceCharacters) * i, startPos.y + 0);
		target.draw(charos);
	}
}

void Hardware::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_body);
	//this->drawLine(target, 0);
	//drawLine(target, 1);



	//target.draw(_ligne1);	
}
