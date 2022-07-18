#pragma once

#include <SFML/Graphics.hpp>

#define MAX_MENU 4

class MainMenu
{
public:
	MainMenu();

	void draw(sf::RenderWindow &window);
	void reset();

	~MainMenu();
private:
	sf::Font font;
	sf::Text title;
	sf::Text menu[MAX_MENU];
};