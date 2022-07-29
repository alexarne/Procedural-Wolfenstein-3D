#pragma once

#include <SFML/Graphics.hpp>

class button
{
public:
	button(sf::String dir, float x, float y, float w, float h);

	void draw(sf::RenderWindow& window, sf::Vector2i mouse);
	bool isHover(sf::Vector2i mouse);

	~button();
private:
	float percX;
	float percY;
	float percW;
	float percH;
	sf::Texture texture;
	sf::Vector2u texture_size;
	sf::Sprite sprite;
	sf::Vector2f sprite_size;
};