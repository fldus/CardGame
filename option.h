#ifndef OPTION_H
#define OPTION_H

#include <SFML/Graphics.hpp>
#include <string>

// �ɼ� �Լ� ����
void showOption();

// utf-8 ���ڿ��� utf-16���� ��ȯ�ϴ� �Լ� ����
std::wstring to_wstring(const std::string& str);

// ��ư Ŭ���� ����
class Button {
private:
	sf::RectangleShape shape;	// �簢�� ��ư
	const float Width = 200.f;	// ��ư �ʺ�
	const float Height = 50.f;	// ��ư ����
	sf::Font font;	// ��Ʈ
	sf::Text text;	// ��ư �ؽ�Ʈ
public:
	Button(float x, float y, std::string str, sf::Color color);	// ��ư ������
	void draw(sf::RenderWindow& window);	// ��ư �׸���
	bool isClicked(sf::Vector2f mousePos);	// ��ư Ŭ�� ���� Ȯ��
};

#endif