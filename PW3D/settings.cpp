#include "Settings.h"
#include <iostream>
#include "Slider.h"
#include "assets.hpp"
#include <string>

Settings::Settings(sf::RenderWindow* win, Configuration* config) {
	window = win;
	percW = (float) 300 / 1280;
	percH = (float) 500 / 720;
	active = false;
	int margin = 20;
	startX = (1.0 - percW) / 2 + margin / 1280.0;
	endX = 0.5 + percW / 2 - margin / 1280.0;
	startY = (1.0 - percH) / 2 + margin / 720.0;
	endY = 0.5 + percH / 2 - margin / 720.0;

	// Default settings
	config->movementSpeed = 1;
	config->sensitivity = 1;
	config->fov = 60;
	config->res = 0;
	this->config = config;

	h1 = 34;
	h2 = 28;
	h3 = 20;
	f.loadFromMemory(&font, font_len);
	resText.setFont(f);
	resText.setCharacterSize(h3);
	resText.setFillColor(sf::Color::Black);
	Settings::displayRes();
	title.setFont(f);
	title.setCharacterSize(h1);
	title.setFillColor(sf::Color::Black);
	title.setString("Settings");

	movementSlider = Slider(window, 0.5, 0.5, 0.1, 0.03, 1, 10);
	sensitivitySlider = Slider(window, 0.5, 0.6, 0.1, 0.03, 1, 10);
	fovSlider = Slider(window, 0.5, 0.7, 0.1, 0.03, 30, 170);
	/*nextResButton = Button();
	prevResButton = Button();
	closeButton = Button();
	saveButton = Button();
	leaveButton = Button();
	redoButton = Button();*/

	Settings::reset();
}

void Settings::draw(sf::Vector2i mouse) {
	if (!active) return;
	sf::Vector2u windowSize = window->getSize();

	// Draw overlay and body
	overlay = sf::RectangleShape(sf::Vector2f(windowSize.x, windowSize.y));
	overlay.setFillColor(sf::Color(0, 0, 0, 128));
	settingsWindow = sf::RectangleShape(sf::Vector2f(windowSize.x * percW, windowSize.y * percH));
	settingsSize = settingsWindow.getSize();
	settingsWindow.setPosition((float)(windowSize.x - settingsSize.x) / 2, (float)(windowSize.y - settingsSize.y) / 2);
	window->draw(overlay);
	window->draw(settingsWindow);

	// Draw title
	float scale = windowSize.y / 720.0;
	title.setScale(sf::Vector2f(scale, scale));
	sf::FloatRect titleBounds = title.getLocalBounds();
	title.setOrigin(titleBounds.left, titleBounds.top);
	title.setPosition((float)windowSize.x / 2 - scale * (float)titleBounds.width / 2, (startY + 20.0 / 720) * windowSize.y);
	window->draw(title);

	// Draw resolution section
	Settings::displayRes();

	// Draw sliders
	movementSlider.draw(mouse);
	sensitivitySlider.draw(mouse);
	fovSlider.draw(mouse);

	// Draw labels

	// Draw extra buttons

	/* Margin showcase
	sf::RectangleShape first(sf::Vector2f(100, 200));
	first.setPosition(startX * windowSize.x, 0.4 * windowSize.y);
	first.setFillColor(sf::Color::Black);
	sf::RectangleShape second(sf::Vector2f(200, 100));
	second.setPosition(0.5 * windowSize.x, startY * windowSize.y);
	second.setFillColor(sf::Color::Black);
	window->draw(first);
	window->draw(second);*/
}

void Settings::reset() {
	movementSlider.setValue(config->movementSpeed);
	sensitivitySlider.setValue(config->sensitivity);
	fovSlider.setValue(config->fov);
	res = config->res;
}

void Settings::save() {
	config->movementSpeed = movementSlider.getValue();
	config->sensitivity = sensitivitySlider.getValue();
	config->fov = fovSlider.getValue();
	config->res = res;
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

void Settings::mouseClick(sf::Vector2i mouse) {
	if (!settingsWindow.getGlobalBounds().contains(sf::Vector2f(mouse.x, mouse.y))) active = false;
}

void Settings::handleEvent(sf::Event event) {
	sf::Vector2i click(event.mouseButton.x, event.mouseButton.y);
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		Settings::mouseClick(click);
		break;
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Enter) Settings::save();
		if (event.key.code == sf::Keyboard::Left) Settings::prevRes();
		if (event.key.code == sf::Keyboard::Right) Settings::nextRes();

		break;
	}
	movementSlider.handleEvent(event);
	sensitivitySlider.handleEvent(event);
	fovSlider.handleEvent(event);
}

void Settings::prevRes() {
	if (--res < 0) res = NUM_RES - 1;
}

void Settings::nextRes() {
	if (++res >= NUM_RES) res = 0;
}

void Settings::displayRes() {
	sf::Vector2u windowSize = window->getSize();
	resText.setString(std::to_string(resolutions[res].w) + "x" + std::to_string(resolutions[res].h));
	float scale = (float) windowSize.y / 720;
	resText.setScale(sf::Vector2f(scale, scale));
	sf::FloatRect bounds = resText.getLocalBounds();
	resText.setOrigin(bounds.left, bounds.top);
	resText.setPosition((float) windowSize.x / 2 - scale * (float) bounds.width / 2, (startY + 80.0/720) * windowSize.y);
	window->draw(resText);
}

Resolution Settings::getResolution() { return resolutions[res]; }
Configuration* Settings::getConfig() { return config; }

Settings::~Settings() {

}