#include <SFML/Graphics.hpp>
#include <string>
#include "home.h"
#include "option.h"

using namespace sf;
using namespace std;

enum App
{
	WIDTH = 900, HEIGHT = 700
};

Record::Record(float x, float y, string mode, string slevel)
{
	if (!font.loadFromFile("HANDotum.ttf")) {
		throw runtime_error("Font loading failed");
	}

	cardMode.setFont(font);
	cardMode.setCharacterSize(30);
	cardMode.setString(to_wstring(mode));
	cardMode.setFillColor(Color::Black);
	cardMode.setPosition(x, y);
	cardMode.setOrigin(
		cardMode.getGlobalBounds().width / 2.f,
		cardMode.getGlobalBounds().height / 2.f);

	startlevel.setFont(font);
	startlevel.setCharacterSize(30);
	startlevel.setString(to_wstring(slevel));
	startlevel.setFillColor(Color::Black);
	startlevel.setPosition(x+170.f, y);
	startlevel.setOrigin(
		startlevel.getGlobalBounds().width / 2.f, 
		startlevel.getGlobalBounds().height / 2.f);

	text.setFont(font);
	text.setCharacterSize(30);
	text.setString("Lv." + to_string(level) + "        " + to_string(score));
	text.setFillColor(Color::Black);
	text.setPosition(x+365.f, y);
	text.setOrigin(
		text.getGlobalBounds().width / 2.f,
		text.getGlobalBounds().height / 2.f);
}
void Record::draw(RenderWindow& window) {
	window.draw(cardMode);
	window.draw(startlevel);
	window.draw(text);
}
void Record::update(int l, int s) {
	if (level < l) {
		level = l; score = s;
	}
	else if (level == l && score < s) {
		score = s;
	}
	text.setString("Lv." + to_string(level) + "        " + to_string(score));
}

class Start {
private:
	RectangleShape box;
	RectangleShape shape;
	const float StartButtonWidth = 600.f;
	const float StartButtonHeight = 100.f;
	Font font;
	Text title;
	Text label;
	Text start;
public:
	Start(float x, float y) {
		if (!font.loadFromFile("HANDotum.ttf")) {
			throw runtime_error("Font loading failed");
		}

		shape.setSize({ StartButtonWidth, StartButtonHeight});
		shape.setFillColor(Color(255, 238, 173));
		shape.setPosition(x, y+100.f);
		shape.setOrigin(StartButtonWidth / 2.f, StartButtonHeight / 2.f);

		box.setSize({ StartButtonWidth, 450.f });
		box.setFillColor(Color(217, 217, 217));
		box.setOutlineThickness(1.f);
		box.setOutlineColor(Color::Black);
		box.setPosition(x, y - 200.f);
		box.setOrigin(box.getSize().x / 2.f, box.getSize().y / 2.f);

		title.setFont(font);
		title.setCharacterSize(45);
		title.setString(L"게임 최고 기록");
		title.setFillColor(Color::Black);
		title.setPosition(
			box.getPosition().x - title.getGlobalBounds().width / 2.f,
			box.getPosition().y - 210.f);

		label.setFont(font);
		label.setCharacterSize(35);
		label.setString(L"카드 모드     난이도     레벨     점수");
		label.setFillColor(Color::Black);
		label.setPosition(
			box.getPosition().x - label.getGlobalBounds().width / 2.f,
			box.getPosition().y - 140.f);

		start.setFont(font);
		start.setCharacterSize(65);
		start.setString(L"게임 시작");
		start.setFillColor(Color::Black);
		start.setOrigin(start.getGlobalBounds().width / 2.f, start.getGlobalBounds().height / 1.4f);
		start.setPosition(shape.getPosition().x, shape.getPosition().y);
	}
	void draw(RenderWindow& window) {
		window.draw(box);
		window.draw(title);
		window.draw(label);
		window.draw(shape);
		window.draw(start);
	}
	bool isClicked(Vector2f mousePos) {
		return shape.getGlobalBounds().contains(mousePos);
	}
};

Record be(255.f, 190.f, u8"기본 카드", u8"쉬움");
Record bc(255.f, 240.f, u8"기본 카드", u8"보통");
Record bh(255.f, 290.f, u8"기본 카드", u8"어려움");
Record ie(255.f, 340.f, u8"아이템 카드", u8"쉬움");
Record ic(255.f, 390.f, u8"아이템 카드", u8"보통");
Record ih(255.f, 440.f, u8"아이템 카드", u8"어려움");

void showHome()
{
	RenderWindow window(VideoMode(App::WIDTH, App::HEIGHT), "Card Game");
	// 초당 프레임 조절(컴퓨터 성능과 상관없이 일정한 속도)
	window.setFramerateLimit(60);

	Start sbutton(App::WIDTH / 2, (App::HEIGHT / 3) * 2);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				Vector2f mousePos(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
				if (sbutton.isClicked(mousePos)) {
					window.close();
					showOption();
					return;
				}
			}
		}
		window.clear(Color::White);
		sbutton.draw(window);
		be.draw(window);
		bc.draw(window);
		bh.draw(window);
		ie.draw(window);
		ic.draw(window);
		ih.draw(window);
		window.display();
	}
}