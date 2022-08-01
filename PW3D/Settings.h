#pragma once

#include <SFML/Graphics.hpp>
#include "Slider.h"

struct Resolution {
	int w;
	int h;
};

struct Configuration {
	float movementSpeed;
	float sensitivity;
	int fov;
	int res;
};

class Settings
{
public:
	Settings(sf::RenderWindow* win, Configuration* config);

	void draw(sf::Vector2i mouse);
	bool visible();
	void toggle();
	void handleEvent(sf::Event event);

	Resolution getResolution();
	Configuration* getConfig();

	~Settings();
private:
	sf::RenderWindow* window;
	bool active;
	float percW;
	float percH;
	float startX;
	float endX;
	float startY;
	float endY;
	sf::RectangleShape overlay;
	sf::RectangleShape settingsWindow;
	sf::Vector2f settingsSize;

	void reset();
	void save();
	void mouseClick(sf::Vector2i mouse);
	void prevRes();
	void nextRes();
	void displayRes();

	const int NUM_RES = 3;
	Resolution resolutions[3] = {
		{1280, 720},
		{960, 540},
		{640, 360}
	};
	int res;

	Configuration* config;
	Slider movementSlider;
	Slider sensitivitySlider;
	Slider fovSlider;

	int h1;
	int h2; 
	int h3;
	sf::Font f;
	sf::Text resText;
	sf::Text title;
};

