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
	tex.update(drawnMap);
	tex.setSmooth(true);
	sf::Vector2u windowSize = window->getSize();
	float scale = windowSize.y / 720.0;

	// Player indicator
	p.setRadius(3 * scale);
	p.setFillColor(sf::Color(255, 102, 0));

	if (fullView) {
		mapSprite.setTexture(tex);
		mapSprite.setScale(scale, scale);
		sf::FloatRect bounds = mapSprite.getGlobalBounds();
		if (bounds.height > windowSize.y) mapSprite.setScale(scale * 0.9 * windowSize.y / bounds.height, scale * 0.9 * windowSize.y / bounds.height);
		bounds = mapSprite.getGlobalBounds();
		mapSprite.setPosition(windowSize.x / 2 - bounds.width / 2, windowSize.y / 2 - bounds.height / 2);
		bounds = mapSprite.getGlobalBounds();
		sf::Vector2f playerPos = player->getPos();
		sf::Vector2f scopeScale = mapSprite.getScale();
		sf::FloatRect pBounds = p.getGlobalBounds();
		p.setPosition(bounds.left + playerPos.x * scopeScale.x * pixelsPerUnit - pBounds.width / 2, bounds.top + playerPos.y * scopeScale.y * pixelsPerUnit - pBounds.height / 2);
		
		window->draw(mapSprite);
		window->draw(p);
	}
	else {
		float radius = 90 * scale;
		float outlineThickness = 6 * scale;
		float outlineRadius = radius + outlineThickness;
		float margin = 20 * scale;
		sf::Vector2f playerPos = player->getPos();
		int coverage = radius / scale * pixelsPerUnit / 16;
		const int circlePoints = 60;
		float origin = margin + radius;

		sf::Color outlineColor(30, 30, 30);
		sf::Vertex circleMap[circlePoints + 2];
		sf::Vertex circleOutline[circlePoints + 2];
		circleMap[0] = sf::Vertex(sf::Vector2f(origin, origin), sf::Vector2f(playerPos.x * pixelsPerUnit, playerPos.y * pixelsPerUnit));
		circleOutline[0] = sf::Vertex(sf::Vector2f(origin, origin), outlineColor);
		float rot = player->getAngle();
		float step = 2 * 3.14159265359 / circlePoints;
		for (int i = 1; i <= circlePoints; i++) {
			float angle = (i - 1) * step;
			circleMap[i] = sf::Vertex(
				sf::Vector2f(origin + radius * cos(angle), origin + radius * sin(angle)),
				sf::Vector2f(pixelsPerUnit * playerPos.x + coverage * cos(angle + rot), pixelsPerUnit * playerPos.y + coverage * sin(angle + rot))
			);
			circleOutline[i] = sf::Vertex(
				sf::Vector2f(origin + outlineRadius * cos(angle), origin + outlineRadius * sin(angle)),
				outlineColor
			);
		}
		circleMap[circlePoints + 1] = circleMap[1];
		circleOutline[circlePoints + 1] = circleOutline[1];

		p.setScale(1.4, 1.4);
		sf::FloatRect bounds = p.getGlobalBounds();
		p.setPosition(margin + radius - bounds.width / 2, margin + radius - bounds.height / 2);

		sf::RenderStates st(&tex);
		window->draw(circleOutline, circlePoints + 2, sf::TriangleFan);
		window->draw(circleMap, circlePoints + 2, sf::TriangleFan, st);
		window->draw(p);
	}
	Map::drawMap();
}

void Map::seenWall(float x0, float y0, float x1, float y1) {
	float incr = 1 / (pixelsPerUnit * sqrt(x1 * x1 + y1 * y1));
	sf::Color c = sf::Color(200, 200, 200);
	for (float i = 0; i < 1; i += incr) {
		drawnMap.setPixel(pixelsPerUnit * (x0 + i * x1), pixelsPerUnit * (y0 + i * y1), c);
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
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,1},
		{1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,1},
		{1,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,1},
		{1,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
		{1,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,1},
		{1,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
		{1,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
		{1,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
		{1,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,1},
		{1,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,1},
		{1,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,1},
		{1,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,1},
		{1,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,1},
		{1,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
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
			sf::Color color = Assets::getMapColor(worldMap[i][j]);
			for (int x = 0; x < pixelsPerUnit; x++) {
				for (int y = 0; y < pixelsPerUnit; y++) {
					img.setPixel(j * pixelsPerUnit + x, i * pixelsPerUnit + y, color);
				}
			}
		}
	}
	this->mapImage = img;
}