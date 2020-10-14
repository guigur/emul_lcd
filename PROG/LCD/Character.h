#ifndef CHARACTER_H_
#define CHARACTER_H_
#include <vector>
#include <SFML/Graphics.hpp>

class Character : public sf::RectangleShape
{
	public:
		Character(const std::map<char, std::vector<uint8_t>> &charset, const char &lcd_char = 'O');

		void setChar(const char &lcd_char);
		void setPixSize(const float sizeX, const float sizeY = -1);
		sf::Vector2f getPixSize();
		//void blinkCursor(const sf::Vector2u &posCursor);

	private:
		void init();
		void searchAndSet();

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;
		
		char _lcd_char;
		unsigned int _width;
		unsigned int _height;
		double _sizePixX;
		double _sizePixY;
		sf::Color _colorLetter;
		sf::Color _colorBackground;
		std::vector<uint8_t> _character;
		std::map<char, std::vector<uint8_t>> _charset;
};

#endif // !CHARACTER_H_
