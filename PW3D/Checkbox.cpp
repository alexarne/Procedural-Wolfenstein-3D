#include "Checkbox.h"


Checkbox::Checkbox(sf::RenderWindow* window, float percX, float percY, bool checked, int ALIGN) {
	this->window = window;
	this->percX = percX;
	this->percY = percY;
	switch (ALIGN)
	{
	case ALIGN_LEFT:
		percX -= 7 / 1280.0;
		break;
	case ALIGN_CENTER:
		percX -= 7 / 2560.0;
		break;
	case ALIGN_RIGHT:
		break;
	}
	this->checked = checked;
	fillColor = sf::Color(74, 191, 46);
	hoverColor = sf::Color(0, 0, 0, 80);

	box = sf::CircleShape(4);
	box.setOutlineThickness(1.5);
	box.setOutlineColor(sf::Color::Black);
}

Checkbox::Checkbox() {}

void Checkbox::draw(sf::Vector2i mouse) {
	sf::Vector2u windowSize = window->getSize();
	float scale = windowSize.x / 720.0;
	box.setScale(sf::Vector2f(scale, scale));
	bool hover = Checkbox::isInside(mouse);
	box.setFillColor(checked ? fillColor : (hover ? hoverColor : sf::Color(0, 0, 0, 50)));
	box.setPosition(windowSize.x * percX, windowSize.y * percY);

	window->draw(box);
}

void Checkbox::handleEvent(sf::Event event) {
	sf::Vector2i click(event.mouseButton.x, event.mouseButton.y);
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		if (event.mouseButton.button == sf::Mouse::Left)
			if (isInside(click)) checked = !checked;
		break;
	}
}

void Checkbox::setValue(bool value) {
	checked = value;
}

bool Checkbox::getValue() { return checked; }

bool Checkbox::isInside(sf::Vector2i mouse) {
	return box.getGlobalBounds().contains(sf::Vector2f(mouse.x, mouse.y));
}

Checkbox::~Checkbox() {

}