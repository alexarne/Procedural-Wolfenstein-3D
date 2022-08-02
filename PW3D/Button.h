#pragma once

#include <SFML/Graphics.hpp>

#define ALIGN_LEFT 0
#define ALIGN_CENTER 1
#define ALIGN_RIGHT 2


class Button
{
public:
	Button();

	void init(sf::RenderWindow* win, sf::String s, float x, float y, float w, float h, int charSize, int ALIGN);
	void draw(sf::Vector2i mouse, bool canHover);
	void setButtonColor(sf::Color color);
	void setHoverColor(sf::Color color);
	void setBorder(float thickness);
	void setTextColor(sf::Color color);
	bool isInside(sf::Vector2i mouse);

	~Button();
private:
	sf::RenderWindow* window;
	float percX;
	float percY;
	float percW;
	float percH;
	float baseHeight;
	sf::RectangleShape button;
	sf::Color baseColor;
	sf::Color hoverColor;
	float thickness;
	sf::Font f;
	sf::Text text;
	sf::Color textColor;

	sf::Texture texture;
	sf::Vector2u texture_size;
	sf::Sprite sprite;
	sf::Vector2f sprite_size;
};
