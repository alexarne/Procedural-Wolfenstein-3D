#include "button.h"

button::button(sf::String dir, float x, float y, float w, float h) {
    percX = x;
    percY = y;
    percW = w;
    percH = h;

    texture.loadFromFile(dir);
    texture.setSmooth(true);
    texture_size = texture.getSize();
    sprite.setTexture(texture);
}

void button::draw(sf::RenderWindow &window, sf::Vector2i mouse) {
    // Percentage values (0 to 1), based on window size
    int x = window.getSize().x * percX;
    int y = window.getSize().y * percY;
    int w = window.getSize().x * percW;
    int h = window.getSize().y * percH;
    // Scale button
    sprite.setScale((float) 2 * w / texture_size.x, (float) h / texture_size.y);
    // Update sprite size through isHover
    bool isHover = button::isHover(mouse);
    sprite.setTextureRect(sf::IntRect(isHover ? texture_size.x / 2 : 0, 0, texture_size.x / 2, texture_size.y));
    sprite.setPosition(x - sprite_size.x / 2, h);

    window.draw(sprite);
}

bool button::isHover(sf::Vector2i mouse) {
    sprite_size = sf::Vector2f(
        texture_size.x/2 * sprite.getScale().x,
        texture_size.y   * sprite.getScale().y
    );
    return mouse.x > sprite.getPosition().x && mouse.x < sprite.getPosition().x + sprite_size.x &&
           mouse.y > sprite.getPosition().y && mouse.y < sprite.getPosition().y + sprite_size.y;
}

button::~button() {

}