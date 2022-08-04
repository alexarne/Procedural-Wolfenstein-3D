#include "game.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Settings.h"

sf::RenderWindow* window;   // Pointer to the window
Settings* settings;         // Pointer to the settings object
Configuration* config;      // Pointer to the values/configuration to be used

bool running;               // If the game is running
bool settingsVisible;       // If settings are visible
bool prevSettingsVisible;   // If settings were visible last game loop

int game::start(sf::RenderWindow* win, Settings* set) {
    window = win;
    settings = set;
    running = true;
    settingsVisible = false;
    prevSettingsVisible = false;
    config = settings->getConfig();

    window->setMouseCursorVisible(false);
    int value = loop();
    window->setMouseCursorVisible(true);

	return value;
}

int loop() {
    sf::RectangleShape rect(sf::Vector2f(100, 200));
    float x = 0;
    float y = 0;
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
                x += config->sensitivity * (mouse.x - (float) windowSize.x / 2);
                y += config->sensitivity * (mouse.y - (float) windowSize.y / 2);
                sf::Mouse::setPosition(sf::Vector2i(windowSize.x / 2, windowSize.y / 2), *window);
                rect.setPosition(windowSize.x / 2 + x, windowSize.y / 2 + y);
            }
        }
        window->draw(rect);
        printf("move: %f sens: %f fov: %f vis: %f useVis: %s res: %i\n", config->movementSpeed, config->sensitivity, config->fov, config->visibilityDepth, config->useVisibility ? "true" : "false", config->res);
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
            sf::Vector2i click(event.mouseButton.x, event.mouseButton.y);
            switch (event.type) {
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (settings->isInsideLeave(click)) {
                        settings->toggle();
                        return 0;
                    }
                    if (settings->isInsideRestart(click)) printf("Restarting...\n");
                }
                break;
            case sf::Event::Closed:
                window->close();
                return 0;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) settings->toggle();
                break;
            }

            // Specific events (depending on settings visibility)
            if (settingsVisible) {
                settings->handleEvent(event);
            }
            else {

            }
        }
	}
	return 0;
}