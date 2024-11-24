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

// class 선언

void showGame()
{
	RenderWindow window(VideoMode(App::WIDTH, App::HEIGHT), "Card Game");
	window.setFramerateLimit(60);

	// class 생성

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
		// class draw
		window.display();
	}
}