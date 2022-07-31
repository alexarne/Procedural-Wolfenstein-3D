#include "game.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Settings.h"

sf::RenderWindow* window;   // Pointer to the window
Settings* settings;         // Pointer to the settings object

bool running;               // If the game is running
bool settingsVisible;       // If settings are visible
bool prevSettingsVisible;   // If settings were visible last game loop
Config configuration;       // The settings/configuration to be used

int game::start(sf::RenderWindow* win, Settings* set) {
    window = win;
    settings = set;
    running = true;
    settingsVisible = false;
    prevSettingsVisible = false;
    configuration = settings->getConfig();

    window->setMouseCursorVisible(false);
    int value = loop();
    window->setMouseCursorVisible(true);

	return value;
}

int loop() {
    sf::RectangleShape rect(sf::Vector2f(100, 200));
    int x = 0;
    int y = 0;
    sf::Vector2u windowSize = window->getSize();
    sf::Mouse::setPosition(sf::Vector2i(windowSize.x / 2, windowSize.y / 2), *window);
    rect.setFillColor(sf::Color::Blue);

	while (running) {
		window->clear(sf::Color(90, 20, 20));

        prevSettingsVisible = settingsVisible;
        settingsVisible = settings->visible();

        sf::Vector2i mouse = sf::Mouse::getPosition(*window);

        // Get mouse movement information
        if (prevSettingsVisible != settingsVisible) {
            window->setMouseCursorVisible(settingsVisible);
            if (!settingsVisible) {
                sf::Vector2u windowSize = window->getSize();
                sf::Mouse::setPosition(sf::Vector2i(windowSize.x / 2, windowSize.y / 2), *window);
            }
        }
        else {
            if (!settingsVisible) {
                sf::Vector2u windowSize = window->getSize();
                x += mouse.x - windowSize.x / 2;
                y += mouse.y - windowSize.y / 2;
                sf::Mouse::setPosition(sf::Vector2i(windowSize.x / 2, windowSize.y / 2), *window);
                rect.setPosition(windowSize.x / 2 + x, windowSize.y / 2 + y);
            }
        }
        window->draw(rect);

        // Get player movement information
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D));

        settings->draw(mouse);
        window->display();

        sf::Event event;
        while (window->pollEvent(event)) {
            // Universal events (Regardless of settings visibility)
            if (event.type == sf::Event::Closed) {
                window->close();
                return 0;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) settings->toggle();
            }

            // Specific events (depending on settings visibility)
            if (settingsVisible) {
                settings->handleEvent(event);
            }
            else {
                if (event.key.code == sf::Keyboard::B) return 0;
            }
        }
	}
	return 0;
}