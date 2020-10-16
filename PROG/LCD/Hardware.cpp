﻿#include "Hardware.h"

Hardware::Hardware()
{
	setupCharset();
	_spaceBetweenLines = 45;
	_clock = &sf::Clock(); //init da clock
	_blickingRate = sf::milliseconds(409.6f * 250000.f / _fOSC);

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
		if (_cursorBlink && _clock->getElapsedTime() >= _blickingRate) //Blinky blinky
		{
			if (_cursorState == true)
				_cursorState = false;
			else
				_cursorState = true;
			_clock->restart();
		}

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

void Hardware::blink(bool state)
{
	_cursorBlink = state;
	_cursorState = state;
	//_clock->restart();
}

void Hardware::cursor(bool state)
{
	_cursorState = state;
}

void Hardware::setCursor(const unsigned int &col, const unsigned int &row)
{
	_cursor.x = col;
	_cursor.y = row;
}

void Hardware::write(const uint8_t &c)
{
	std::string temp;

	temp = c;
	//debugChar(c);

	_linesStr.at(_cursor.y).replace(_cursor.x, 1, temp);
	_cursor.x = _cursor.x++;
	fillWithBlanks(_cursor.y);
}

void Hardware::createChar(const uint8_t &pos, uint8_t *character)
{
	std::vector<uint8_t> charmap;
	for (int i = 0; i < 8; i++)
	{
		charmap.push_back(character[i]);
	}

	_charset[pos] = charmap;
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

//////////////////////////////////////////////// DEBUG ////////////////////////////////////////////////
void Hardware::debugChar(const uint8_t c)
{
	std::cout << "O=====O";
	for (auto line : _charset[0x00])
	{
		std::cout << std::endl << "|";
		for (int i = 4; i >= 0; --i)
		{
			std::cout << ((line & (1 << i)) ? (char)219 : (char)255);
		}
		std::cout << "|";
	}
	std::cout << std::endl << "O=====O" << std::endl;
}

//////////////////////////////////////////// PRIVATE STUFF ////////////////////////////////////////////
void Hardware::drawLine(sf::RenderTarget& target, int line) const
{
	unsigned int spaceCharacters = 1;
	sf::Vector2f startPos(68, 100);

	Character charos(_charset);
	charos.setPixSize(5);

	for (unsigned int i = 0; i < _numberOfCollums && i < _linesStr.at(line).length(); i++)
	{
		charos.setChar(_linesStr.at(line).at(i));

		if (_cursorState && _cursor.x == i && _cursor.y == line)
		{
			if (_cursorState == true)
				charos.setChar(0xFF);
		}

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

void Hardware::setupCharset()
{
	//1st col 0x00 -> 0x0F
	//custom chars 0x00 -> 0x07

	// 0x08 -> 0x0F
	//reserved for the lcd ram

	//2nd col 0x10 -> 0x1F
	//empty

	//3rd col 0x20 -> 0x2F
	/* 0x20 */ _charset[' '] = { { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } };
	/* 0x21 */ _charset['!'] = { { 0x04 }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x00 }, { 0x00 }, { 0x04 }, { 0x00 } };
	/* 0x22 */ _charset['"'] = { { 0x0A }, { 0x0A }, { 0x0A }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } };
	/* 0x23 */ _charset['#'] = { { 0x0A }, { 0x0A }, { 0x1F }, { 0x0A }, { 0x1F }, { 0x0A }, { 0x0A }, { 0x00 } };
	/* 0x24 */ _charset['$'] = { { 0x04 }, { 0x0F }, { 0x14 }, { 0x0E }, { 0x05 }, { 0x1E }, { 0x04 }, { 0x00 } };
	/* 0x25 */ _charset['%'] = { { 0x18 }, { 0x19 }, { 0x02 }, { 0x04 }, { 0x08 }, { 0x13 }, { 0x03 }, { 0x00 } };
	/* 0x26 */ _charset['&'] = { { 0x0C }, { 0x12 }, { 0x14 }, { 0x08 }, { 0x15 }, { 0x12 }, { 0x0D }, { 0x00 } };
	/* 0x27 */ _charset['\''] = { { 0x0C }, { 0x04 }, { 0x08 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } };
	/* 0x28 */ _charset['('] = { { 0x02 }, { 0x04 }, { 0x08 }, { 0x08 }, { 0x08 }, { 0x04 }, { 0x02 }, { 0x00 } };
	/* 0x29 */ _charset[')'] = { { 0x08 }, { 0x04 }, { 0x02 }, { 0x02 }, { 0x02 }, { 0x04 }, { 0x08 }, { 0x00 } };
	/* 0x2A */ _charset['*'] = { { 0x00 }, { 0x04 }, { 0x15 }, { 0x0E }, { 0x15 }, { 0x04 }, { 0x00 }, { 0x00 } };
	/* 0x2B */ _charset['+'] = { { 0x00 }, { 0x04 }, { 0x04 }, { 0x1F }, { 0x04 }, { 0x04 }, { 0x00 }, { 0x00 } };
	/* 0x2C */ _charset[','] = { { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x0C }, { 0x04 }, { 0x08 }, { 0x00 } };
	/* 0x2D */ _charset['-'] = { { 0x00 }, { 0x00 }, { 0x00 }, { 0x1F }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } };
	/* 0x2E */ _charset['.'] = { { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x0C }, { 0x0C }, { 0x00 } };
	/* 0x2F */ _charset['/'] = { { 0x00 }, { 0x01 }, { 0x02 }, { 0x04 }, { 0x08 }, { 0x10 }, { 0x00 }, { 0x00 } };

	//4th col 0x30 -> 0x3F
	/* 0x30 */ _charset['0'] = { { 0x0E }, { 0x11 }, { 0x13 }, { 0x15 }, { 0x19 }, { 0x11 }, { 0x0E }, { 0x00 } };
	/* 0x31 */ _charset['1'] = { { 0x04 }, { 0x0C }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x0E }, { 0x00 } };
	/* 0x32 */ _charset['2'] = { { 0x0E }, { 0x11 }, { 0x01 }, { 0x02 }, { 0x04 }, { 0x08 }, { 0x1F }, { 0x00 } };
	/* 0x33 */ _charset['3'] = { { 0x1F }, { 0x02 }, { 0x04 }, { 0x02 }, { 0x01 }, { 0x11 }, { 0x0E }, { 0x00 } };
	/* 0x34 */ _charset['4'] = { { 0x02 }, { 0x06 }, { 0x0A }, { 0x12 }, { 0x1F }, { 0x02 }, { 0x02 }, { 0x00 } };
	/* 0x35 */ _charset['5'] = { { 0x1F }, { 0x10 }, { 0x1E }, { 0x01 }, { 0x01 }, { 0x11 }, { 0x0E }, { 0x00 } };
	/* 0x36 */ _charset['6'] = { { 0x06 }, { 0x08 }, { 0x10 }, { 0x1E }, { 0x11 }, { 0x11 }, { 0x0E }, { 0x00 } };
	/* 0x37 */ _charset['7'] = { { 0x1F }, { 0x11 }, { 0x01 }, { 0x02 }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x00 } };
	/* 0x38 */ _charset['8'] = { { 0x0E }, { 0x11 }, { 0x11 }, { 0x0E }, { 0x11 }, { 0x11 }, { 0x0E }, { 0x00 } };
	/* 0x39 */ _charset['9'] = { { 0x0E }, { 0x11 }, { 0x11 }, { 0x0F }, { 0x01 }, { 0x02 }, { 0x0C }, { 0x00 } };
	/* 0x3A */ _charset[':'] = { { 0x00 }, { 0x0C }, { 0x0C }, { 0x00 }, { 0x0C }, { 0x0C }, { 0x00 }, { 0x00 } };
	/* 0x3B */ _charset[';'] = { { 0x00 }, { 0x0C }, { 0x0C }, { 0x00 }, { 0x0C }, { 0x04 }, { 0x08 }, { 0x00 } };
	/* 0x3C */ _charset['<'] = { { 0x02 }, { 0x04 }, { 0x08 }, { 0x10 }, { 0x08 }, { 0x04 }, { 0x02 }, { 0x00 } };
	/* 0x3D */ _charset['='] = { { 0x00 }, { 0x00 }, { 0x1F }, { 0x00 }, { 0x1F }, { 0x00 }, { 0x00 }, { 0x00 } };
	/* 0x3E */ _charset['>'] = { { 0x08 }, { 0x04 }, { 0x02 }, { 0x01 }, { 0x02 }, { 0x04 }, { 0x08 }, { 0x00 } };
	/* 0x3F */ _charset['?'] = { { 0x0E }, { 0x11 }, { 0x01 }, { 0x02 }, { 0x04 }, { 0x00 }, { 0x04 }, { 0x00 } };

	//5th col 0x40 -> 0x4F
	/* 0x40 */ _charset['@'] = { { 0x0E }, { 0x11 }, { 0x01 }, { 0x0D }, { 0x15 }, { 0x15 }, { 0x0E }, { 0x00 } };
	/* 0x41 */ _charset['A'] = { { 0x0E }, { 0x11 }, { 0x11 }, { 0x1F }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x00 } };
	/* 0x42 */ _charset['B'] = { { 0x1E }, { 0x11 }, { 0x11 }, { 0x1E }, { 0x11 }, { 0x11 }, { 0x1E }, { 0x00 } };
	/* 0x43 */ _charset['C'] = { { 0x0E }, { 0x11 }, { 0x10 }, { 0x10 }, { 0x10 }, { 0x11 }, { 0x0E }, { 0x00 } };
	/* 0x44 */ _charset['D'] = { { 0x1C }, { 0x12 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x12 }, { 0x1C }, { 0x00 } };
	/* 0x45 */ _charset['E'] = { { 0x1F }, { 0x10 }, { 0x10 }, { 0x1E }, { 0x10 }, { 0x10 }, { 0x1F }, { 0x00 } };
	/* 0x46 */ _charset['F'] = { { 0x1F }, { 0x10 }, { 0x10 }, { 0x1E }, { 0x10 }, { 0x10 }, { 0x10 }, { 0x00 } };
	/* 0x47 */ _charset['G'] = { { 0x0E }, { 0x11 }, { 0x10 }, { 0x17 }, { 0x11 }, { 0x11 }, { 0x0F }, { 0x00 } };
	/* 0x48 */ _charset['H'] = { { 0x11 }, { 0x11 }, { 0x11 }, { 0x1F }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x00 } };
	/* 0x49 */ _charset['I'] = { { 0x0E }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x0E }, { 0x00 } };
	/* 0x4A */ _charset['J'] = { { 0x07 }, { 0x02 }, { 0x02 }, { 0x02 }, { 0x02 }, { 0x12 }, { 0x0C }, { 0x00 } };
	/* 0x4B */ _charset['K'] = { { 0x11 }, { 0x12 }, { 0x14 }, { 0x18 }, { 0x14 }, { 0x12 }, { 0x11 }, { 0x00 } };
	/* 0x4C */ _charset['L'] = { { 0x10 }, { 0x10 }, { 0x10 }, { 0x10 }, { 0x10 }, { 0x10 }, { 0x1F }, { 0x00 } };
	/* 0x4D */ _charset['M'] = { { 0x11 }, { 0x1B }, { 0x15 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x00 } };
	/* 0x4E */ _charset['N'] = { { 0x11 }, { 0x11 }, { 0x19 }, { 0x15 }, { 0x13 }, { 0x11 }, { 0x11 }, { 0x00 } };
	/* 0x4F */ _charset['O'] = { { 0x0E }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x0E }, { 0x00 } };

	//6th col 0x50 -> 0x5F
	/* 0x50 */ _charset['P'] = { { 0x1E }, { 0x11 }, { 0x11 }, { 0x1E }, { 0x10 }, { 0x10 }, { 0x10 }, { 0x00 } };
	/* 0x51 */ _charset['Q'] = { { 0x0E }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x15 }, { 0x12 }, { 0x0D }, { 0x00 } };
	/* 0x52 */ _charset['R'] = { { 0x1E }, { 0x11 }, { 0x11 }, { 0x1E }, { 0x14 }, { 0x12 }, { 0x11 }, { 0x00 } };
	/* 0x53 */ _charset['S'] = { { 0x0F }, { 0x10 }, { 0x10 }, { 0x0E }, { 0x01 }, { 0x01 }, { 0x1E }, { 0x00 } };
	/* 0x54 */ _charset['T'] = { { 0x1F }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x00 } };
	/* 0x55 */ _charset['U'] = { { 0x11 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x0E }, { 0x00 } };
	/* 0x56 */ _charset['V'] = { { 0x11 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x0A }, { 0x04 }, { 0x00 } };
	/* 0x57 */ _charset['W'] = { { 0x11 }, { 0x11 }, { 0x11 }, { 0x15 }, { 0x15 }, { 0x15 }, { 0x0A }, { 0x00 } };
	/* 0x58 */ _charset['X'] = { { 0x11 }, { 0x11 }, { 0x0A }, { 0x04 }, { 0x0A }, { 0x11 }, { 0x11 }, { 0x00 } };
	/* 0x59 */ _charset['Y'] = { { 0x11 }, { 0x11 }, { 0x11 }, { 0x0A }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x00 } };
	/* 0x5A */ _charset['Z'] = { { 0x1F }, { 0x01 }, { 0x02 }, { 0x04 }, { 0x08 }, { 0x10 }, { 0x1F }, { 0x00 } };
	/* 0x5B */ _charset['['] = { { 0x1C }, { 0x10 }, { 0x10 }, { 0x10 }, { 0x10 }, { 0x10 }, { 0x1C }, { 0x00 } };
	/* 0x5C */ _charset[0x5C] = { { 0x11 }, { 0x0A }, { 0x1F }, { 0x04 }, { 0x1F }, { 0x04 }, { 0x04 }, { 0x00 } }; //¥
	/* 0x5D */ _charset[']'] = { { 0x0E }, { 0x02 }, { 0x02 }, { 0x02 }, { 0x02 }, { 0x02 }, { 0x0E }, { 0x00 } };
	/* 0x5E */ _charset['^'] = { { 0x04 }, { 0x0A }, { 0x11 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } };
	/* 0x5F */ _charset['_'] = { { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x1F }, { 0x00 } };

	//7th col 0x60 -> 0x6F
	/* 0x60 */ _charset['`'] = { { 0x08 }, { 0x04 }, { 0x02 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } };
	/* 0x61 */ _charset['a'] = { { 0x00 }, { 0x00 }, { 0x0E }, { 0x01 }, { 0x0F }, { 0x11 }, { 0x0F }, { 0x00 } };
	/* 0x62 */ _charset['b'] = { { 0x10 }, { 0x10 }, { 0x16 }, { 0x19 }, { 0x11 }, { 0x11 }, { 0x1E }, { 0x00 } };
	/* 0x63 */ _charset['c'] = { { 0x00 }, { 0x00 }, { 0x0E }, { 0x10 }, { 0x10 }, { 0x11 }, { 0x0E }, { 0x00 } };
	/* 0x64 */ _charset['d'] = { { 0x01 }, { 0x01 }, { 0x0D }, { 0x13 }, { 0x11 }, { 0x11 }, { 0x0F }, { 0x00 } };
	/* 0x65 */ _charset['e'] = { { 0x00 }, { 0x00 }, { 0x0E }, { 0x11 }, { 0x1F }, { 0x10 }, { 0x0E }, { 0x00 } };
	/* 0x66 */ _charset['f'] = { { 0x06 }, { 0x09 }, { 0x08 }, { 0x1C }, { 0x08 }, { 0x08 }, { 0x08 }, { 0x00 } };
	/* 0x67 */ _charset['g'] = { { 0x00 }, { 0x0F }, { 0x11 }, { 0x11 }, { 0x0F }, { 0x01 }, { 0x0E }, { 0x00 } };
	/* 0x68 */ _charset['h'] = { { 0x10 }, { 0x10 }, { 0x16 }, { 0x19 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x00 } };
	/* 0x69 */ _charset['i'] = { { 0x04 }, { 0x00 }, { 0x0C }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x0E }, { 0x00 } };
	/* 0x6A */ _charset['j'] = { { 0x02 }, { 0x00 }, { 0x06 }, { 0x02 }, { 0x02 }, { 0x12 }, { 0x0C }, { 0x00 } };
	/* 0x6B */ _charset['k'] = { { 0x10 }, { 0x10 }, { 0x12 }, { 0x14 }, { 0x18 }, { 0x14 }, { 0x12 }, { 0x00 } };
	/* 0x6C */ _charset['l'] = { { 0x0C }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x0E }, { 0x00 } };
	/* 0x6D */ _charset['m'] = { { 0x00 }, { 0x00 }, { 0x1A }, { 0x15 }, { 0x15 }, { 0x11 }, { 0x11 }, { 0x00 } };
	/* 0x6E */ _charset['n'] = { { 0x00 }, { 0x00 }, { 0x16 }, { 0x19 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x00 } };
	/* 0x6F */ _charset['o'] = { { 0x00 }, { 0x00 }, { 0x0E }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x0E }, { 0x00 } };

	//8th col 0x70 -> 0x7F
	/* 0x70 */ _charset['p'] = { { 0x00 }, { 0x00 }, { 0x1E }, { 0x11 }, { 0x1E }, { 0x10 }, { 0x10 }, { 0x00 } };
	/* 0x71 */ _charset['q'] = { { 0x00 }, { 0x00 }, { 0x0D }, { 0x13 }, { 0x0F }, { 0x01 }, { 0x01 }, { 0x00 } };
	/* 0x72 */ _charset['r'] = { { 0x00 }, { 0x00 }, { 0x16 }, { 0x19 }, { 0x10 }, { 0x10 }, { 0x10 }, { 0x00 } };
	/* 0x73 */ _charset['s'] = { { 0x00 }, { 0x00 }, { 0x0E }, { 0x10 }, { 0x0E }, { 0x01 }, { 0x1E }, { 0x00 } };
	/* 0x74 */ _charset['t'] = { { 0x08 }, { 0x08 }, { 0x1C }, { 0x08 }, { 0x08 }, { 0x09 }, { 0x06 }, { 0x00 } };
	/* 0x75 */ _charset['u'] = { { 0x00 }, { 0x00 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x13 }, { 0x0D }, { 0x00 } };
	/* 0x76 */ _charset['v'] = { { 0x00 }, { 0x00 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x0A }, { 0x04 }, { 0x00 } };
	/* 0x77 */ _charset['w'] = { { 0x00 }, { 0x00 }, { 0x11 }, { 0x15 }, { 0x15 }, { 0x15 }, { 0x0A }, { 0x00 } };
	/* 0x78 */ _charset['x'] = { { 0x00 }, { 0x00 }, { 0x11 }, { 0x0A }, { 0x04 }, { 0x0A }, { 0x11 }, { 0x00 } };
	/* 0x79 */ _charset['y'] = { { 0x00 }, { 0x00 }, { 0x11 }, { 0x11 }, { 0x0F }, { 0x01 }, { 0x0E }, { 0x00 } };
	/* 0x7A */ _charset['z'] = { { 0x00 }, { 0x00 }, { 0x1F }, { 0x02 }, { 0x04 }, { 0x08 }, { 0x1F }, { 0x00 } };
	/* 0x7B */ _charset['{'] = { { 0x02 }, { 0x04 }, { 0x04 }, { 0x08 }, { 0x04 }, { 0x04 }, { 0x02 }, { 0x00 } };
	/* 0x7C */ _charset['|'] = { { 0x04 }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x00 } };
	/* 0x7D */ _charset['}'] = { { 0x08 }, { 0x04 }, { 0x04 }, { 0x02 }, { 0x04 }, { 0x04 }, { 0x08 }, { 0x00 } };
	/* 0x7E */ _charset[0x7E] = { { 0x00 }, { 0x04 }, { 0x02 }, { 0x1F }, { 0x02 }, { 0x04 }, { 0x00 }, { 0x00 } }; // →
	/* 0x7F */ _charset[0x7F] = { { 0x00 }, { 0x04 }, { 0x08 }, { 0x1F }, { 0x08 }, { 0x04 }, { 0x00 }, { 0x00 } }; // ←

	//9th & 10th col 0x80 -> 0x9F
	//empty

	//11th col 0xA0 -> 0xAF
	/* 0xA0 */ //empty
	/* 0xA1 */ _charset[0xA1] = { { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x1C }, { 0x14 }, { 0x1C }, { 0x00 } }; // º or ° or゜ ??
	/* 0xA2 */ _charset[0xA2] = { { 0x07 }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // ┌
	/* 0xA3 */ _charset[0xA3] = { { 0x00 }, { 0x00 }, { 0x00 }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x1C }, { 0x00 } }; // ┘
	/* 0xA4 */ _charset[0xA4] = { { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x10 }, { 0x08 }, { 0x04 }, { 0x00 } }; // ヽ iteration mark
	/* 0xA5 */ _charset[0xA5] = { { 0x00 }, { 0x00 }, { 0x00 }, { 0x0C }, { 0x0C }, { 0x00 }, { 0x00 }, { 0x00 } }; // ・ middle dot
	/* 0xA6 */ _charset[0xA6] = { { 0x00 }, { 0x1F }, { 0x01 }, { 0x1F }, { 0x01 }, { 0x02 }, { 0x04 }, { 0x00 } }; // ヲ wo
	/* 0xA7 */ _charset[0xA7] = { { 0x00 }, { 0x00 }, { 0x1F }, { 0x01 }, { 0x06 }, { 0x04 }, { 0x08 }, { 0x00 } }; // ァ small a 
	/* 0xA8 */ _charset[0xA8] = { { 0x00 }, { 0x00 }, { 0x02 }, { 0x04 }, { 0x0C }, { 0x14 }, { 0x04 }, { 0x00 } }; // ィ small i 
	/* 0xA9 */ _charset[0xA9] = { { 0x00 }, { 0x00 }, { 0x04 }, { 0x1F }, { 0x11 }, { 0x01 }, { 0x06 }, { 0x00 } }; // ゥ small u 
	/* 0xAA */ _charset[0xAA] = { { 0x00 }, { 0x00 }, { 0x00 }, { 0x1F }, { 0x04 }, { 0x04 }, { 0x1F }, { 0x00 } }; // ェ small e 
	/* 0xAB */ _charset[0xAB] = { { 0x00 }, { 0x00 }, { 0x02 }, { 0x1F }, { 0x06 }, { 0x0A }, { 0x12 }, { 0x00 } }; // ォ small o 
	/* 0xAC */ _charset[0xAC] = { { 0x00 }, { 0x00 }, { 0x08 }, { 0x1F }, { 0x09 }, { 0x0A }, { 0x08 }, { 0x00 } }; // small ya ャ
	/* 0xAD */ _charset[0xAD] = { { 0x00 }, { 0x00 }, { 0x00 }, { 0x0E }, { 0x02 }, { 0x02 }, { 0x1F }, { 0x00 } }; // small yu ュ
	/* 0xAE */ _charset[0xAE] = { { 0x00 }, { 0x00 }, { 0x1E }, { 0x02 }, { 0x1E }, { 0x02 }, { 0x1E }, { 0x00 } }; // small yo ョ
	/* 0xAF */ _charset[0xAF] = { { 0x00 }, { 0x00 }, { 0x00 }, { 0x15 }, { 0x15 }, { 0x01 }, { 0x06 }, { 0x00 } }; // small tu ッ or tu ツ ???

	//12th col 0xB0 -> 0xBF
	/* 0xB0 */ _charset[0xB0] = { { 0x00 }, { 0x00 }, { 0x00 }, { 0x1F }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // ー same as -
	/* 0xB1 */ _charset[0xB1] = { { 0x1F }, { 0x01 }, { 0x05 }, { 0x06 }, { 0x04 }, { 0x04 }, { 0x08 }, { 0x00 } }; // ア a
	/* 0xB2 */ _charset[0xB2] = { { 0x01 }, { 0x02 }, { 0x04 }, { 0x0C }, { 0x14 }, { 0x04 }, { 0x04 }, { 0x00 } }; // イ i
	/* 0xB3 */ _charset[0xB3] = { { 0x04 }, { 0x1F }, { 0x11 }, { 0x11 }, { 0x01 }, { 0x02 }, { 0x04 }, { 0x00 } }; // ウ u
	/* 0xB4 */ _charset[0xB4] = { { 0x00 }, { 0x1F }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x1F }, { 0x00 } }; // エ e
	/* 0xB5 */ _charset[0xB5] = { { 0x02 }, { 0x1F }, { 0x02 }, { 0x06 }, { 0x0A }, { 0x12 }, { 0x02 }, { 0x00 } }; // オ o
	/* 0xB6 */ _charset[0xB6] = { { 0x08 }, { 0x1F }, { 0x09 }, { 0x09 }, { 0x09 }, { 0x09 }, { 0x12 }, { 0x00 } }; // カ ka
	/* 0xB7 */ _charset[0xB7] = { { 0x04 }, { 0x1F }, { 0x04 }, { 0x1F }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x00 } }; // キ ki
	/* 0xB8 */ _charset[0xB8] = { { 0x00 }, { 0x0F }, { 0x09 }, { 0x11 }, { 0x01 }, { 0x02 }, { 0x0C }, { 0x00 } }; // ク ku
	/* 0xB9 */ _charset[0xB9] = { { 0x08 }, { 0x0F }, { 0x12 }, { 0x02 }, { 0x02 }, { 0x02 }, { 0x04 }, { 0x00 } }; // ケ ke
	/* 0xBA */ _charset[0xBA] = { { 0x1F }, { 0x01 }, { 0x01 }, { 0x01 }, { 0x01 }, { 0x01 }, { 0x1F }, { 0x00 } }; // コ ko
	/* 0xBB */ _charset[0xBB] = { { 0x0A }, { 0x1F }, { 0x0A }, { 0x0A }, { 0x02 }, { 0x04 }, { 0x08 }, { 0x00 } }; // サ sa
	/* 0xBC */ _charset[0xBC] = { { 0x00 }, { 0x18 }, { 0x01 }, { 0x19 }, { 0x01 }, { 0x02 }, { 0x1C }, { 0x00 } }; // シ shi
	/* 0xBD */ _charset[0xBD] = { { 0x00 }, { 0x1F }, { 0x01 }, { 0x02 }, { 0x04 }, { 0x0A }, { 0x11 }, { 0x00 } }; // ス su
	/* 0xBE */ _charset[0xBE] = { { 0x00 }, { 0x08 }, { 0x1F }, { 0x09 }, { 0x0A }, { 0x08 }, { 0x07 }, { 0x00 } }; // セ se
	/* 0xBF */ _charset[0xBF] = { { 0x00 }, { 0x11 }, { 0x11 }, { 0x09 }, { 0x01 }, { 0x02 }, { 0x0C }, { 0x00 } }; // ソ so

	//13th col 0xC0 -> 0xCF
	/* 0xC0 */ _charset[0xC0] = { { 0x00 }, { 0x0F }, { 0x09 }, { 0x15 }, { 0x03 }, { 0x02 }, { 0x0C }, { 0x00 } }; // タ ta
	/* 0xC1 */ _charset[0xC1] = { { 0x02 }, { 0x1C }, { 0x04 }, { 0x1F }, { 0x04 }, { 0x04 }, { 0x08 }, { 0x00 } }; // チ chi
	/* 0xC2 */ _charset[0xC2] = { { 0x00 }, { 0x15 }, { 0x15 }, { 0x15 }, { 0x01 }, { 0x02 }, { 0x04 }, { 0x00 } }; // ツ tsu
	/* 0xC3 */ _charset[0xC3] = { { 0x0E }, { 0x00 }, { 0x1F }, { 0x04 }, { 0x04 }, { 0x04 }, { 0x08 }, { 0x00 } }; // テ te
	/* 0xC4 */ _charset[0xC4] = { { 0x08 }, { 0x08 }, { 0x08 }, { 0x0C }, { 0x0A }, { 0x08 }, { 0x08 }, { 0x00 } }; // ト to
	/* 0xC5 */ _charset[0xC5] = { { 0x04 }, { 0x04 }, { 0x1F }, { 0x04 }, { 0x04 }, { 0x08 }, { 0x10 }, { 0x00 } }; // ナ na
	/* 0xC6 */ _charset[0xC6] = { { 0x00 }, { 0x0E }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x1F }, { 0x00 } }; // ニ ni
	/* 0xC7 */ _charset[0xC7] = { { 0x00 }, { 0x1F }, { 0x01 }, { 0x0A }, { 0x04 }, { 0x0A }, { 0x10 }, { 0x00 } }; // ヌ nu
	/* 0xC8 */ _charset[0xC8] = { { 0x04 }, { 0x1F }, { 0x02 }, { 0x04 }, { 0x0E }, { 0x15 }, { 0x04 }, { 0x00 } }; // ネ ne
	/* 0xC9 */ _charset[0xC9] = { { 0x02 }, { 0x02 }, { 0x02 }, { 0x02 }, { 0x02 }, { 0x04 }, { 0x08 }, { 0x00 } }; // ノ no
	/* 0xCA */ _charset[0xCA] = { { 0x00 }, { 0x04 }, { 0x02 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x00 } }; // ハ ha
	/* 0xCB */ _charset[0xCB] = { { 0x10 }, { 0x10 }, { 0x1F }, { 0x10 }, { 0x10 }, { 0x10 }, { 0x0F }, { 0x00 } }; // ヒ hi
	/* 0xCC */ _charset[0xCC] = { { 0x00 }, { 0x1F }, { 0x01 }, { 0x01 }, { 0x01 }, { 0x02 }, { 0x0C }, { 0x00 } }; // フ fu
	/* 0xCD */ _charset[0xCD] = { { 0x00 }, { 0x08 }, { 0x14 }, { 0x02 }, { 0x01 }, { 0x01 }, { 0x00 }, { 0x00 } }; // ヘ he
	/* 0xCE */ _charset[0xCE] = { { 0x04 }, { 0x1F }, { 0x04 }, { 0x04 }, { 0x15 }, { 0x15 }, { 0x04 }, { 0x00 } }; // ホ ho
	/* 0xCF */ _charset[0xCF] = { { 0x00 }, { 0x1F }, { 0x01 }, { 0x01 }, { 0x0A }, { 0x04 }, { 0x02 }, { 0x00 } }; // マ ma

	//14th col 0xD0 -> 0xDF
	/* 0xD0 */ _charset[0xD0] = { { 0x00 }, { 0x0E }, { 0x00 }, { 0x0E }, { 0x00 }, { 0x0E }, { 0x01 }, { 0x00 } }; // ミ mi
	/* 0xD1 */ _charset[0xD1] = { { 0x00 }, { 0x04 }, { 0x08 }, { 0x10 }, { 0x11 }, { 0x1F }, { 0x01 }, { 0x00 } }; // ム mu
	/* 0xD2 */ _charset[0xD2] = { { 0x00 }, { 0x01 }, { 0x01 }, { 0x0A }, { 0x04 }, { 0x0A }, { 0x10 }, { 0x00 } }; // メ me
	/* 0xD3 */ _charset[0xD3] = { { 0x00 }, { 0x1F }, { 0x08 }, { 0x1F }, { 0x08 }, { 0x08 }, { 0x07 }, { 0x00 } }; // モ mo
	/* 0xD4 */ _charset[0xD4] = { { 0x08 }, { 0x1F }, { 0x09 }, { 0x0A }, { 0x08 }, { 0x08 }, { 0x08 }, { 0x00 } }; // ヤ ya
	/* 0xD5 */ _charset[0xD5] = { { 0x00 }, { 0x0E }, { 0x02 }, { 0x02 }, { 0x02 }, { 0x02 }, { 0x1F }, { 0x00 } }; // ユ yu
	/* 0xD6 */ _charset[0xD6] = { { 0x00 }, { 0x1F }, { 0x01 }, { 0x1F }, { 0x01 }, { 0x01 }, { 0x1F }, { 0x00 } }; // ヨ yo
	/* 0xD7 */ _charset[0xD7] = { { 0x0E }, { 0x00 }, { 0x1F }, { 0x01 }, { 0x01 }, { 0x02 }, { 0x04 }, { 0x00 } }; // ラ ra
	/* 0xD8 */ _charset[0xD8] = { { 0x12 }, { 0x12 }, { 0x12 }, { 0x12 }, { 0x02 }, { 0x04 }, { 0x08 }, { 0x00 } }; // リ ri
	/* 0xD9 */ _charset[0xD9] = { { 0x00 }, { 0x04 }, { 0x14 }, { 0x14 }, { 0x15 }, { 0x15 }, { 0x16 }, { 0x00 } }; // ル ru
	/* 0xDA */ _charset[0xDA] = { { 0x00 }, { 0x10 }, { 0x10 }, { 0x11 }, { 0x12 }, { 0x14 }, { 0x18 }, { 0x00 } }; // レ re
	/* 0xDB */ _charset[0xDB] = { { 0x00 }, { 0x1F }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x1F }, { 0x00 } }; // ロ ro
	/* 0xDC */ _charset[0xDC] = { { 0x00 }, { 0x1F }, { 0x11 }, { 0x11 }, { 0x01 }, { 0x02 }, { 0x04 }, { 0x00 } }; // ワ wa
	/* 0xDD */ _charset[0xDD] = { { 0x00 }, { 0x18 }, { 0x00 }, { 0x01 }, { 0x01 }, { 0x02 }, { 0x1C }, { 0x00 } }; // ン n ????
	/* 0xDE */ _charset[0xDE] = { { 0x04 }, { 0x12 }, { 0x08 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // ゛ voiced sound mark
	/* 0xDF */ _charset[0xDF] = { { 0x1C }, { 0x14 }, { 0x1C }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // ゜ semi voiced sound mark

	//15th col 0xEF -> 0xEF
	/* 0xE0 */ _charset[0xE0] = { { 0x00 }, { 0x00 }, { 0x09 }, { 0x15 }, { 0x12 }, { 0x12 }, { 0x0D }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // α alpha
	/* 0xE1 */ _charset[0xE1] = { { 0x0A }, { 0x00 }, { 0x0E }, { 0x01 }, { 0x0F }, { 0x11 }, { 0x0F }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // ä 
	/* 0xE2 */ _charset[0xE2] = { { 0x00 }, { 0x00 }, { 0x0E }, { 0x11 }, { 0x1E }, { 0x11 }, { 0x1E }, { 0x10 }, { 0x10 }, { 0x10 }, { 0x00 } }; // β beta
	/* 0xE3 */ _charset[0xE3] = { { 0x00 }, { 0x00 }, { 0x0E }, { 0x10 }, { 0x0C }, { 0x11 }, { 0x0E }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // ε epsilon
	/* 0xE4 */ _charset[0xE4] = { { 0x00 }, { 0x00 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x13 }, { 0x1D }, { 0x10 }, { 0x10 }, { 0x10 }, { 0x00 } }; // μ mu
	/* 0xE5 */ _charset[0xE5] = { { 0x00 }, { 0x00 }, { 0x0F }, { 0x14 }, { 0x12 }, { 0x11 }, { 0x0E }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // δ delta or σ signa ??
	/* 0xE6 */ _charset[0xE6] = { { 0x00 }, { 0x00 }, { 0x06 }, { 0x09 }, { 0x11 }, { 0x11 }, { 0x1E }, { 0x10 }, { 0x10 }, { 0x10 }, { 0x00 } }; // ρ rho
	/* 0xE7 */ _charset[0xE7] = { { 0x00 }, { 0x00 }, { 0x0F }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x0F }, { 0x01 }, { 0x01 }, { 0x0E }, { 0x00 } }; // big g (for grams ??)
	/* 0xE8 */ _charset[0xE8] = { { 0x00 }, { 0x00 }, { 0x07 }, { 0x04 }, { 0x04 }, { 0x14 }, { 0x08 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // √ square root
	/* 0xE9 */ _charset[0xE9] = { { 0x00 }, { 0x02 }, { 0x1A }, { 0x02 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // ¯¹ small -1
	/* 0xEA */ _charset[0xEA] = { { 0x02 }, { 0x00 }, { 0x06 }, { 0x02 }, { 0x02 }, { 0x02 }, { 0x02 }, { 0x02 }, { 0x12 }, { 0x0C }, { 0x00 } }; // big j
	/* 0xEB */ _charset[0xEB] = { { 0x00 }, { 0x14 }, { 0x08 }, { 0x14 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // ͯ  small x
	/* 0xEC */ _charset[0xEC] = { { 0x00 }, { 0x04 }, { 0x0E }, { 0x14 }, { 0x15 }, { 0x0E }, { 0x04 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // ¢
	/* 0xED */ _charset[0xED] = { { 0x08 }, { 0x08 }, { 0x1C }, { 0x1C }, { 0x08 }, { 0x00 }, { 0x0F }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // ₤
	/* 0xEE */ _charset[0xEE] = { { 0x0E }, { 0x00 }, { 0x16 }, { 0x19 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // ñ
	/* 0xEF */ _charset[0xEF] = { { 0x0A }, { 0x00 }, { 0x0E }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x0E }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // ö

	//16th col 0xF0 -> 0xFF
	/* 0xF0 */ _charset[0xF0] = { { 0x00 }, { 0x00 }, { 0x16 }, { 0x19 }, { 0x11 }, { 0x11 }, { 0x1E }, { 0x10 }, { 0x10 }, { 0x10 }, { 0x00 } }; // big p
	/* 0xF1 */ _charset[0xF1] = { { 0x00 }, { 0x00 }, { 0x0D }, { 0x13 }, { 0x11 }, { 0x11 }, { 0x0F }, { 0x01 }, { 0x01 }, { 0x01 }, { 0x00 } }; // big q
	/* 0xF2 */ _charset[0xF2] = { { 0x00 }, { 0x0E }, { 0x11 }, { 0x1F }, { 0x11 }, { 0x11 }, { 0x0E }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // θ theta
	/* 0xF3 */ _charset[0xF3] = { { 0x00 }, { 0x00 }, { 0x00 }, { 0x0B }, { 0x15 }, { 0x1A }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // ∞ infinity
	/* 0xF4 */ _charset[0xF4] = { { 0x00 }, { 0x00 }, { 0x0E }, { 0x11 }, { 0x11 }, { 0x0A }, { 0x1B }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // Ω omega
	/* 0xF5 */ _charset[0xF5] = { { 0x0A }, { 0x00 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x13 }, { 0x0D }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // ü
	/* 0xF6 */ _charset[0xF6] = { { 0x1F }, { 0x10 }, { 0x08 }, { 0x04 }, { 0x08 }, { 0x10 }, { 0x1F }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // Σ sigma
	/* 0xF7 */ _charset[0xF7] = { { 0x00 }, { 0x00 }, { 0x1F }, { 0x0A }, { 0x0A }, { 0x0A }, { 0x13 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // π pi
	/* 0xF8 */ _charset[0xF8] = { { 0x1F }, { 0x00 }, { 0x11 }, { 0x0A }, { 0x04 }, { 0x0A }, { 0x11 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // X̅ x bar
	/* 0xF8 */ _charset[0xF8] = { { 0x1F }, { 0x00 }, { 0x11 }, { 0x0A }, { 0x04 }, { 0x0A }, { 0x11 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // X̅ x bar
	/* 0xF9 */ _charset[0xF9] = { { 0x00 }, { 0x00 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x11 }, { 0x0F }, { 0x01 }, { 0x01 }, { 0x0E }, { 0x00 } }; // big y
	/* 0xFA */ _charset[0xFA] = { { 0x00 }, { 0x01 }, { 0x1E }, { 0x04 }, { 0x1F }, { 0x04 }, { 0x04 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // 千 sen 1000
	/* 0xFB */ _charset[0xFB] = { { 0x00 }, { 0x00 }, { 0x1F }, { 0x08 }, { 0x0F }, { 0x09 }, { 0x11 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // 万 man 10 000
	/* 0xFC */ _charset[0xFC] = { { 0x00 }, { 0x00 }, { 0x1F }, { 0x15 }, { 0x1F }, { 0x11 }, { 0x11 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // 円 en
	/* 0xFD */ _charset[0xFD] = { { 0x00 }, { 0x00 }, { 0x04 }, { 0x00 }, { 0x1F }, { 0x00 }, { 0x04 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // ÷
	/* 0xFE */ _charset[0xFE] = { { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 }, { 0x00 } }; // nbsp same as ' '
	/* 0xFF */ _charset[0xFF] = { { 0x1F }, { 0x1F }, { 0x1F }, { 0x1F }, { 0x1F }, { 0x1F }, { 0x1F }, { 0x1F }, { 0x00 }, { 0x00 }, { 0x00 } }; // █ cursor
}