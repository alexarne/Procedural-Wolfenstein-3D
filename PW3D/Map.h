#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"

class Map
{
public:
	Map(sf::RenderWindow* window, Player* player);

	void draw();
	void toggleView();
	int** getMap();

	~Map();
private:
	sf::RenderWindow* window;
	Player* player;
	bool fullView;
	int mapWidth;
	int mapHeight;
	int** worldMap;
	int pixelsPerUnit;
	int initSize();
	void createWorldMap();
	void createMapImage();
	void drawMap();
	sf::Image mapImage;
	sf::Image drawnMap;
	sf::Texture tex;
	sf::Sprite sprite;
};

