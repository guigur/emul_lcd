#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Character.h"

class Hardware : public sf::RectangleShape
{
public:
	Hardware();
	void setText(const std::string &str);
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;
	sf::Texture _texture;
	sf::Font _font;
	std::string _ligne1;
	sf::RectangleShape _body;
};

#endif // !HARDWARE_H_


