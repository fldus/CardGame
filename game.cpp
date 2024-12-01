#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "home.h"
#include "option.h"
#include "game.h"

using namespace sf;
using namespace std;

// Render window의 너비와 높이를 정의
enum App
{
	WIDTH = 1100, HEIGHT = 800
};
// 카드 배치 박스의 너비와 높이를 정의
enum Box
{
	Width = 900, Height = 600
};

// 카드 클래스 정의
class Card {
private:
	RectangleShape shape;	// 카드 모양
	Texture front;	// 카드 앞면 이미지
	// 기본 카드 이미지 경로
	vector<string> baseimgs = { "card/01.png", "card/14.png", "card/02.png", 
		"card/15.png", "card/03.png", "card/16.png", "card/04.png", "card/17.png", 
		"card/05.png", "card/18.png", "card/06.png", "card/19.png", "card/07.png", 
		"card/20.png", "card/08.png", "card/21.png", "card/09.png", "card/22.png", 
		"card/10.png", "card/23.png", "card/11.png", "card/24.png", "card/12.png", 
		"card/25.png", "card/13.png", "card/26.png" };
	// 아이템 카드 이미지 경로
	vector<string> itemimgs = { "card/heart.png", "card/31.png", "card/44.png", 
		"card/bomb.png", "card/32.png", "card/45.png", "card/33.png", "card/46.png", 
		"card/34.png", "card/47.png", "card/35.png", "card/48.png", "card/36.png", 
		"card/49.png", "card/37.png", "card/50.png", "card/38.png", "card/51.png", 
		"card/39.png", "card/52.png", "card/40.png", "card/53.png", "card/41.png", 
		"card/54.png", "card/42.png", "card/55.png", "card/43.png", "card/56.png" };
public:
	bool isFlipped;	// 카드가 뒤집혔는지 여부
	bool isMatched;	// 카드가 매칭되었는지 여부
	int card_img;	// 카드 이미지 경로의 인덱스

	// 카드 클래스 생성자
	Card(float x, float y, float width, float height, int i, bool isItem)
		: isFlipped(false), isMatched(false), card_img(i)
	{
		// 카드 모드에 따른 이미지 경로 설정
		vector<string>& imgs = isItem ? itemimgs : baseimgs;

		// 폰트 로그 + 예외 처리
		if (!front.loadFromFile(imgs[i]))
			cout << "Falid to load texture:" << imgs[i] << endl;

		// 카드 모양 (뒷면 = 흰색, 앞면 = 이미지)
		shape.setSize({ width, height });
		shape.setFillColor(Color::White);	// 뒤집힌 상태
		shape.setOutlineThickness(2.5f);
		shape.setOutlineColor(Color::Black);
		shape.setPosition(x, y);
		shape.setTexture(nullptr);	// 텍스쳐 제거
	}
	// 카드 그리는 함수
	void draw(RenderWindow& window) {
		window.draw(shape);	// 카드 모양 그리기
	}
	// 카드 뒤집는 함수
	void flip() {
		isFlipped = !isFlipped;	// 뒤집기 상태 변경
		if (isFlipped) {
			// 앞면이면 이미지 보이기
			shape.setTexture(&front);
		}
		else {
			// 뒷면이면 텍스쳐 제거
			shape.setTexture(nullptr);
		}
	}
	// 각 카드 클릭 여부 확인 함수
	bool contains(Vector2f point) {
		// 카드가 클릭되었는지 확인
		return shape.getGlobalBounds().contains(point);
	}
};

// 카드 게임 클래스 정의
class CardGame {
private:
	int cnt = 0;	// 점수(뒤집은 카드 수)
	int level;	// 현재 레벨
	int startlevel;	// 시작 레벨(난이도)
	bool isItem;	// true = 아이템 카드 / false = 기본 카드
	bool waiting = false;	// 카드 뒤집기 대기 상태
	RectangleShape timerBox;	// 타이머 박스(테두리)
	RectangleShape timer;	// 타이머
	RectangleShape box;	// 카드 배치되는 박스
	Text text;	// 레벨 텍스트
	Font font;	
	vector<Card> cards;	// 카드 백터
	vector<Card*> flippedCards;	// 뒤집힌 카드(2개) 저장
	Clock delayClock;	// 카드 뒤집기 지연 타이머
	Clock clock;	// 전체 게임 타이머
	float remindTime = 30.f;	// 제한 시간
	RenderWindow& window;
public:
	// 카드 게임 생성자
	CardGame(bool Item, int slevel, RenderWindow& renderWindow)
		: isItem(Item), level(slevel), window(renderWindow)
	{
		//시작 레벨 설정
		startlevel = level;

		// 폰트 로드 + 예외 처리
		if (!font.loadFromFile("HANDotum.ttf")) {
			throw runtime_error("Font loading failed");
		}

		// 카드 배치 박스 설정
		box.setSize({ Box::Width, Box::Height });
		box.setFillColor(Color::White);
		box.setOutlineThickness(2.f);
		box.setOutlineColor(Color::Black);
		box.setPosition(App::WIDTH / 2 - Box::Width / 2, App::HEIGHT / 2 - 250.f);

		// 현재 레벨 표시 텍스트 설정
		text.setFont(font);
		text.setCharacterSize(50);
		text.setFillColor(Color::Black);
		text.setString(to_wstring(L" Lv." + to_string(level)));
		text.setPosition(box.getPosition().x, 40.f);

		// 타이머 박스 설정
		timerBox.setSize({ box.getSize().x - text.getGlobalBounds().width - 30.f, 50.f});
		timerBox.setFillColor(Color::White);
		timerBox.setOutlineThickness(2.f);
		timerBox.setOutlineColor(Color::Black);
		timerBox.setPosition(text.getPosition().x + text.getGlobalBounds().width + 30.f, 50.f);

		// 타이머 설정
		timer.setSize({ timerBox.getSize().x, timerBox.getSize().y });
		timer.setFillColor(Color(102, 204, 102));
		timer.setPosition(timerBox.getPosition().x, timerBox.getPosition().y);

		// 레벨 설정 함수 호출
		setlevel(level);
	}
	// 카드 게임 그리는 함수
	void draw() {
		// 남은 시간 업데이트
		remindTime -= clock.restart().asSeconds();
		// 타이머 너비 계산
		float timerWidth = timerBox.getSize().x * (remindTime / 30.f);
		// 타이머가 타이머 박스 초과하지 않도록 제한
		if (timerWidth > timerBox.getSize().x) timerWidth = timerBox.getSize().x;
		// 제한 시간이 끝나면 게임 종료
		if (remindTime < 0) {
			GameOver(false);
		}
		// 타이머 크기 설정
		timer.setSize({ timerWidth, timer.getSize().y });
		// 타이머 남은 시간에 따라 색상 설정
		if (remindTime > 15) timer.setFillColor(Color(102, 204, 102));	// 초록
		if (remindTime <= 15) timer.setFillColor(Color(255, 165, 0));	// 주황
		if (remindTime < 5) timer.setFillColor(Color::Red);	// 빨강

		// 화면에 그리기
		window.draw(text);	// 레벨 
		window.draw(timerBox);	// 타이머 박스
		window.draw(timer);	// 타이머
		window.draw(box);	// 카드 배치 박스
		for (auto& card : cards) {	// 모든 카드
			card.draw(window);
		}
		
		// 카드 두 장이 뒤집힌 경우 처리
		if (waiting && delayClock.getElapsedTime().asMilliseconds() >= 500) {
			// 다시 뒤집기
			flippedCards[0]->flip();
			flippedCards[1]->flip();
			// 뒤집힌 카드 초기화
			flippedCards.clear();
			// 대기 상태 헤제
			waiting = false;
		}
	}
	// 레벨에 따른 카드 개수를 구하는 함수
	int cardCount(int level) {
		if (level <= 1) return 4;
		if (level == 2) return 6;
		if (level == 3) return 8;
		if (level == 4) return 12;
		return 4 + 2 * (level - 1);
	}
	// 레벨 설정 함수
	void setlevel(int level) {
		text.setString(L" Lv." + to_string(level));	// 레벨 텍스트 초기화
		remindTime = 30.f;	// 제한 시간 초기화
		timer.setSize({ timerBox.getSize().x, timerBox.getSize().y });	// 타이머 크기 초기화
		clock.restart();	// 타이머 초기화
		cards.clear();	// 카드 목록 초기화

		int cardNumber = cardCount(level);	// 카드 수 구하기
		float cardWidth = 80.f;	// 카드 너비
		float cardHeight = 130.f;	// 카드 높이

		// 카드 배치 열과 행 수 계산
		int colNumber, rowNumber;
		for (int j = 1; j <= sqrt(cardNumber); ++j) {
			if (cardNumber % j == 0) {
				rowNumber = j;	// 행 수
				colNumber = cardNumber / j;	// 열 수
			}
		}
		// 행 또는 열이 0일 경우
		if (rowNumber == 0 || colNumber == 0) {
			rowNumber = ceil(sqrt(cardNumber));
			colNumber = ceil(float(cardNumber) / rowNumber);
		}

		// 행, 열 패딩 계산
		float colpadding = (Box::Width - colNumber * cardWidth) / (colNumber + 1.f);
		float rowpadding = (Box::Height - rowNumber * cardHeight) / (rowNumber + 1.f);

		// 박스 위치 가져오기
		Vector2f boxPos = box.getPosition();
		float startx = boxPos.x + colpadding;	// 시작 x 위치
		float starty = boxPos.y + rowpadding;	// 시작 y 위치

		// 카드 이미지 인덱스 저장
		vector<int> cardImages;
		for (int i = 0; i < cardNumber / 2; ++i) {
			cardImages.push_back(i);	// 카드 이미지 2번 추가
			cardImages.push_back(i);
		}
		// 카드 이미지 무작위로 섞기
		shuffle(cardImages.begin(), cardImages.end(), default_random_engine(random_device{}()));
		for (int i = 0; i < cardNumber; ++i) {
			// 카드 위치 계산
			float x = startx + (i % colNumber) * (cardWidth + colpadding);
			float y = starty + (i / colNumber) * (cardHeight + rowpadding);
			// 카드 추기
			cards.emplace_back(x, y, cardWidth, cardHeight, cardImages[i], isItem);
		}
		// 모든 카드 보여주는 함수 호출
		showAllCards();
		// 타이머 시작
		clock.restart();
	}
	// 모든 카드 보여주는 함수
	void showAllCards() {
		// 카드 전부 뒤집기
		for (auto& card : cards)
			card.flip();
		// 화면 초기화 + 그리기 + 출력
		window.clear(Color::White);
		draw();
		window.display();
		// 1초 대기
		sf::sleep(sf::seconds(1));
		// 다시 카드 뒤집기
		for (auto& card : cards)
			card.flip();
	}
	// 카드 처리 함수
	void cardClick(Vector2f mousePos) {
		// 대기 중에 카드 처리 x
		if (waiting) return;

		// 제한 시간 종료 = 게임 오버
		if (remindTime < 0) {
			GameOver(false);
			return;
		}

		// 모든 카드 클릭 이벤트 처리
		for (auto& card : cards) {
			// 카드를 클릭하고, 매칭되지 않았고, 뒤집히지 않았을 때
			if (card.contains(mousePos) && !card.isMatched && !card.isFlipped) {
				card.flip();// 카드 뒤집기
				// 뒤집힌 카드 목록에 추가
				flippedCards.push_back(&card);

				// 카드 2장이 뒤집혔을 때
				if (flippedCards.size() == 2) {
					waiting = true;	// 대기 상태로 설정
					if (flippedCards[0]->card_img == flippedCards[1]->card_img) {
						// 아이템 카드가 heart.png일 때 
						if (isItem && flippedCards[0]->card_img == 0) {
							remindTime += 5;	// 제한 시간 5초 추가
						}
						// 아이템 카드가 bomb.png일 때
						else if (isItem && flippedCards[0]->card_img == 3) {
							remindTime -= 5;	// 제한 시간 5초 감소
						}
						// 카드 매칭 처리
						flippedCards[0]->isMatched = true;
						flippedCards[1]->isMatched = true;
						flippedCards.clear();	// 뒤집힌 카드 목록 초기화
						cnt += 2;	// 뒤집은 카드 수 (점수) 증가
						waiting = false;	// 대기 상태 헤제
					}
					// 카드가 일치하지 않을 때
					else {
						// 카드 뒤집기 대기 타이머 초기화
						delayClock.restart();
					}
				}
				// 제한 시간이 남아있으면 카드 전부 뒤집혔는지 확인
				if(remindTime > 0)
					cardCheck();

				// 클릭 처리 종료
				return;
			}
		}
	}
	// 모든 카드가 매칭되었는지 확인하는 함수
	void cardCheck() {
		bool allMatched = true;	// 모든 카드 매칭 여부 초기화
		for (auto& card : cards) {
			// 매칭 되지 않은 카드가 있으면 
			if (!card.isMatched) {
				allMatched = false; // 매칭 상태 업데이트
				break;	// 반복 종료
			}
		}
		// 모든 카드 매칭된 경우 게임 종료(게임 클리어)
		if (allMatched) {
			GameOver(true);
		}
	}
	// 다음 레벨로 넘어가는 함수
	void nextLevel() {
		++level;
		setlevel(level);
	}
	// 게임 종료를 처리하는 함수
	void GameOver(bool isClear) {
		// 게임 종료 창 띄우기
		RenderWindow gameOverWindow(VideoMode(600, 400), isClear ? "Game Clear" : "Game Over");
		gameOverWindow.setFramerateLimit(60);
		
		// 메시지 텍스트 (Game Clear, Game Over) 설정
		Text message;
		message.setFont(font);
		message.setCharacterSize(40);
		message.setFillColor(Color::Black);
		message.setString(isClear ? L"Game Clear!" : L"Game Over..");
		message.setPosition(gameOverWindow.getSize().x / 2.f - message.getLocalBounds().width / 2.f, 100.f);

		// 다음 레벨 / 다시하기, 취소 버튼 생성
		Button next(80.f, 250.f, isClear? u8"다음 레벨" : u8"다시하기", Color(102, 204, 102));
		Button home(320.f, 250.f, u8"취소", Color(192, 192, 192));

		// 게임 오버 창이 열려 있는 동안
		while (gameOverWindow.isOpen()) {
			Event event;
			while (gameOverWindow.pollEvent(event)) {
				// 창 닫기 이벤트 처리
				if (event.type == Event::Closed) {
					// 창 닫기
					gameOverWindow.close();
					window.close();
					showHome();	// 홈화면으로 이동
					return;
				}
				// 마우스 클릭 이벤트 처리
				if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
					// 마우스 위치
					Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
					// 다음 레벨 / 다시하기 버튼 클릭했을 때
					if (next.isClicked(mousePos)) {
						gameOverWindow.close();	// 창 닫기
						// 점수 업데이트
						updateScore(isItem, startlevel, level, cnt);
						if (isClear)
							// 다음 레벨로 이동
							nextLevel();
						else {
							// 창 닫고, 다시 게임 시작
							window.close();
							showGame(isItem, startlevel);
						}
						return;
					}// 취소 버튼 클릭했을 때
					else if (home.isClicked(mousePos)) {
						// 창 닫기, 점수 업데이트, 홈 화면으로 이동
						gameOverWindow.close();
						updateScore(isItem, startlevel, level, cnt);
						window.close();
						showHome();
						return ;
					}
				}
			}
			// 게임 오버 화면 초기화
			gameOverWindow.clear(Color::White);
			// 메시지, 버튼 그리기
			gameOverWindow.draw(message);	
			next.draw(gameOverWindow);
			home.draw(gameOverWindow);
			// 화면에 출력
			gameOverWindow.display();
		}
	}
	// 점수 업데이트 함수
	void updateScore(bool isItem, int startlevel, int level, int cnt) {
		if (isItem) {	// 아이템 카드일 때 
			if (startlevel == 1) ie.update(level, cnt);
			else if (startlevel == 5) ic.update(level, cnt);
			else ih.update(level, cnt);
		}
		else {	// 기본 카드일 때
			if (startlevel == 1) be.update(level, cnt);
			else if (startlevel == 5) bc.update(level, cnt);
			else bh.update(level, cnt);
		}
	}
};

// 게임 화면 보여주는 함수
void showGame(bool isItem, int startlevel)
{
	// 게임 창 생성
	RenderWindow window(VideoMode(App::WIDTH, App::HEIGHT), "Card Game");
	window.setFramerateLimit(60);

	// 카드 게임 객체 생성
	CardGame cardGame = CardGame(isItem, startlevel, window);

	// 게임 창이 열려있을 동안
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			// 창 닫기 이벤트 처리
			if (event.type == Event::Closed) {
				window.close();	// 창 닫기
				showHome();	// 홈 화면 이동
				return;
			}
			// 마우스 클릭 이벤트 처리
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				// 마우스 위치
				Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
				// 카드 클릭 처리
				cardGame.cardClick(mousePos);
			}
		}
		// 화면 초기화, 그리기, 출력
		window.clear(Color::White);
		cardGame.draw();
		window.display();
	}
}