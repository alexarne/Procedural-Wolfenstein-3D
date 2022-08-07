#pragma once

#include <SFML/Graphics.hpp>

class Map
{
public:
	Map(sf::RenderWindow* window);

	void draw();
	void toggleView();
	int** getMap();

	~Map();
private:
	sf::RenderWindow* window;
	bool fullView;
	int mapWidth;
	int mapHeight;
	int** worldMap;
	void createMap();
	int initSize();
	sf::Image img;
	sf::Texture tex;
	sf::Sprite sprite;
	void updateImage();
};

