#include "button.h"
#include "assets.hpp"
#include <iostream>

button::button(int id, float x, float y, float h) {
    // Percentage values (0 to 1), based on window size
    percX = x;
    percY = y;
    percH = h;

    texture.loadFromMemory(menu_buttons[id], menu_buttons_len[id]);
    texture.setSmooth(true);
    texture_size = texture.getSize();
    sprite.setTexture(texture);
}

void button::draw(sf::RenderWindow &window, sf::Vector2i mouse, bool canHover) {
    sf::Vector2u windowSize = window.getSize();
    int x = windowSize.x * percX;
    int y = windowSize.y * percY;
    int h = windowSize.y * percH;

    // Scale button and update size and position
    sprite.setScale((float) h / texture_size.y, (float) h / texture_size.y);
    sprite_size = sf::Vector2f(
        texture_size.x / 2 * sprite.getScale().x,
        texture_size.y * sprite.getScale().y
    );
    sprite.setPosition(x - sprite_size.x / 2, y);

    // Check if hovering/on button
    bool isHover = button::isInside(mouse) && canHover;
    sprite.setTextureRect(sf::IntRect(isHover ? texture_size.x / 2 : 0, 0, texture_size.x / 2, texture_size.y));
    
    window.draw(sprite);
}

bool button::isInside(sf::Vector2i mouse) {
    return mouse.x > sprite.getPosition().x && mouse.x < sprite.getPosition().x + sprite_size.x &&
           mouse.y > sprite.getPosition().y && mouse.y < sprite.getPosition().y + sprite_size.y;
}

button::~button() {

}