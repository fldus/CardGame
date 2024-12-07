﻿#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "home.h"
#include "option.h"
#include "game.h"

using namespace sf;
using namespace std;

enum App
{
	WIDTH = 1100, HEIGHT = 800
};
enum Box
{
	Width = 900, Height = 600
};

class Card {
private:
	RectangleShape shape;
	Texture front;
	vector<string> baseimgs = { "card/01.png", "card/14.png", "card/02.png", 
		"card/15.png", "card/03.png", "card/16.png", "card/04.png", "card/17.png", 
		"card/05.png", "card/18.png", "card/06.png", "card/19.png", "card/07.png", 
		"card/20.png", "card/08.png", "card/21.png", "card/09.png", "card/22.png", 
		"card/10.png", "card/23.png", "card/11.png", "card/24.png", "card/12.png", 
		"card/25.png", "card/13.png", "card/26.png" };
	vector<string> itemimgs = { "card/heart.png", "card/31.png", "card/44.png", 
		"card/bomb.png", "card/32.png", "card/45.png", "card/33.png", "card/46.png", 
		"card/34.png", "card/47.png", "card/35.png", "card/48.png", "card/36.png", 
		"card/49.png", "card/37.png", "card/50.png", "card/38.png", "card/51.png", 
		"card/39.png", "card/52.png", "card/40.png", "card/53.png", "card/41.png", 
		"card/54.png", "card/42.png", "card/55.png", "card/43.png", "card/56.png" };
public:
	bool isFlipped;
	bool isMatched;
	int card_img;

	Card(float x, float y, float width, float height, int i, bool isItem)
		: isFlipped(false), isMatched(false), card_img(i)
	{
		vector<string>& imgs = isItem ? itemimgs : baseimgs;

		if (!front.loadFromFile(imgs[i]))
			cout << "Falid to load texture:" << imgs[i] << endl;

		shape.setSize({ width, height });
		shape.setFillColor(Color::White);	// 뒤집힌 상태
		shape.setOutlineThickness(2.5f);
		shape.setOutlineColor(Color::Black);
		shape.setPosition(x, y);
		shape.setTexture(nullptr);
	}
	void draw(RenderWindow& window) {
		window.draw(shape);
	}
	void flip() {
		isFlipped = !isFlipped;
		if (isFlipped) {
			shape.setTexture(&front);
		}
		else {
			shape.setTexture(nullptr);
		}
	}
	bool contains(Vector2f point) {
		return shape.getGlobalBounds().contains(point);
	}
};

class CardGame {
private:
	int cnt = 0;
	int level;
	int startlevel;
	bool isItem;
	bool waiting = false;
	RectangleShape timerBox;
	RectangleShape timer;
	RectangleShape box;
	Text text;
	Font font;
	vector<Card> cards;
	vector<Card*> flippedCards;
	Clock delayClock;
	Clock clock;
	float remindTime = 30.f;	// 제한 시간
	RenderWindow& window;
public:
	CardGame(bool Item, int slevel, RenderWindow& renderWindow)
		: isItem(Item), level(slevel), window(renderWindow)
	{
		startlevel = level;

		if (!font.loadFromFile("HANDotum.ttf")) {
			throw runtime_error("Font loading failed");
		}

		box.setSize({ Box::Width, Box::Height });
		box.setFillColor(Color::White);
		box.setOutlineThickness(2.f);
		box.setOutlineColor(Color::Black);
		box.setPosition(App::WIDTH / 2 - Box::Width / 2, App::HEIGHT / 2 - 250.f);

		text.setFont(font);
		text.setCharacterSize(50);
		text.setFillColor(Color::Black);
		text.setString(to_wstring(L" Lv." + to_string(level)));
		text.setPosition(box.getPosition().x, 40.f);

		timerBox.setSize({ box.getSize().x - text.getGlobalBounds().width - 30.f, 50.f});
		timerBox.setFillColor(Color::White);
		timerBox.setOutlineThickness(2.f);
		timerBox.setOutlineColor(Color::Black);
		timerBox.setPosition(text.getPosition().x + text.getGlobalBounds().width + 30.f, 50.f);

		timer.setSize({ timerBox.getSize().x, timerBox.getSize().y });
		timer.setFillColor(Color(102, 204, 102));
		timer.setPosition(timerBox.getPosition().x, timerBox.getPosition().y);

		setlevel(level);
	}
	void draw() {
		remindTime -= clock.restart().asSeconds();
		float timerWidth = timerBox.getSize().x * (remindTime / 30.f);
		
		if (timerWidth > timerBox.getSize().x) timerWidth = timerBox.getSize().x;
		if (remindTime < 0) {
			GameOver(false);
		}

		timer.setSize({ timerWidth, timer.getSize().y });
		if (remindTime > 15) timer.setFillColor(Color(102, 204, 102));
		if (remindTime <= 15) timer.setFillColor(Color(255, 165, 0));
		if (remindTime < 5) timer.setFillColor(Color::Red);

		window.draw(text);
		window.draw(timerBox);
		window.draw(timer);
		window.draw(box);
		for (auto& card : cards) {
			card.draw(window);
		}

		if (waiting && delayClock.getElapsedTime().asMilliseconds() >= 500) {
			flippedCards[0]->flip();
			flippedCards[1]->flip();
			flippedCards.clear();
			waiting = false;
		}
	}
	int cardCount(int level) {
		if (level <= 1) return 4;
		if (level == 2) return 6;
		if (level == 3) return 8;
		if (level == 4) return 12;
		return 4 + 2 * (level - 1);
	}
	void setlevel(int level) {
		text.setString(L" Lv." + to_string(level));
		remindTime = 30.f;
		timer.setSize({ timerBox.getSize().x, timerBox.getSize().y });
		clock.restart();
		cards.clear();

		int cardNumber = cardCount(level);
		float cardWidth = 80.f;
		float cardHeight = 130.f;

		int colNumber, rowNumber;
		for (int j = 1; j <= sqrt(cardNumber); ++j) {
			if (cardNumber % j == 0) {
				rowNumber = j;
				colNumber = cardNumber / j;
			}
		}
		if (rowNumber == 0 || colNumber == 0) {
			rowNumber = ceil(sqrt(cardNumber));
			colNumber = ceil(float(cardNumber) / rowNumber);
		}

		float colpadding = (Box::Width - colNumber * cardWidth) / (colNumber + 1.f);
		float rowpadding = (Box::Height - rowNumber * cardHeight) / (rowNumber + 1.f);

		Vector2f boxPos = box.getPosition();
		float startx = boxPos.x + colpadding;
		float starty = boxPos.y + rowpadding;

		vector<int> cardImages;
		for (int i = 0; i < cardNumber / 2; ++i) {
			cardImages.push_back(i);
			cardImages.push_back(i);
		}
		shuffle(cardImages.begin(), cardImages.end(), default_random_engine(random_device{}()));
		for (int i = 0; i < cardNumber; ++i) {
			float x = startx + (i % colNumber) * (cardWidth + colpadding);
			float y = starty + (i / colNumber) * (cardHeight + rowpadding);
			cards.emplace_back(x, y, cardWidth, cardHeight, cardImages[i], isItem);
		}

		showAllCards();
		clock.restart();
	}
	void showAllCards() {
		for (auto& card : cards)
			card.flip();
		window.clear(Color::White);
		draw();
		window.display();
		sf::sleep(sf::seconds(1));
		for (auto& card : cards)
			card.flip();
	}
	void cardClick(Vector2f mousePos) {
		if (waiting) return;

		if (remindTime < 0) {
			GameOver(false);
			return;
		}

		for (auto& card : cards) {
			if (card.contains(mousePos) && !card.isMatched && !card.isFlipped) {
				card.flip();
				flippedCards.push_back(&card);

				if (flippedCards.size() == 2) {
					waiting = true;
					if (flippedCards[0]->card_img == flippedCards[1]->card_img) {
						if (isItem && flippedCards[0]->card_img == 0) {
							remindTime += 5;
						}
						else if (isItem && flippedCards[0]->card_img == 3) {
							remindTime -= 5;
						}	
						flippedCards[0]->isMatched = true;
						flippedCards[1]->isMatched = true;
						flippedCards.clear();
						cnt += 2;
						waiting = false;
					}
					else {
						delayClock.restart();
					}
				}
				if(remindTime > 0)
					cardCheck();
				return;
			}
		}
	}
	void cardCheck() {
		bool allMatched = true;
		for (auto& card : cards) {
			if (!card.isMatched) {
				allMatched = false;
				break;
			}
		}
		if (allMatched) {
			GameOver(true);
		}
	}
	void nextLevel() {
		++level;
		setlevel(level);
	}
	void GameOver(bool isClear) {
		RenderWindow gameOverWindow(VideoMode(600, 400), isClear ? "Game Clear" : "Game Over");
		gameOverWindow.setFramerateLimit(60);

		Text message;
		message.setFont(font);
		message.setCharacterSize(40);
		message.setFillColor(Color::Black);
		message.setString(isClear ? L"Game Clear!" : L"Game Over..");
		message.setPosition(gameOverWindow.getSize().x / 2.f - message.getLocalBounds().width / 2.f, 100.f);

		Button next(80.f, 250.f, isClear? u8"다음 레벨" : u8"다시하기", Color(102, 204, 102));
		Button home(320.f, 250.f, u8"취소", Color(192, 192, 192));

		while (gameOverWindow.isOpen()) {
			Event event;
			while (gameOverWindow.pollEvent(event)) {
				if (event.type == Event::Closed) {
					gameOverWindow.close();
					window.close();
					showHome();
					return;
				}
				if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
					Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
					if (next.isClicked(mousePos)) {
						gameOverWindow.close();
						updateScore(isItem, startlevel, level, cnt);
						if (isClear)
							nextLevel();
						else {
							window.close();
							showGame(isItem, startlevel);
						}
						return;
					}else if (home.isClicked(mousePos)) {
						gameOverWindow.close();
						updateScore(isItem, startlevel, level, cnt);
						window.close();
						showHome();
						return ;
					}
				}
			}
			gameOverWindow.clear(Color::White);
			gameOverWindow.draw(message);
			next.draw(gameOverWindow);
			home.draw(gameOverWindow);
			gameOverWindow.display();
		}
	}
	void updateScore(bool isItem, int startlevel, int level, int cnt) {
		if (isItem) {
			if (startlevel == 1) ie.update(level, cnt);
			else if (startlevel == 5) ic.update(level, cnt);
			else ih.update(level, cnt);
		}
		else {
			if (startlevel == 1) be.update(level, cnt);
			else if (startlevel == 5) bc.update(level, cnt);
			else bh.update(level, cnt);
		}
	}
};

void showGame(bool isItem, int startlevel)
{
	RenderWindow window(VideoMode(App::WIDTH, App::HEIGHT), "Card Game");
	window.setFramerateLimit(60);

	CardGame cardGame = CardGame(isItem, startlevel, window);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
				showHome();
				return;
			}
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
				cardGame.cardClick(mousePos);
			}
		}
		window.clear(Color::White);
		cardGame.draw();
		window.display();
	}
}