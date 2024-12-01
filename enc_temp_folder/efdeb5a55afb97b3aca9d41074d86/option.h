#ifndef OPTION_H
#define OPTION_H

#include <SFML/Graphics.hpp>
#include <string>

void showOption();

std::wstring to_wstring(const std::string& str);

class Button {
private:
	sf::RectangleShape shape;
	const float Width = 200.f;
	const float Height = 50.f;
	sf::Font font;
	sf::Text text;
public:
	Button(float x, float y, std::string str, sf::Color color);
	void draw(sf::RenderWindow& window);
	bool isClicked(sf::Vector2f mousePos);
};

#endif