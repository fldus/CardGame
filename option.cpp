#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "option.h"
#include "home.h"

using namespace sf;
using namespace std;

enum App
{
	WIDTH = 600, HEIGHT = 500
};

class Dropdown {
private:
	RectangleShape box;
	Text label;
	Font font;
	vector<Text> options;
	bool isOpen = false;
	int selectedIndex = 0;	// 첫 번째 옵션 = 기본 선택지
public:
	Dropdown(float x, float y, const vector<string>& optionText) {
		if (optionText.empty())
			throw invalid_argument("Option list cannot be empty");

		box.setSize({ 200.f, 50.f });
		box.setFillColor(Color::White);
		box.setPosition(x, y);

		if (!font.loadFromFile("HANDotum.ttf")) {
			throw runtime_error("Font loading failed");
		}

		label.setFont(font);
		label.setCharacterSize(25);
		label.setFillColor(Color::Black);
		label.setString(optionText[0]);
		label.setPosition(x + 10.f, y + 10.f);

		for (size_t i = 0; i < optionText.size(); ++i) {
			Text option;
			option.setFont(font);
			option.setCharacterSize(20);
			option.setString(optionText[i]);
			option.setFillColor(Color::Black);
			option.setPosition(x + 10.f, y + 60.f + i * 30.f);
			options.push_back(option);
		}
	}
	void draw(RenderWindow& window) {
		window.draw(box);
		window.draw(label);

		if (isOpen) {
			for (auto& option : options) {
				window.draw(option);
			}
		}
	}
	void handleEvent(Event event, Vector2f mousePos) {
		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
			if (box.getGlobalBounds().contains(mousePos)) {
				isOpen = !isOpen;
			}else if (isOpen) {
				for (size_t i = 0; i < options.size(); ++i) {
					if (options[i].getGlobalBounds().contains(mousePos)) {
						selectedIndex = i;
						label.setString(options[i].getString());
						isOpen = false;
						break;
					}
				}
			}
		}
	}
	string getSelectedOption() const {
		return options[selectedIndex].getString();
	}
};

void showOption() {
	RenderWindow window(VideoMode(App::WIDTH, App::HEIGHT), "Game Option");
	window.setFramerateLimit(60);

	// class 생성

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				window.close();
				showHome();
			}
		}
	}
	window.clear(Color::White);
	// class drow
	window.display();
}