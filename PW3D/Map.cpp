#include "Map.h"



Map::Map(sf::RenderWindow* window, Player* player, Settings* settings) {
	this->window = window;
	this->player = player;
	this->settings = settings;
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
	pixelsPerUnit = 16;
	
	Map::createWorldMap();
	Map::createMapImage();
	Map::drawMap();
	tex.loadFromImage(drawnMap);
}

Map::~Map() {

}

void Map::draw() {
	Map::drawMap();
	tex.update(drawnMap);
	tex.setSmooth(false);
	sprite.setTexture(tex);
	sf::Vector2u windowSize = window->getSize();
	float scale = windowSize.y / 720.0;
	sprite.setScale(scale, scale);

	// Player indicator
	sf::CircleShape p(3 * scale);
	p.setFillColor(sf::Color(255, 102, 0));

	if (fullView) {
		sf::FloatRect bounds = sprite.getGlobalBounds();
		if (bounds.height > windowSize.y) sprite.setScale(scale * 0.9 * windowSize.y / bounds.height, scale * 0.9 * windowSize.y / bounds.height);
		bounds = sprite.getGlobalBounds();
		sprite.setPosition(windowSize.x / 2 - bounds.width / 2, windowSize.y / 2 - bounds.height / 2);
		bounds = sprite.getGlobalBounds();
		sf::Vector2f playerPos = player->getPos();
		sf::Vector2f scopeScale = sprite.getScale();
		p.setPosition(bounds.left + playerPos.x * scopeScale.x * pixelsPerUnit, bounds.top + playerPos.y * scopeScale.y * pixelsPerUnit);
		window->draw(sprite);
		window->draw(p);
	}
	else {
		tex.setSmooth(true);
		float radius = 90 * scale;
		int margin = 20 * scale;
		sf::CircleShape mapRegion(radius);
		sprite.setPosition(margin, margin);
		sf::FloatRect bounds = mapRegion.getGlobalBounds();
		mapRegion.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
		mapRegion.setPosition(margin + radius, margin + radius);
		mapRegion.setPointCount(60);
		mapRegion.setOutlineColor(sf::Color(30, 30, 30));
		mapRegion.setOutlineThickness(6 * scale);
		mapRegion.setTexture(&tex);
		sf::Vector2f playerPos = player->getPos();
		int coverage = 2 * radius / scale;
		sf::IntRect mask(playerPos.x * pixelsPerUnit - coverage / 2, playerPos.y * pixelsPerUnit - coverage / 2, coverage, coverage);
		mapRegion.setTextureRect(mask);
		mapRegion.setRotation(-1 * player->getAngle() * 180 / 3.14159265359);
		p.setScale(1.4, 1.4);
		bounds = p.getGlobalBounds();
		p.setPosition(margin + radius - bounds.width / 2, margin + radius - bounds.height / 2);
		window->draw(mapRegion, &tex);
		window->draw(p);
	}


}

void Map::toggleView() {
	fullView = !fullView;
}

void Map::drawMap() {
	drawnMap = mapImage;
}

void Map::createWorldMap() {
	int temp[24][24] = {
		{4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
		{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
		{4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
		{4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
		{4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
		{4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
		{4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
		{4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
		{4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
		{4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
		{4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
		{4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
		{6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
		{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
		{6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
		{4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
		{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
		{4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
		{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
		{4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
		{4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
		{4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
		{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
		{4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
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

void Map::createMapImage() {
	sf::Image img;
	img.create(mapWidth * pixelsPerUnit, mapHeight * pixelsPerUnit);
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			sf::Color color = getMapColor(worldMap[i][j]);
			for (int x = 0; x < pixelsPerUnit; x++) {
				for (int y = 0; y < pixelsPerUnit; y++) {
					img.setPixel(j * pixelsPerUnit + x, i * pixelsPerUnit + y, color);
				}
			}
		}
	}
	this->mapImage = img;
}