#include "Slider.h"



Slider::Slider(sf::RenderWindow* win, float x, float y, float w, float h, float f, float t, float defValue) {
	window = win;
	percX = x;
	percY = y;
	percW = w;
	percH = h;
	from = f;
	to = t;
	value = defValue;
	holdingHandle = false;

	sf::Vector2u windowSize = window->getSize();
	background = sf::RectangleShape(sf::Vector2f(windowSize.x * percW, windowSize.y * percH));
	handle = sf::RectangleShape(sf::Vector2f(windowSize.y * percH, windowSize.y * percH));
	background.setPosition(windowSize.x * percX, windowSize.y * percY);
	Slider::updateHandle();
}

Slider::Slider() {}

void Slider::draw(sf::Vector2i mouse) {
	bool hover = holdingHandle || isInside(mouse, background);
	background.setFillColor(hover ? sf::Color::Yellow : sf::Color::Red);
	handle.setFillColor(hover ? sf::Color::Cyan : sf::Color::Blue);
	Slider::updateValue(mouse);
	window->draw(background);
	window->draw(handle);
}

void Slider::handleEvent(sf::Event event) {
	sf::Vector2i click(event.mouseButton.x, event.mouseButton.y);
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		if (event.mouseButton.button == sf::Mouse::Left)
			if (isInside(click, background)) holdingHandle = true;
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
	value = alpha * (to - from) + from;
	if (value > to) value = to;
	if (value < from) value = from;
	Slider::updateHandle();
}

void Slider::updateHandle() {
	sf::Vector2u windowSize = window->getSize();
	handle.setPosition(windowSize.x * percX + (windowSize.x * percW - windowSize.y * percH) * (value - from) / (to - from), windowSize.y * percY);
}

bool Slider::isInside(sf::Vector2i mouse, sf::RectangleShape rect) {
	sf::FloatRect bounds = rect.getGlobalBounds();
	return mouse.x > bounds.left && mouse.x < bounds.left + bounds.width &&
		mouse.y > bounds.top && mouse.y < bounds.top + bounds.height;
}

int Slider::getValue() { return value; }

Slider::~Slider() {

}