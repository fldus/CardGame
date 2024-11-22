#include <SFML/Graphics.hpp>
#include <locale>
#include <codecvt>
#include <vector>
#include <string>
#include "option.h"
#include "home.h"
#pragma execution_character_set("utf-8")

using namespace sf;
using namespace std;

enum App
{
	WIDTH = 600, HEIGHT = 500
};

// UTF-8 -> UTF-16
std::wstring to_wstring(const std::string& str) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(str);
}

class Dropdown {
private:
	RectangleShape box;
	Text Title;
	Text label;
	Font font;
	vector<Text> options;
	vector<RectangleShape> optionBoxes;
	bool isOpen = false;
	int selectedIndex = 0;	// 첫 번째 옵션 = 기본 선택지
public:
	Dropdown(float x, float y, string title, const vector<string>& optionText) {
		if (optionText.empty())
			throw invalid_argument("Option list cannot be empty");
		// font
		if (!font.loadFromFile("HANDotum.ttf")) {
			throw runtime_error("Font loading failed");
		}

		// menu title
		Title.setFont(font);
		Title.setCharacterSize(25);
		Title.setFillColor(Color::Black);
		Title.setString(to_wstring(title));
		Title.setPosition(x + 70.f, y - 50.f);

		// dropdown selected
		box.setSize({ 200.f, 50.f });
		box.setFillColor(Color::White);
		box.setOutlineThickness(2.f);
		box.setOutlineColor(Color::Black);
		box.setPosition(x, y);
			
		label.setFont(font);
		label.setCharacterSize(25);
		label.setFillColor(Color::Black);
		label.setString(to_wstring(optionText[0]));
		label.setPosition(
			box.getPosition().x + (box.getSize().x - label.getLocalBounds().width) / 2.f,
			box.getPosition().y + (box.getSize().y - label.getLocalBounds().height) / 2.f - 5.f
		);

		// dropdown option
		for (size_t i = 0; i < optionText.size(); ++i) {
			RectangleShape optionBox;
			optionBox.setSize({ box.getSize().x, 30.f });
			optionBox.setFillColor(Color::White);
			optionBox.setOutlineThickness(2.f);
			optionBox.setOutlineColor(Color::Black);
			optionBox.setPosition(box.getPosition().x, box.getPosition().y + 60.f + i * 30.f);
			optionBoxes.push_back(optionBox);

			Text option;
			option.setFont(font);
			option.setCharacterSize(20);
			option.setString(to_wstring(optionText[i]));
			option.setFillColor(Color::Black);
			option.setPosition(
				optionBox.getPosition().x + (optionBox.getSize().x - option.getLocalBounds().width) / 2.f - 5.f,
				optionBox.getPosition().y + (optionBox.getSize().y - option.getLocalBounds().height) / 2.f - 5.f
			);			
			options.push_back(option);
		}
	}
	void draw(RenderWindow& window) {
		window.draw(Title);
		window.draw(box);
		window.draw(label);

		if (isOpen) {
			for (size_t i = 0; i < options.size(); ++i) {
				window.draw(optionBoxes[i]);
				window.draw(options[i]);
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


	Dropdown mode = Dropdown((App::WIDTH / 7), App::HEIGHT / 3, "카드", { "기본 카드", "아이템 카드" });
	Dropdown level = Dropdown((App::WIDTH / 6) * 3, App::HEIGHT / 3, "난이도", {"쉬움", "보통", "어려움"});
	
	while (window.isOpen())
	{
		Event event;
		Vector2f mousePos(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
		
		while (window.pollEvent(event)){
			if (event.type == Event::Closed) {
				window.close();
				showHome();
				return;
			}
			mode.handleEvent(event, mousePos);
			level.handleEvent(event, mousePos);
		}

		window.clear(Color::White);
		mode.draw(window);
		level.draw(window);
		window.display();
	}
}