#include <SFML/Graphics.hpp>
#include <iostream>
#include <locale>
#include <codecvt>
#include <vector>
#include <string>
#include "option.h"
#include "home.h"
#include "game.h"

using namespace sf;
using namespace std;

// Render window 화면 너비와 높이
enum App
{
	WIDTH = 600, HEIGHT = 500
};

// UTF-8문자열을 UTF-16으로 변환하는 함수
std::wstring to_wstring(const std::string& str) {
	try{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(str);	// 변환 
	}
	catch (const std::range_error& e) {	// 오류 발생 시 예외 처리
		throw runtime_error("String conversion failed: " + string(e.what()));
	}
}

// 드롭다운 메뉴 클래스
class Dropdown {
private:
	RectangleShape box;	// 선택된 텍스트가 보이는 박스
	Text Title;	// 제목 텍스트 (카드모드, 난이도) 
	Text label;	// 선택된 텍스트 표시
	Font font;
	vector<Text> options;	// 옵션(선택지) 텍스트
	vector<RectangleShape> optionBoxes;	// 옵션(선택지) 사각형 배경
	bool isOpen = false;	// 드랍다운 열림 여부 확인
	int selectedIndex = 0;	// 첫 번째 옵션 = 기본 선택지
public:
	// Dropdown 생성자
	Dropdown(float x, float y, string title, const vector<string>& optionText) {
		// 옵션이 비어있을 경우 예외 처리
		if (optionText.empty())
			throw invalid_argument("Option list cannot be empty");

		// font load + 예외 처리
		if (!font.loadFromFile("HANDotum.ttf")) {
			throw runtime_error("Font loading failed");
		}

		// 메뉴 제목 설정
		Title.setFont(font);
		Title.setCharacterSize(25);
		Title.setFillColor(Color::Black);
		Title.setString(to_wstring(title));
		Title.setPosition(x + 70.f, y - 50.f);

		// 드롭다운 박스 설정(선택된 내용 보이는 박스)
		box.setSize({ 200.f, 50.f });
		box.setFillColor(Color::White);
		box.setOutlineThickness(2.f);
		box.setOutlineColor(Color::Black);
		box.setPosition(x, y);
		
		// 선택된 옵션 텍스트 표시
		label.setFont(font);
		label.setCharacterSize(25);
		label.setFillColor(Color::Black);
		label.setString(to_wstring(optionText[0]));	// 첫 번째 옵션으로 초기화
		label.setPosition(
			box.getPosition().x + (box.getSize().x - label.getLocalBounds().width) / 2.f,
			box.getPosition().y + (box.getSize().y - label.getLocalBounds().height) / 2.f - 5.f
		);

		// 드롭다운 옵션(선택지) 생성
		for (size_t i = 0; i < optionText.size(); ++i) {
			RectangleShape optionBox;	// 각 옵션의 박스
			optionBox.setSize({ box.getSize().x, 30.f });
			optionBox.setFillColor(Color::White);
			optionBox.setOutlineThickness(2.f);
			optionBox.setOutlineColor(Color::Black);
			optionBox.setPosition(box.getPosition().x, box.getPosition().y + 60.f + i * 30.f);
			optionBoxes.push_back(optionBox);	//  옵션 박스 추가
				
			// 옵션 텍스트 설정
			Text option;	// 옵션 텍스트 객체
			option.setFont(font);
			option.setCharacterSize(20);
			option.setString(to_wstring(optionText[i]));
			option.setFillColor(Color::Black);
			option.setPosition(
				optionBox.getPosition().x + (optionBox.getSize().x - option.getLocalBounds().width) / 2.f - 5.f,
				optionBox.getPosition().y + (optionBox.getSize().y - option.getLocalBounds().height) / 2.f - 5.f
			);			
			options.push_back(option);	// 옵션 텍스트 추가
		}
	}
	// 드롭다운 그리는 함수
	void draw(RenderWindow& window) {
		window.draw(Title);	// 제목 
		window.draw(box);	// 선택된 드롭다운 박스
		window.draw(label);	// 선택된 드롭다운 옵션

		// window가 열려있을 때 모든 옵션 그리기
		if (isOpen) {
			for (size_t i = 0; i < options.size(); ++i) {
				window.draw(optionBoxes[i]);	// 각 옵션 박스
				window.draw(options[i]);	// 각 옵션 텍스트
			}
		}
	}
	// 드롭다운 이벤트 처리 함수
	void handleEvent(Event event, Vector2f mousePos) {
		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
			// 드롭다운 박스를 클릭했을 때 
			if (box.getGlobalBounds().contains(mousePos)) {
				isOpen = !isOpen;
			}
			// 드롭다운 열려 있을 때 
			else if (isOpen) 
			{	
				for (size_t i = 0; i < options.size(); ++i) {
						// 각 옵션 박스를 클릭했을 때 
					if (optionBoxes[i].getGlobalBounds().contains(mousePos)) {
						// 선택 옵션 업데이트
						selectedIndex = i;
						label.setString(options[i].getString());
						isOpen = false;	// 드롭다운 닫기
						break;
					}
				}
			}
		}
	}
	// 선택된 옵션을 반환하는 함수
	const wstring getSelectedOption() const{
		// 선택된 옵션 텍스트를 utf-16으로 변환
		return options[selectedIndex].getString().toWideString();	
	}
};

// 버튼 클래스 정의
Button::Button(float x, float y, string str, Color color) {
	shape.setPosition(x, y);
	shape.setSize({ Width, Height });	
	shape.setFillColor(color);

	// 폰트 로드 + 예외 처리
	if (!font.loadFromFile("HANDotum.ttf")) {
		throw runtime_error("Font loading failed");
	}

	// 버튼 텍스트 설정
	text.setFont(font);
	text.setCharacterSize(25);
	text.setString(to_wstring(str));
	text.setFillColor(Color::Black);
	text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 1.4f);
	text.setPosition(
		shape.getPosition().x + (shape.getSize().x / 2.f),	// 중앙에 텍스트 정렬
		shape.getPosition().y + (shape.getSize().y / 2.f));
}
// 버튼 그리는 함수
void Button::draw(RenderWindow& window) {
	window.draw(shape); 
	window.draw(text); 
}
// 버튼 클릭 여부를 확인하는 함수
bool Button::isClicked(Vector2f mousePos) {
	return shape.getGlobalBounds().contains(mousePos);
}

// 게임 옵션을 보여주는 함수
void showOption() {
	// 옵션 창 생성
	RenderWindow window(VideoMode(App::WIDTH, App::HEIGHT), "Game Option");
	window.setFramerateLimit(60);	// 프레임 제한 설정

	// 드롭다운 + 버튼 생성
	Dropdown mode = Dropdown(App::WIDTH / 7, App::HEIGHT / 3, u8"카드", { u8"기본 카드", u8"아이템 카드" });
	Dropdown level = Dropdown(App::WIDTH / 2, App::HEIGHT / 3, u8"난이도", {u8"쉬움", u8"보통", u8"어려움"});
	Button start = Button(App::WIDTH / 7, (App::HEIGHT / 7) * 5, u8"시작", Color(102, 204, 102));
	Button cancel = Button(App::WIDTH / 2, (App::HEIGHT / 7)*5, u8"취소", Color(192, 192, 192));
	
	while (window.isOpen())
	{
		Event event;
		// 마우스 위치 가져오기
		Vector2f mousePos(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
		
		while (window.pollEvent(event)){
			// 창 닫기 이벤트 처리
			if (event.type == Event::Closed) {
				window.close();	// 창 닫기
				showHome();	// 홈 화면으로 이동
				return;
			}
			// 드롭다운 옵션 선택하는 이벤트 처리
			mode.handleEvent(event, mousePos);
			level.handleEvent(event, mousePos);

			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				// 시작 버튼 클릭 이벤트 처리
				if (start.isClicked(mousePos)) {
					// 카드 모드 설정
					bool isItem = (mode.getSelectedOption() == L"아이템 카드");
					// 난이도 설정
					int selectedLevel;
					if (level.getSelectedOption() == L"쉬움") selectedLevel = 1;
					else if (level.getSelectedOption() == L"보통") selectedLevel = 5;
					else selectedLevel = 9;

					window.close();	// 옵션 창 닫기
					showGame(isItem, selectedLevel);	// 게임 화면으로 이동
					return;
				}
				// 취소 버튼 클릭 이벤트 처리
				if (cancel.isClicked(mousePos)) {
					window.close();	// 옵션 창 닫기
					showHome();	// 홈 화면으로 이동
					return;
				}
			}
		}
		// window 배경 초기화
		window.clear(Color::White);
		// 드롭다운 버튼 그리기
		mode.draw(window);
		level.draw(window);
		start.draw(window);
		cancel.draw(window);
		// 화면에 출력
		window.display();
	}
}