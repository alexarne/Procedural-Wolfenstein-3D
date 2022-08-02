#pragma once

#include <SFML/Graphics.hpp>

class Checkbox
{
public:
	Checkbox(sf::RenderWindow* window, float percX, float percY, bool checked);
	Checkbox();

	void draw(sf::Vector2i mouse);
	void handleEvent(sf::Event event);
	void setValue(bool value);
	bool getValue();

	~Checkbox();
private:
	sf::RenderWindow* window;
	bool checked;
	bool hovering;
	float percX;
	float percY;
	sf::CircleShape box;

	bool isInside(sf::Vector2i mouse);

	sf::Color fillColor;
	sf::Color hoverColor;
};

