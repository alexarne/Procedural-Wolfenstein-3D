#pragma once

#include <SFML/Graphics.hpp>
#include "Slider.h"

struct Resolution {
	int w;
	int h;
};

struct Config {
	float movementSpeed;
	float rotationSpeed;
	int fov;
	Resolution res;
};

class Settings
{
public:
	Settings(sf::RenderWindow* win);

	void draw(sf::Vector2i mouse);
	bool visible();
	void toggle();
	void handleEvent(sf::Event event);

	float getMovementSpeed();
	float getRotationSpeed();
	int getFOV();
	Resolution getResolution();
	Config getConfig();

	~Settings();
private:
	sf::RenderWindow* window;
	bool active;
	float percW;
	float percH;
	sf::RectangleShape overlay;
	sf::RectangleShape settingsWindow;
	sf::Vector2f settingsSize;

	void reset();
	void save();
	void mouseAction(sf::Vector2i mouse);
	void prevRes();
	void nextRes();

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

	Slider movementSlider;
	Slider rotationSlider;
};

