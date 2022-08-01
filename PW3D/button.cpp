#include "Button.h"
#include "assets.hpp"
#include <iostream>

Button::Button(sf::RenderWindow* win, sf::String s, float x, float y, float w, float h, int ALIGN) {
    window = win;
    f.loadFromMemory(&font, font_len);
    text.setFont(f);
    text.setString(s);
    text.setCharacterSize(30);

    // Percentage values (0 to 1), based on window size
    percX = x;
    percY = y;
    percW = w;
    percH = h;
    switch (ALIGN)
    {
    case ALIGN_LEFT:
        percX -= percW;
        break;
    case ALIGN_CENTER:
        percX -= percW/2;
        break;
    case ALIGN_RIGHT:
        break;
    }
    this->thickness = 0;

    sf::Vector2u windowSize = window->getSize();
    button = sf::RectangleShape(sf::Vector2f());
    baseColor = sf::Color::Black;
    hoverColor = sf::Color(48, 48, 48);
    textColor = sf::Color::White;
}

void Button::setButtonColor(sf::Color color) { baseColor = color; }
void Button::setHoverColor(sf::Color color) { hoverColor = color; }
void Button::setBorder(float thickness) { this->thickness = thickness; }
void Button::setTextColor(sf::Color color) { textColor = color; }

void Button::draw(sf::Vector2i mouse, bool canHover) {
    sf::Vector2u windowSize = window->getSize();
    float x = windowSize.x * percX;
    float y = windowSize.y * percY;
    float w = windowSize.x * percW;
    float h = windowSize.y * percH;
    button.setSize(sf::Vector2f(w, h));
    button.setPosition(x, y);

    bool isHover = Button::isInside(mouse) && canHover;
    button.setFillColor(isHover ? hoverColor : baseColor);
    button.setOutlineColor(baseColor);
    button.setOutlineThickness(windowSize.y * thickness);

    text.setFillColor(sf::Color::White);
    float scale = (float) windowSize.y / 720;
    text.setScale(sf::Vector2f(scale, scale));
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left, textBounds.top);
    text.setPosition(x + w / 2 - scale * textBounds.width / 2, y + h / 2 - scale * textBounds.height / 2);

    window->draw(button);
    window->draw(text);

}

bool Button::isInside(sf::Vector2i mouse) {
    return button.getGlobalBounds().contains(sf::Vector2f(mouse.x, mouse.y));
}

Button::~Button() {

}