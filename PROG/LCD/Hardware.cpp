#include "Hardware.h"

Hardware::Hardware()
{
	_spaceBetweenLines = 45;
	
	if (!_texture.loadFromFile("medias/lcd.png"))
	{
		std::cout << "cannot load texture" << std::endl;
		exit(-1);
	}

	returnHome();

	std::thread gui(&Hardware::gui_thread, this);     // spawn new thread that calls gui_thread()
	gui.detach();
}

void Hardware::clear()
{
	for (int i = 0; i < _numberOfRows; i++) //init the vector
	{
		_linesStr.at(i).insert(0, _numberOfCollums, ' ');
	}
}

void Hardware::returnHome()
{
	_cursor = { 0, 0 };
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

void Hardware::setCursor(const unsigned int &col, const unsigned int &row)
{
	_cursor.x = col;
	_cursor.y = row;
}

void Hardware::print(const std::string &str)
{
	//_linesStr.at(_cursor.y).insert(_cursor.x, str);
	_linesStr.at(_cursor.y).replace(_cursor.x, str.length(), str);
	_cursor.x = _cursor.x + str.length();
	fillWithBlanks(_cursor.y);
}

void Hardware::setText(const std::string &str, const unsigned int &line)
{
	if (!_linesStr.empty() && _linesStr.size() > line)
	{
		_linesStr.at(line) = str;
		fillWithBlanks(line);
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
	_linesStr.clear(); //erase the vector if you rerun this command
	for (int i = 0; i < _numberOfRows; i++) //init the vector
	{
		_linesStr.push_back("");
	}
	clear(); //to fill screen with empty characters
}

unsigned int Hardware::getNumberOfRows()
{
	return _numberOfRows;
}

//////////////////////////////////////////// PRIVATE STUFF ////////////////////////////////////////////
void Hardware::drawLine(sf::RenderTarget& target, int line) const
{
	unsigned int spaceCharacters = 1;

	sf::Vector2f startPos(68, 100);

	Character charos;
	charos.setPixSize(5);

	for (unsigned int i = 0; i < _numberOfCollums && i < _linesStr.at(line).length(); i++)
	{
		charos.setChar(_linesStr.at(line).at(i));
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

void Hardware::fillWithBlanks(const unsigned int &line)
{
	if (_linesStr.at(line).length() < _numberOfCollums)
	{
		_linesStr.at(line).insert(_linesStr.at(line).end(), _numberOfCollums - _linesStr.at(line).length(), ' '); //fill the remaning space with spaces
	}
}