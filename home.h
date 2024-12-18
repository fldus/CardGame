#ifndef HOME_H
#define HOME_H

#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;
using namespace std;

// Record 클래스 정의
class Record {
private:
	Font font;
	Text cardMode;	// 카드 모드
	Text startlevel;	// 난이도
	Text text;	// 레벨, 점수 Text
	int level = 0;	// 최고 레벨
	int score = 0;	// 최고 점수
public:
	// Record 생성자
    Record(float x, float y, std::string mode, std::string slevel);
    // Record 그리기
	void draw(sf::RenderWindow& window);
	// 최고 기록, 점수 업데이트하는 함수
    void update(int level, int score);
};

// 전역 Record 변수 선언
extern Record be, bc, bh, ie, ic, ih;

// 시작 화면 호출 함수
void showHome();

#endif