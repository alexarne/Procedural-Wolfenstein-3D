#pragma once

#include <SFML/Graphics.hpp>
#include "Slider.h"
#include "Checkbox.h"
#include "Button.h"

struct Resolution {
	int w;
	int h;
};

struct Configuration {
	float movementSpeed;
	float sensitivity;
	float fov;
	float visibilityDepth;
	bool useVisibility;
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
	void displayRes(sf::Vector2i mouse);
	void drawTitle(sf::String subtitle, float percY);
	bool isInside(sf::Vector2i mouse, sf::Transformable obj, sf::FloatRect bounds);

	const int NUM_RES = 3;
	Resolution resolutions[3] = {
		{1280, 720},
		{960, 540},
		{640, 360}
	};
	int res;

	Configuration* config;

	int h1;
	int h2; 
	int h3;
	sf::Font f;
	sf::Text resText;
	sf::Text title;

	float subtitleHeight;

	sf::RectangleShape titleLine;

	sf::Text subtitle;

	float resPos;
	Button nextResButton;
	Button prevResButton;

	float sliderOffset;
	float subtitleSpacing;
	float labelSpacing;

	float sensPos;
	Slider movementSlider;
	Slider sensitivitySlider;
	sf::Text labelMovement;
	sf::Text labelMouse;
	float labelMovementPos;
	float labelMousePos;

	float appPos;
	Slider fovSlider;
	Checkbox visibilityCheckbox;
	Slider visibilitySlider;
	float savedFov;
	float savedVis;
	bool savedUseVis;
	sf::Text labelFov;
	sf::Text labelEnableShadows;
	sf::Text labelDistance;
	float labelFovPos;
	float labelEnableShadowsPos;
	float labelDistancePos;
	sf::RectangleShape shadowBlock;

	Button saveButton;
	Button leaveButton;
	Button redoButton;
	sf::Text closeButton;
};

