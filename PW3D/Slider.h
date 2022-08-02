#pragma once

#include <SFML/Graphics.hpp>


class Slider
{
public:
	Slider(sf::RenderWindow* window, float x, float y, float w, float h, float f, float t);
	Slider();

	void draw(sf::Vector2i mouse);
	void handleEvent(sf::Event event);
	void setValue(float value);
	float getValue();

	~Slider();
private:
	void updateValue(sf::Vector2i mouse);
	void updateHandle();
	bool isInside(sf::Vector2i mouse);

	sf::RenderWindow* window;
	float percX;
	float percY;
	float percW;
	float percH;
	float from;
	float to;
	float value;
	sf::RectangleShape handle;
	sf::RectangleShape background;
	bool holdingHandle;
	bool prevHover;
};

