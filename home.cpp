#include <SFML/Graphics.hpp>
#include <string>
#include "home.h"
#include "option.h"

using namespace sf;
using namespace std;

// Render window 창 너비와 높이
enum App	
{
	WIDTH = 900, HEIGHT = 700
};

// Record(게임 최고 기록 저장하는 클래스) 생성자
Record::Record(float x, float y, string mode, string slevel)
{	// 폰트 로드 + 예외 처리
	if (!font.loadFromFile("HANDotum.ttf")) {
		throw runtime_error("Font loading failed");
	}

	// 카드 모드 텍스트 설정 (기본 카드 / 아이템 카드)
	cardMode.setFont(font);
	cardMode.setCharacterSize(30);
	cardMode.setString(to_wstring(mode));
	cardMode.setFillColor(Color::Black);
	cardMode.setPosition(x, y);
	cardMode.setOrigin(	
		cardMode.getGlobalBounds().width / 2.f,
		cardMode.getGlobalBounds().height / 2.f);

	// 난이도 텍스트 설정 (쉬움 / 보통 / 어려움)
	startlevel.setFont(font);
	startlevel.setCharacterSize(30);
	startlevel.setString(to_wstring(slevel));
	startlevel.setFillColor(Color::Black);
	startlevel.setPosition(x+170.f, y);
	startlevel.setOrigin(
		startlevel.getGlobalBounds().width / 2.f, 
		startlevel.getGlobalBounds().height / 2.f);

	// 최고 기록 레벨, 점수(카드 뒤집은 수) 텍스트 설정
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
	window.draw(cardMode);	// 카드 모드
	window.draw(startlevel);	// 난이도
	window.draw(text);	// 최고 기록 레벨, 점수
}
void Record::update(int l, int s) {
	// 레벨 기준으로 업데이트
	if (level < l) {
		level = l; score = s;
	}
	else if (level == l && score < s) {
		score = s;	// 점수 업그레이드
	}
	// 업데이트된 텍스트 화면에 반영
	text.setString("Lv." + to_string(level) + "        " + to_string(score));
}
// 게임 최고 기록 (제목, 목록) + 게임 시작 버튼 정의하는 클래스
class Start {
private:
	RectangleShape box;	// 게임 최고 기록
	RectangleShape shape;	// 게임 시작 버튼
	const float StartButtonWidth = 600.f;
	const float StartButtonHeight = 100.f;
	Font font;
	Text title;	// 제목 : 게임 최고 기록
	Text label;	// 목록 : 카드 모드, 난이도, 레벨, 점수
	Text start;	// 시작 버튼 텍스트
public:
	Start(float x, float y) {
		// 폰트 로드 + 예외 처리
		if (!font.loadFromFile("HANDotum.ttf")) {
			throw runtime_error("Font loading failed");
		}
		// 게임 시작 버튼 설정
		shape.setSize({ StartButtonWidth, StartButtonHeight});
		shape.setFillColor(Color(255, 238, 173));	// 연한 노란색
		shape.setPosition(x, y+100.f);
		shape.setOrigin(StartButtonWidth / 2.f, StartButtonHeight / 2.f);

		// 게임 최고 기록 배경 설정
		box.setSize({ StartButtonWidth, 450.f });
		box.setFillColor(Color(217, 217, 217));	// 회색
		box.setOutlineThickness(1.f);
		box.setOutlineColor(Color::Black);
		box.setPosition(x, y - 200.f);
		box.setOrigin(box.getSize().x / 2.f, box.getSize().y / 2.f);

		// 제목 텍스트 설정 : 게임 최고 기록
		title.setFont(font);
		title.setCharacterSize(45);
		title.setString(L"게임 최고 기록");
		title.setFillColor(Color::Black);
		title.setPosition(
			box.getPosition().x - title.getGlobalBounds().width / 2.f,
			box.getPosition().y - 210.f);

		// 목록 텍스트 설정 : 카드 모드, 난이도, 레벨, 점수
		label.setFont(font);
		label.setCharacterSize(35);
		label.setString(L"카드 모드     난이도     레벨     점수");
		label.setFillColor(Color::Black);
		label.setPosition(
			box.getPosition().x - label.getGlobalBounds().width / 2.f,
			box.getPosition().y - 140.f);

		// 게임 시작 버튼 텍스트 설정
		start.setFont(font);
		start.setCharacterSize(65);
		start.setString(L"게임 시작");
		start.setFillColor(Color::Black);
		start.setOrigin(start.getGlobalBounds().width / 2.f, start.getGlobalBounds().height / 1.4f);
		start.setPosition(shape.getPosition().x, shape.getPosition().y);
	}
	// 게임 최고 기록 + 시작 버튼 그리기
	void draw(RenderWindow& window) {
		// 게임 최고 기록
		window.draw(box);
		window.draw(title);	
		window.draw(label);
		// 게임 시작 버튼
		window.draw(shape);	
		window.draw(start);
	}
	// 시작 버튼 클릭 여부를 확인하는 메서드
	bool isClicked(Vector2f mousePos) {
		return shape.getGlobalBounds().contains(mousePos);
	}
};

// 게임 저장 기록 (제목, 목록 별로 생성)
Record be(255.f, 190.f, u8"기본 카드", u8"쉬움");
Record bc(255.f, 240.f, u8"기본 카드", u8"보통");
Record bh(255.f, 290.f, u8"기본 카드", u8"어려움");
Record ie(255.f, 340.f, u8"아이템 카드", u8"쉬움");
Record ic(255.f, 390.f, u8"아이템 카드", u8"보통");
Record ih(255.f, 440.f, u8"아이템 카드", u8"어려움");

void showHome()
{
	// 홈 화면 창 생성
	RenderWindow window(VideoMode(App::WIDTH, App::HEIGHT), "Card Game");
	// 초당 프레임 조절(컴퓨터 성능과 상관없이 일정한 속도)
	window.setFramerateLimit(60);

	// 시작 버튼 위치 설정
	Start sbutton(App::WIDTH / 2, (App::HEIGHT / 3) * 2);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			// 창 닫기 이벤트 처리
			if (event.type == Event::Closed)
				window.close();
			// 마우스 클릭 이벤트 처리
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				Vector2f mousePos(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
				if (sbutton.isClicked(mousePos)) {
					window.close();	// 현재 창 닫기
					showOption();	// 옵션 화면으로 이동
					return;
				}
			}
		}
		// 화면 초기화
		window.clear(Color::White);
		// 시작 버튼 그리기
		sbutton.draw(window);
		// 게임 최고 기록 그리기
		be.draw(window);	
		bc.draw(window);
		bh.draw(window);
		ie.draw(window);
		ic.draw(window);
		ih.draw(window);
		// 화면에 출력
		window.display();
	}
}