#ifndef OPTION_H
#define OPTION_H

#include <SFML/Graphics.hpp>
#include <string>

// 옵션 함수 선언
void showOption();

// utf-8 문자열을 utf-16으로 변환하는 함수 선언
std::wstring to_wstring(const std::string& str);

// 버튼 클래스 정의
class Button {
private:
	sf::RectangleShape shape;	// 사각형 버튼
	const float Width = 200.f;	// 버튼 너비
	const float Height = 50.f;	// 버튼 높이
	sf::Font font;	// 폰트
	sf::Text text;	// 버튼 텍스트
public:
	Button(float x, float y, std::string str, sf::Color color);	// 버튼 생성자
	void draw(sf::RenderWindow& window);	// 버튼 그리기
	bool isClicked(sf::Vector2f mousePos);	// 버튼 클릭 여부 확인
};

#endif