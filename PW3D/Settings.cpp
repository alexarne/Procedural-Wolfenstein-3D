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
	inGame = false;
	int margin = 20;
	startX = (1.0 - percW) / 2 + margin / 1280.0;
	endX = 0.5 + percW / 2 - margin / 1280.0;
	startY = (1.0 - percH) / 2 + margin / 720.0;
	endY = 0.5 + percH / 2 - margin / 720.0;

	// Default settings
	savedFov = 66;
	savedVis = 8;
	savedUseVis = true;
	res = 2;
	config->movementSpeed = 2;
	config->sensitivity = 0.003;
	config->fov = savedFov;
	config->visibilityDepth = savedVis;
	config->useVisibility = savedUseVis;
	config->res = res;
	this->config = config;

	h1 = 34;
	h2 = 24;
	h3 = 16;
	f.loadFromMemory(&font, font_len);
	resText.setFont(f);
	resText.setCharacterSize(h3);
	resText.setFillColor(sf::Color::Black);
	title.setFont(f);
	title.setCharacterSize(h1);
	title.setFillColor(sf::Color::Black);
	title.setString("Settings");
	closeButton.setFont(f);
	closeButton.setCharacterSize(28);
	closeButton.setString("x");

	titleLine = sf::RectangleShape(sf::Vector2f());
	titleLine.setOutlineColor(sf::Color(150, 150, 150));
	
	subtitle.setFont(f);
	subtitle.setCharacterSize(h2);
	subtitle.setFillColor(sf::Color::Black);
	subtitle.setString("Resolution");
	subtitleHeight = subtitle.getLocalBounds().height / 720;

	resPos = startY + 65 / 720.0;

	nextResButton.init(window, ">", 0.54, resPos + 45.0 / 720, 17 / 1280.0, 17 / 720.0, 20, ALIGN_RIGHT);
	nextResButton.setButtonColor(sf::Color::Transparent);
	nextResButton.setButtonHoverColor(sf::Color::Transparent);
	nextResButton.setTextColor(sf::Color::Black);
	nextResButton.setTextHoverColor(sf::Color::Black + sf::Color(100, 100, 100));
	prevResButton.init(window, "<", 0.46, resPos + 45.0 / 720, 17 / 1280.0, 17 / 720.0, 20, ALIGN_LEFT);
	prevResButton.setButtonColor(sf::Color::Transparent);
	prevResButton.setButtonHoverColor(sf::Color::Transparent);
	prevResButton.setTextColor(sf::Color::Black);
	prevResButton.setTextHoverColor(sf::Color::Black + sf::Color(100, 100, 100));

	float sliderHeight = 15 / 720.0, sliderWidth = 130 / 1280.0;
	int SLIDER_ALIGN = ALIGN_LEFT;

	sensPos = resPos + 90 / 720.0;
	appPos = sensPos + 110 / 720.0;

	sliderOffset = - 2 / 720.0;
	subtitleSpacing = 45 / 720.0;
	labelSpacing = 30 / 720.0;

	labelMovementPos = sensPos + subtitleSpacing;
	labelMousePos = labelMovementPos + labelSpacing;
	labelFovPos = appPos + subtitleSpacing;
	labelEnableShadowsPos = labelFovPos + labelSpacing;
	labelDistancePos = labelEnableShadowsPos + labelSpacing;

	movementSlider = Slider(window, endX, labelMovementPos + sliderOffset, sliderWidth, sliderHeight, config->movementSpeed * 0.2, config->movementSpeed * 1.8, SLIDER_ALIGN);
	sensitivitySlider = Slider(window, endX, labelMousePos + sliderOffset, sliderWidth, sliderHeight, config->sensitivity * 0.2, config->sensitivity * 1.8, SLIDER_ALIGN);

	fovSlider = Slider(window, endX, labelFovPos + sliderOffset, sliderWidth, sliderHeight, config->fov * 0.2, config->fov * 1.8, SLIDER_ALIGN);
	visibilityCheckbox = Checkbox(window, 0.5 + 3 / 1280.0, labelEnableShadowsPos + sliderOffset, true, ALIGN_RIGHT);
	visibilitySlider = Slider(window, endX, labelDistancePos + sliderOffset, sliderWidth, sliderHeight, config->visibilityDepth * 0.2, config->visibilityDepth * 1.8, SLIDER_ALIGN);

	labelMovement.setFont(f);
	labelMovement.setCharacterSize(h3);
	labelMovement.setFillColor(sf::Color::Black);
	labelMovement.setString("Movement");
	labelMouse.setFont(f);
	labelMouse.setCharacterSize(h3);
	labelMouse.setFillColor(sf::Color::Black);
	labelMouse.setString("Mouse");
	labelFov.setFont(f);
	labelFov.setCharacterSize(h3);
	labelFov.setFillColor(sf::Color::Black);
	labelFov.setString("Field of View");
	labelEnableShadows.setFont(f);
	labelEnableShadows.setCharacterSize(h3);
	labelEnableShadows.setFillColor(sf::Color::Black);
	labelEnableShadows.setString("Enable Shadows");
	labelDistance.setFont(f);
	labelDistance.setCharacterSize(h3);
	labelDistance.setFillColor(sf::Color::Black);
	labelDistance.setString("Shadow Depth");

	sf::FloatRect bounds = labelMovement.getLocalBounds();
	labelMovement.setOrigin(bounds.left, bounds.top);
	bounds = labelMouse.getLocalBounds();
	labelMouse.setOrigin(bounds.left, bounds.top);
	bounds = labelFov.getLocalBounds();
	labelFov.setOrigin(bounds.left, bounds.top);
	bounds = labelEnableShadows.getLocalBounds();
	labelEnableShadows.setOrigin(bounds.left, bounds.top);
	bounds = labelDistance.getLocalBounds();
	labelDistance.setOrigin(bounds.left, bounds.top);

	float buttonHeight = 25 / 720.0, buttonWidth = 70 / 1280.0;
	saveButton.init(window, "Save", 0.5, endY - buttonHeight, buttonWidth, buttonHeight, h3, ALIGN_CENTER);
	saveButton.setBorder(1 / 720.0);
	saveButton.setButtonColor(sf::Color::Black);
	saveButton.setButtonHoverColor(sf::Color::Transparent);
	saveButton.setTextHoverColor(sf::Color::Black);
	leaveButton.init(window, "Leave", startX, endY - buttonHeight, buttonWidth, buttonHeight, h3, ALIGN_RIGHT);
	leaveButton.setBorder(1 / 720.0);
	leaveButton.setButtonColor(sf::Color(81, 126, 214));
	leaveButton.setButtonHoverColor(sf::Color::Transparent);
	leaveButton.setTextHoverColor(sf::Color(81, 126, 214));
	restartButton.init(window, "Restart", endX, endY - buttonHeight, buttonWidth, buttonHeight, h3, ALIGN_LEFT);
	restartButton.setBorder(1 / 720.0);
	restartButton.setButtonColor(sf::Color(81, 126, 214));
	restartButton.setButtonHoverColor(sf::Color::Transparent);
	restartButton.setTextHoverColor(sf::Color(81, 126, 214));

	Settings::reset();
}

void Settings::draw(sf::Vector2i mouse) {
	if (!active) return;
	sf::Vector2u windowSize = window->getSize();

	float scale = windowSize.y / 720.0;

	// Draw overlay and body
	overlay = sf::RectangleShape(sf::Vector2f(windowSize.x, windowSize.y));
	overlay.setFillColor(sf::Color(0, 0, 0, 128));
	settingsWindow = sf::RectangleShape(sf::Vector2f(windowSize.x * percW, windowSize.y * percH));
	settingsSize = settingsWindow.getSize();
	settingsWindow.setPosition((float)(windowSize.x - settingsSize.x) / 2, (float)(windowSize.y - settingsSize.y) / 2);
	window->draw(overlay);
	window->draw(settingsWindow);

	// Draw title
	title.setScale(scale, scale);
	sf::FloatRect titleBounds = title.getLocalBounds();
	title.setOrigin(titleBounds.left, titleBounds.top);
	title.setPosition((float)windowSize.x / 2 - scale * (float)titleBounds.width / 2, (startY + 10.0 / 720) * windowSize.y);
	window->draw(title);

	// Draw close button
	closeButton.setScale(scale * 1.3, scale);
	sf::FloatRect closeBounds = closeButton.getLocalBounds();
	closeButton.setOrigin(closeBounds.left, closeBounds.top);
	closeButton.setPosition(windowSize.x * endX - closeBounds.width * scale, windowSize.y * startY);
	closeButton.setFillColor(Settings::isInside(mouse, closeButton, closeBounds) ? sf::Color(150, 150, 150) : sf::Color::Black);
	window->draw(closeButton);

	// Draw resolution section
	Settings::displayRes(mouse);	

	// Draw sliders
	movementSlider.draw(mouse);
	sensitivitySlider.draw(mouse);
	fovSlider.draw(mouse);
	visibilitySlider.draw(visibilityCheckbox.getValue() ? mouse : sf::Vector2i(0, 0));

	// Draw subtitles
	Settings::drawTitle("Sensitivity", sensPos);
	Settings::drawTitle("Appearance", appPos);

	// Draw checkbox
	visibilityCheckbox.draw(mouse);

	// Draw labels
	labelMovement.setPosition(windowSize.x * startX, windowSize.y * labelMovementPos);
	labelMouse.setPosition(windowSize.x * startX, windowSize.y * labelMousePos);
	labelFov.setPosition(windowSize.x * startX, windowSize.y * labelFovPos);
	labelEnableShadows.setPosition(windowSize.x * startX, windowSize.y * labelEnableShadowsPos);
	labelDistance.setPosition(windowSize.x * startX, windowSize.y * labelDistancePos);
	labelMovement.setScale(scale, scale);
	labelMouse.setScale(scale, scale);
	labelFov.setScale(scale, scale);
	labelEnableShadows.setScale(scale, scale);
	labelDistance.setScale(scale, scale);
	window->draw(labelMovement);
	window->draw(labelMouse);
	window->draw(labelFov);
	window->draw(labelEnableShadows);
	window->draw(labelDistance);
	//printf("%f\n", movementSlider.getValue());

	// Block shadow field if unchecked
	if (!visibilityCheckbox.getValue()) {
		shadowBlock = sf::RectangleShape(sf::Vector2f(windowSize.x * (endX - startX), labelDistance.getLocalBounds().height * scale - windowSize.y * sliderOffset));
		shadowBlock.setPosition(windowSize.x * startX, windowSize.y * (labelDistancePos + sliderOffset));
		shadowBlock.setFillColor(sf::Color(255, 255, 255, 150));
		window->draw(shadowBlock);
	}

	// Draw extra buttons
	saveButton.draw(mouse, true);
	if (inGame) {
		leaveButton.draw(mouse, true);
		restartButton.draw(mouse, true);
	}

	/* Margin showcase
	sf::RectangleShape first(sf::Vector2f(100, 200));
	first.setPosition(startX * windowSize.x, 0.4 * windowSize.y);
	first.setFillColor(sf::Color::Black);
	sf::RectangleShape second(sf::Vector2f(200, 100));
	second.setPosition(0.5 * windowSize.x, startY * windowSize.y);
	second.setFillColor(sf::Color::Black);
	window->draw(first);
	window->draw(second);*/

	Settings::updateConfig();
}

void Settings::reset() {
	movementSlider.setValue(config->movementSpeed);
	sensitivitySlider.setValue(config->sensitivity);
	fovSlider.setValue(savedFov);
	visibilitySlider.setValue(savedVis);
	visibilityCheckbox.setValue(savedUseVis);
	res = config->res;
	Settings::updateConfig();
}

void Settings::save() {
	config->movementSpeed = movementSlider.getValue();
	config->sensitivity = sensitivitySlider.getValue();
	config->fov = fovSlider.getValue();
	config->visibilityDepth = visibilitySlider.getValue();
	config->res = res;
	savedFov = fovSlider.getValue();
	savedVis = visibilitySlider.getValue();
	savedUseVis = visibilityCheckbox.getValue();

	window->setSize(sf::Vector2u(resolutions[res].w, resolutions[res].h));
	window->setView(sf::View(sf::FloatRect(0, 0, resolutions[res].w, resolutions[res].h)));

	Settings::toggle();
}

bool Settings::visible() {
	return active;
}

void Settings::toggle() {
	active = !active;
	Settings::reset();
}

void Settings::gameToggle() {
	inGame = !inGame;
}

void Settings::mouseClick(sf::Vector2i mouse) {
	if (!settingsWindow.getGlobalBounds().contains(sf::Vector2f(mouse.x, mouse.y))) {
		Settings::toggle();
		return;
	}
	if (Settings::isInside(mouse, closeButton, closeButton.getLocalBounds())) Settings::toggle();
	if (nextResButton.isInside(mouse)) nextRes();
	if (prevResButton.isInside(mouse)) prevRes();
	if (saveButton.isInside(mouse)) Settings::save();
}

void Settings::updateConfig() {
	config->fov = fovSlider.getValue();
	config->visibilityDepth = visibilitySlider.getValue();
	config->useVisibility = visibilityCheckbox.getValue();
}

void Settings::handleEvent(sf::Event event) {
	sf::Vector2i click(event.mouseButton.x, event.mouseButton.y);
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		if (event.mouseButton.button == sf::Mouse::Left)
			Settings::mouseClick(click);
		break;
	}
	movementSlider.handleEvent(event);
	sensitivitySlider.handleEvent(event);
	fovSlider.handleEvent(event);
	visibilityCheckbox.handleEvent(event);
	if (visibilityCheckbox.getValue()) visibilitySlider.handleEvent(event);
}

void Settings::prevRes() {
	if (--res < 0) res = NUM_RES - 1;
}

void Settings::nextRes() {
	if (++res >= NUM_RES) res = 0;
}

bool Settings::isInside(sf::Vector2i mouse, sf::Transformable obj, sf::FloatRect bounds) {
	sf::Vector2f pos = obj.getPosition();
	sf::Vector2f scale = obj.getScale();
	return mouse.x > pos.x && mouse.x < pos.x + bounds.width * scale.x &&
		mouse.y > pos.y && mouse.y < pos.y + bounds.height * scale.y;
}

bool Settings::isInsideLeave(sf::Vector2i mouse) {
	return leaveButton.isInside(mouse);
}

bool Settings::isInsideRestart(sf::Vector2i mouse) {
	return restartButton.isInside(mouse);
}

void Settings::displayRes(sf::Vector2i mouse) {
	Settings::drawTitle("Resolution", resPos);

	sf::Vector2u windowSize = window->getSize();
	resText.setString(std::to_string(resolutions[res].w) + "x" + std::to_string(resolutions[res].h));
	float scale = windowSize.y / 720.0;
	resText.setScale(sf::Vector2f(scale, scale));
	sf::FloatRect bounds = resText.getLocalBounds();
	resText.setOrigin(bounds.left, bounds.top);
	resText.setPosition((float) windowSize.x / 2 - scale * (float) bounds.width / 2, (resPos + 46.0 / 720) * windowSize.y + subtitleHeight * scale);

	window->draw(resText);
	nextResButton.draw(mouse, true);
	prevResButton.draw(mouse, true);
}

void Settings::drawTitle(sf::String title, float percY) {
	sf::Vector2u windowSize = window->getSize();
	subtitle.setString(title);
	sf::FloatRect size = subtitle.getLocalBounds();
	float scale = windowSize.y / 720.0;
	subtitle.setScale(scale, scale);
	subtitle.setPosition(windowSize.x * 0.5 - scale * size.width / 2, windowSize.y * percY);
	window->draw(subtitle);
	
	titleLine.setOutlineThickness(1 * scale);
	float spacing = 10.0 / 1280;
	float lineWidth = (windowSize.x * (endX - startX - 2 * spacing) - scale * size.width) / 2;
	titleLine.setSize(sf::Vector2f(lineWidth, 0));
	titleLine.setPosition(sf::Vector2f(windowSize.x * startX, windowSize.y * (percY + subtitleHeight)));
	window->draw(titleLine);

	titleLine.setPosition(sf::Vector2f(windowSize.x * endX - lineWidth, windowSize.y * (percY + subtitleHeight)));
	window->draw(titleLine);

}

Resolution Settings::getResolution() { return resolutions[res]; }
Configuration* Settings::getConfig() { return config; }

Settings::~Settings() {

}
