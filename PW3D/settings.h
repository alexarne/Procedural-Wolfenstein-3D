#pragma once

#include <SFML/Graphics.hpp>

struct Resolution {
	float w;
	float h;
};

struct Config {
	float movementSpeed;
	float rotationSpeed;
	int fov;
	Resolution res;
};

class settings
{
public:
	settings();

	void draw(sf::RenderWindow& window);
	void reset();
	bool visible();
	void toggle();
	void mouseAction(sf::Vector2i mouse);

	float getMovementSpeed();
	float getRotationSpeed();
	int getFOV();
	Resolution getResolution();
	Config getConfig();
	void prevRes();
	void nextRes();

	~settings();
private:
	bool active;
	float percW;
	float percH;
	sf::RectangleShape overlay;
	sf::RectangleShape settingsWindow;
	sf::Vector2f settingsSize;

	float movementSpeed;
	float rotationSpeed;
	int fov;
	const int NUM_RES = 3;
	Resolution resolutions[3] = {
		{1280, 720},
		{960, 540},
		{640, 360}
	};
	int res;

	Config configuration;
	void save();
};

