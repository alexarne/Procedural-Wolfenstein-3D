#include "main.h"
#include "MainMenu.h"
#include "button.h"
#include "game.h"
#include "Settings.h"

#include <iostream>

using std::cout;
using std::endl;
using std::to_string;

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Procedural Wolfenstein 3D", sf::Style::Titlebar | sf::Style::Close);
    window.setKeyRepeatEnabled(false);

    int height = 64, margin = 30;
    int startY = 360 - (3 * height + 2 * margin) / 2;
    button btn_startgame(&window, 0, 0.75, (float)(startY + 0*(height + margin)) / 720, (float) height/720);
    button btn_settings(&window, 1, 0.75, (float)(startY + 1*(height + margin)) / 720, (float) height/720);
    button btn_exit(&window, 2, 0.75, (float)(startY + 2*(height + margin)) / 720, (float) height/720);

    Settings settings(&window);
    bool settingsVisible;

    while (window.isOpen()) {
        // Draw updates all window-relative sizes
        window.clear(sf::Color(24, 24, 24, 0));
        settingsVisible = settings.visible();

        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        btn_startgame.draw(mouse, !settingsVisible);
        btn_settings.draw(mouse, !settingsVisible);
        btn_exit.draw(mouse, !settingsVisible);

        settings.draw(mouse);
        window.display();

        // Then check for mouse action or other event
        sf::Event event;
        while (window.pollEvent(event)) {
            // Universal events (Regardless of settings visibility)
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) settings.toggle();
            }

            // Specific events (depending on settings visibility)
            if (settingsVisible) {
                settings.handleEvent(event);
            }
            else {
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i click(event.mouseButton.x, event.mouseButton.y);
                    if (btn_startgame.isInside(click)) game::start(&window, &settings);
                    if (btn_settings.isInside(click)) settings.toggle();
                    if (btn_exit.isInside(click)) window.close();
                }
            }
        }
    }
    /*
    MainMenu mainMenu;
    //resize(window, 500, 500);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Red);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(24, 24, 24, 0));
        window.draw(shape);
        mainMenu.draw(window);

        sf::Text text;
        sf::Font font;
        //font.loadFromFile("AlexandriaFLF.ttf");
        font.loadFromMemory(&AlexandriaFLF_ttf, AlexandriaFLF_ttf_len);
        text.setFont(font);
        text.setString("loler");
        text.setPosition(100, 100);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(70);
        window.draw(text);

        sf::Texture texture;
        texture.loadFromMemory(&test, test_len);
        texture.setSmooth(true);
        sf::Sprite sprite;
        sprite.setTexture(texture);
        window.draw(sprite);
        //texture.update(window);

        sf::Vector2i position = sf::Mouse::getPosition(window);
        cout << to_string(position.x) << " " << to_string(position.y) << endl;
        //sf::Mouse::setPosition(sf::Vector2i(100, 200), window);

        sf::Image image;
        image.loadFromMemory(&test, test_len);
        if (position.x > 0 && position.x < image.getSize().x && position.y > 0 && position.y < image.getSize().y) {
            sf::Color pixel = image.getPixel(position.x, position.y);
            shape.setFillColor(pixel);
        }
        window.draw(shape);
        //cout << to_string(pixel.r) << " " << to_string(pixel.g) << " " << to_string(pixel.b) << endl;

        window.display();
    }*/

    return 0;
}

void resize(sf::RenderWindow &window, int newWidth, int newHeight) {
    window.setSize(sf::Vector2u(newWidth, newHeight));
}


