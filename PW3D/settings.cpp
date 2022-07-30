#include "settings.h"
#include <iostream>

settings::settings() {
	percW = (float) 300 / 1280;
	percH = (float) 500 / 720;
	active = false;

	movementSpeed = 1;
	rotationSpeed = 1;
	fov = 60;
	res = 0;
	configuration = { 
		movementSpeed, 
		rotationSpeed, 
		fov, 
		resolutions[res]
	};
}

void settings::draw(sf::RenderWindow& window) {
	if (!active) return;
	sf::Vector2u windowSize = window.getSize();
	overlay = sf::RectangleShape(sf::Vector2f(windowSize.x, windowSize.y));
	overlay.setFillColor(sf::Color(0, 0, 0, 128));
	settingsWindow = sf::RectangleShape(sf::Vector2f(windowSize.x * percW, windowSize.y * percH));
	settingsSize = settingsWindow.getSize();
	settingsWindow.setPosition((float)(windowSize.x - settingsSize.x) / 2, (float)(windowSize.y - settingsSize.y) / 2);
	window.draw(overlay);
	window.draw(settingsWindow);
}

void settings::reset() {
	movementSpeed = configuration.movementSpeed;
	rotationSpeed = configuration.rotationSpeed;
	fov = configuration.fov;
	for (int i = 0; i < NUM_RES; i++) {
		if (resolutions[i].w == configuration.res.w) res = i;
	}
}

void settings::save() {
	configuration = {
		movementSpeed,
		rotationSpeed,
		fov,
		resolutions[res]
	};
}

bool settings::visible() {
	return active;
}

void settings::toggle() {
	active = !active;
	settings::reset();
}

void settings::mouseAction(sf::Vector2i mouse) {
	sf::Vector2f position = settingsWindow.getPosition();
	if (!(mouse.x > position.x && mouse.x < position.x + settingsSize.x &&
		  mouse.y > position.y && mouse.y < position.y + settingsSize.y)) 
		active = false;
	settings::prevRes();
	printf("incr to %i\n", res);
	settings::save();
}

void settings::prevRes() {
	if (--res < 0) res = NUM_RES - 1;
}

void settings::nextRes() {
	if (++res >= NUM_RES) res = 0;
}

float settings::getMovementSpeed() { return movementSpeed; }
float settings::getRotationSpeed() { return rotationSpeed; }
int settings::getFOV() { return fov; }
Resolution settings::getResolution() { return resolutions[res]; }
Config settings::getConfig() { return configuration; }

settings::~settings() {

}