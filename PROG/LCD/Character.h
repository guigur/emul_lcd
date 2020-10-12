#ifndef CHARACTER_H_
#define CHARACTER_H_
#include <vector>
#include <SFML/Graphics.hpp>

struct line
{
	uint8_t pixels;
};

class Character : public sf::RectangleShape
{
	public:
		Character();
		Character(const char &lcd_char);
		void setChar(const char &lcd_char);
		void setPixSize(const float sizeX, const float sizeY = -1);
		sf::Vector2f getPixSize();

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
		std::vector<struct line> _character;
		std::map<char, std::vector<struct line>> charset;

};

#endif // !CHARACTER_H_
