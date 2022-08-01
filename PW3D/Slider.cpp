#include "Slider.h"



Slider::Slider(sf::RenderWindow* win, float x, float y, float w, float h, float f, float t) {
	window = win;
	percX = x;
	percY = y;
	percW = w;
	percH = h;
	from = f;
	to = t;
	holdingHandle = false;

	sf::Vector2u windowSize = window->getSize();
	background = sf::RectangleShape(sf::Vector2f(windowSize.x * percW, windowSize.y * percH));
	handle = sf::RectangleShape(sf::Vector2f(windowSize.y * percH, windowSize.y * percH));
	background.setPosition(windowSize.x * percX, windowSize.y * percY);
	Slider::updateHandle();
}

Slider::Slider() {}

void Slider::draw(sf::Vector2i mouse) {
	sf::Vector2u windowSize = window->getSize();
	background.setSize(sf::Vector2f(windowSize.x * percW, windowSize.y * percH));
	handle.setSize(sf::Vector2f(windowSize.y * percH, windowSize.y * percH));
	background.setPosition(windowSize.x * percX, windowSize.y * percY);
	bool hover = holdingHandle || (isInside(mouse) && !sf::Mouse::isButtonPressed(sf::Mouse::Left));
	background.setFillColor(hover ? sf::Color::Yellow : sf::Color::Red);
	handle.setFillColor(hover ? sf::Color::Cyan : sf::Color::Blue);
	Slider::updateValue(mouse);
	Slider::updateHandle();
	window->draw(background);
	window->draw(handle);
}

void Slider::handleEvent(sf::Event event) {
	sf::Vector2i click(event.mouseButton.x, event.mouseButton.y);
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		if (event.mouseButton.button == sf::Mouse::Left)
			if (isInside(click)) holdingHandle = true;
		break;
	case sf::Event::MouseButtonReleased:
		if (event.mouseButton.button == sf::Mouse::Left) {
			Slider::updateValue(click);
			holdingHandle = false;
		}
		break;
	}
}

void Slider::updateValue(sf::Vector2i mouse) {
	if (!holdingHandle) return;
	sf::FloatRect handleBounds = handle.getGlobalBounds();
	sf::FloatRect backgroundBounds = background.getGlobalBounds();
	float maxLeft = backgroundBounds.left + handleBounds.width / 2;
	float maxRight = backgroundBounds.left + backgroundBounds.width - handleBounds.width / 2;
	float alpha = (mouse.x - maxLeft)/(maxRight - maxLeft);
	Slider::setValue(alpha * (to - from) + from);
}

void Slider::updateHandle() {
	sf::Vector2u windowSize = window->getSize();
	sf::FloatRect backgroundBounds = background.getGlobalBounds();
	sf::FloatRect handleBounds = handle.getGlobalBounds();
	handle.setPosition(backgroundBounds.left + (backgroundBounds.width - handleBounds.width) * (value - from) / (to - from), backgroundBounds.top);
}

bool Slider::isInside(sf::Vector2i mouse) {
	return background.getGlobalBounds().contains(sf::Vector2f(mouse.x, mouse.y));
}

void Slider::setValue(float value) { 
	this->value = value;
	if (this->value > to) this->value = to;
	if (this->value < from) this->value = from;
}

float Slider::getValue() { return value; }

Slider::~Slider() {

}