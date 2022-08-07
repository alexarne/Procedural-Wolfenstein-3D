#include "Map.h"



Map::Map(sf::RenderWindow* window) {
	this->window = window;
	fullView = false;
	mapHeight = Map::initSize();
	mapWidth = Map::initSize();
	worldMap = new int* [mapHeight];
	for (int i = 0; i < mapHeight; i++) {
		worldMap[i] = new int[mapWidth];
		for (int j = 0; j < mapWidth; j++) {
			worldMap[i][j] = 0;
		}
	}
	
	Map::createMap();
	Map::updateImage();
	tex.loadFromImage(img);
}

Map::~Map() {

}

void Map::draw() {
	Map::updateImage();
	tex.update(img);
	tex.setSmooth(true);
	sprite.setTexture(tex);
	sf::Vector2u windowSize = window->getSize();
	float scale = windowSize.y / 720.0;

	if (fullView) {
		sf::FloatRect bounds = sprite.getGlobalBounds();
		sprite.setPosition(windowSize.x / 2 - bounds.width / 2, windowSize.y / 2 - bounds.height / 2);
	}
	else {
		sprite.setPosition(10 * scale, 10 * scale);
	}

	window->draw(sprite);
}

void Map::toggleView() {
	fullView = !fullView;
}

void Map::createMap() {
	int temp[24][24] = {
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
		{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
		{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			worldMap[i][j] = temp[i][j];
		}
	}
}

int Map::initSize() {
	return 24;
}

int** Map::getMap() { return worldMap; }

void Map::updateImage() {
	sf::Image img;
	int pixelsPerUnit = 6;
	img.create(mapWidth * pixelsPerUnit, mapHeight * pixelsPerUnit);
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			sf::Color color(255, 0, 0);
			switch (worldMap[i][j])
			{
			case 0:
				color = sf::Color::White;
				break;
			case 1:
				color = sf::Color::Blue;
				break;
			case 2:
				color = sf::Color::Red;
				break;
			case 3:
				color = sf::Color::Green;
				break;
			case 4:
				color = sf::Color::Cyan;
				break;
			case 5:
				color = sf::Color::Magenta;
				break;
			}
			for (int x = 0; x < pixelsPerUnit; x++) {
				for (int y = 0; y < pixelsPerUnit; y++) {
					img.setPixel(j * pixelsPerUnit + x, i * pixelsPerUnit + y, color);
				}
			}
		}
	}
	this->img = img;
}