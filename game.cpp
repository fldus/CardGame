#include <SFML/Graphics.hpp>
#include "home.h"
#include "option.h"
#include "game.h"

using namespace sf;
using namespace std;

enum App
{
	WIDTH = 1100, HEIGHT = 800
};

class CardGame {
private:
	string cardmode;
	int startlevel;
	RectangleShape box;
	
public:
	CardGame(const std::string& mode, const std::string& level)
		: cardmode(mode)
	{
		if (level == "쉬움") startlevel = 1;
		else if (level == "보통") startlevel = 5;
		else startlevel = 9;

		box.setSize({ 900.f, 600.f });
		box.setFillColor(Color::White);
		box.setOutlineThickness(2.f);
		box.setOutlineColor(Color::Black);
		box.setPosition(App::WIDTH / 2 - box.getSize().x / 2, App::HEIGHT / 2 - 250.f);



	}
	void draw(RenderWindow& window) {
		window.draw(box);
	}
};

void showGame(const std::string& mode, const std::string& level)
{
	RenderWindow window(VideoMode(App::WIDTH, App::HEIGHT), "Card Game");
	window.setFramerateLimit(60);

	CardGame cardGame = CardGame(mode, level);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
				showHome();
				return;
			}
			// event
		}
		window.clear(Color::White);
		cardGame.draw(window);
		window.display();
	}
}