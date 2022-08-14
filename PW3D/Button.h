#pragma once

#include <SFML/Graphics.hpp>
#include "Assets.hpp"
#include "align.h"


class Button
{
public:
	Button();

	void init(sf::RenderWindow* win, sf::String s, float x, float y, float w, float h, int charSize, int ALIGN);
	void draw(sf::Vector2i mouse, bool canHover);
	void setButtonColor(sf::Color color);
	void setButtonHoverColor(sf::Color color);
	void setBorder(float thickness);
	void setTextColor(sf::Color color);
	bool isInside(sf::Vector2i mouse);
	void setTextHoverColor(sf::Color color);

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
	sf::Color textBaseColor;
	sf::Color textHoverColor;

	sf::Texture texture;
	sf::Vector2u texture_size;
	sf::Sprite sprite;
	sf::Vector2f sprite_size;
};
