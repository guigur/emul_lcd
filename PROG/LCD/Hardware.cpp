#include "Hardware.h"

Hardware::Hardware()
{
	_spaceBetweenLines = 45;

	if (!_texture.loadFromFile("medias/lcd.png"))
	{
		std::cout << "cannot load texture" << std::endl;
		exit(-1);
	}



	std::thread gui(&Hardware::gui_thread, this);     // spawn new thread that calls gui_thread()
	gui.detach();
}

void Hardware::clear()
{
	for (int i = 0; i < _numberOfRows; i++) //init the vector
	{
		linesStr.at(i) = "";
	}
}

void Hardware::gui_thread()
{
	sf::Vector2u size;
	size.x = 550;
	int ratio = 847 / 380;
	size.y = size.x / ratio;
	_size = size;


	sf::RenderWindow window(sf::VideoMode(size.x, size.y), "LCD 16x2");
	_window = &window;

	_body.setSize((sf::Vector2f)size);
	_body.setTexture(&_texture);



	while (_window->isOpen())
	{
		sf::Event event;
		while (_window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				_window->close();
		}

		_window->clear();
		_window->draw(*this);
		_window->display();
	}
}


void Hardware::setText(const std::string &str, const unsigned int &line)
{
	if (!linesStr.empty() && linesStr.size() > line)
	{
		linesStr.at(line) = str;
	}
}

void Hardware::setNumberOfCollums(const unsigned int &noc)
{
	_numberOfCollums = noc;
}

unsigned int Hardware::getNumberOfCollums()
{
	return _numberOfCollums;
}

void Hardware::setNumberOfRows(const unsigned int &nor)
{
	_numberOfRows = nor;
	linesStr.clear();
	std::cout << "old size " << linesStr.size() << std::endl;
	for (int i = 0; i < _numberOfRows; i++) //init the vector
	{
		linesStr.push_back("");
	}
	std::cout << "new size " << linesStr.size() << std::endl;

}

unsigned int Hardware::getNumberOfRows()
{
	return _numberOfRows;
}

////////////////////////////////////////////////////////////////////////////
void Hardware::drawLine(sf::RenderTarget& target, int line) const
{
	unsigned int spaceCharacters = 1;

	sf::Vector2f startPos(68, 100);

	Character charos;
	charos.setPixSize(5);

	for (unsigned int i = 0; i < _numberOfCollums && i < linesStr.at(line).length(); i++)
	{
		charos.setChar(linesStr.at(line).at(i));
		charos.setPosition(startPos.x + (charos.getPixSize().x * 5 + spaceCharacters) * i, startPos.y + 0 + (line * _spaceBetweenLines));
		target.draw(charos);
	}
}

void Hardware::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_body);

	for (int i = 0; i < _numberOfRows; i++) //draw each line
	{
		drawLine(target, i);
	}
}
