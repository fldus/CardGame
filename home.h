#ifndef HOME_H
#define HOME_H

#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;
using namespace std;

class Record {
private:
	Font font;
	Text cardMode;
	Text startlevel;
	Text text;
	int level = 0;
	int score = 0;
public:
    Record(float x, float y, std::string mode, std::string slevel);
    void draw(sf::RenderWindow& window);
    void update(int level, int score);
};

extern Record be, bc, bh, ie, ic, ih;

void showHome();

#endif