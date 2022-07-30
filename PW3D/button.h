#pragma once

#include <SFML/Graphics.hpp>

class button
{
public:
	button(int id, float x, float y, float h);

	void draw(sf::RenderWindow& window, sf::Vector2i mouse, bool canHover);
	bool isInside(sf::Vector2i mouse);

	~button();
private:
	float percX;
	float percY;
	float percH;
	sf::Texture texture;
	sf::Vector2u texture_size;
	sf::Sprite sprite;
	sf::Vector2f sprite_size;
};