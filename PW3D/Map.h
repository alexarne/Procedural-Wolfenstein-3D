#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Settings.h"
#include "Assets.hpp"

class Map
{
public:
	Map(sf::RenderWindow* window, Player* player, Settings* settings);

	void draw();
	void toggleView();
	int** getMap();
	void seenWall(float x0, float y0, float x1, float y1);

	~Map();
private:
	sf::RenderWindow* window;
	Player* player;
	Settings* settings;
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

