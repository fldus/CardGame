#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

enum App
{
	WIDTH = 1100, HEIGHT = 800
};

class StartButton {
private:
	sf::RectangleShape shape;
	const float StartButtonWidth = 600.f;
	const float StartButtonHeight = 100.f;
	Font font;
	Text start;
public:
	StartButton(float x, float y) {
		shape.setPosition(x, y);
		shape.setSize({ StartButtonWidth, StartButtonHeight});
		shape.setFillColor(sf::Color::Yellow);
		shape.setOrigin(StartButtonWidth / 2.f, StartButtonHeight / 2.f);

		font.loadFromFile("HANDotum.ttf");
		start.setFont(font);
		start.setCharacterSize(65);
		start.setString(L"게임 시작");
		start.setFillColor(Color::Black);
		start.setPosition(x, y);
		start.setOrigin(start.getGlobalBounds().width / 2.f, start.getGlobalBounds().height / 2.f);
	}
	void draw(RenderWindow& window) {
		window.draw(shape);
		window.draw(start);
	}
};

int main(void)
{
	RenderWindow window(VideoMode(App::WIDTH, App::HEIGHT), "Card Game");
	// 초당 프레임 조절(컴퓨터 성능과 상관없이 일정한 속도)
	window.setFramerateLimit(60);

	StartButton sbutton(App::WIDTH / 2, App::HEIGHT / 2);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		
		window.clear(Color::White);
		sbutton.draw(window);
		window.display();
	}
}