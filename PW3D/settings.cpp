#include "Settings.h"
#include <iostream>
#include "Slider.h"

Settings::Settings(sf::RenderWindow* win) {
	window = win;
	percW = (float) 300 / 1280;
	percH = (float) 500 / 720;
	active = true;

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

	movementSlider = Slider(window, 0.5, 0.5, 0.1, 0.03, 1, 10, 5);
	rotationSlider = Slider(window, 0.5, 0.6, 0.1, 0.03, 1, 10, 5);
}

void Settings::draw(sf::Vector2i mouse) {
	if (!active) return;
	sf::Vector2u windowSize = window->getSize();
	overlay = sf::RectangleShape(sf::Vector2f(windowSize.x, windowSize.y));
	overlay.setFillColor(sf::Color(0, 0, 0, 128));
	settingsWindow = sf::RectangleShape(sf::Vector2f(windowSize.x * percW, windowSize.y * percH));
	settingsSize = settingsWindow.getSize();
	settingsWindow.setPosition((float)(windowSize.x - settingsSize.x) / 2, (float)(windowSize.y - settingsSize.y) / 2);
	window->draw(overlay);
	window->draw(settingsWindow);
	movementSlider.draw(mouse);
	rotationSlider.draw(mouse);
}

void Settings::reset() {
	movementSpeed = configuration.movementSpeed;
	rotationSpeed = configuration.rotationSpeed;
	fov = configuration.fov;
	for (int i = 0; i < NUM_RES; i++) {
		if (resolutions[i].w == configuration.res.w) res = i;
	}
}

void Settings::save() {
	configuration = {
		movementSpeed,
		rotationSpeed,
		fov,
		resolutions[res]
	};
	window->setSize(sf::Vector2u(resolutions[res].w, resolutions[res].h));
	window->setView(sf::View(sf::FloatRect(0, 0, resolutions[res].w, resolutions[res].h)));
}

bool Settings::visible() {
	return active;
}

void Settings::toggle() {
	active = !active;
	Settings::reset();
}

void Settings::mouseAction(sf::Vector2i mouse) {
	sf::Vector2f position = settingsWindow.getPosition();
	if (!(mouse.x > position.x && mouse.x < position.x + settingsSize.x &&
		  mouse.y > position.y && mouse.y < position.y + settingsSize.y)) 
		active = false;
}

void Settings::handleEvent(sf::Event event) {
	sf::Vector2i click(event.mouseButton.x, event.mouseButton.y);
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		Settings::mouseAction(click);
		break;
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Enter) Settings::save();
		if (event.key.code == sf::Keyboard::Left) {
			Settings::prevRes();
			printf("selected res is %ix%i\n", resolutions[res].w, resolutions[res].h);
		}
		if (event.key.code == sf::Keyboard::Right) {
			Settings::nextRes();
			printf("selected res is %ix%i\n", resolutions[res].w, resolutions[res].h);
		}

		break;
	}
	movementSlider.handleEvent(event);
	rotationSlider.handleEvent(event);
}

void Settings::prevRes() {
	if (--res < 0) res = NUM_RES - 1;
}

void Settings::nextRes() {
	if (++res >= NUM_RES) res = 0;
}

float Settings::getMovementSpeed() { return movementSpeed; }
float Settings::getRotationSpeed() { return rotationSpeed; }
int Settings::getFOV() { return fov; }
Resolution Settings::getResolution() { return resolutions[res]; }
Config Settings::getConfig() { return configuration; }

Settings::~Settings() {

}