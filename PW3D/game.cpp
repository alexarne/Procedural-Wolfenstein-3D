#include "game.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "settings.h"

bool running;               // If the game is running
bool settingsVisible;       // If settings are visible
bool prevSettingsVisible;   // If settings were visible last game loop
Config configuration;       // The settings/configuration to be used

int game::start(sf::RenderWindow &window, settings& settings) {
    running = true;
    settingsVisible = false;
    prevSettingsVisible = false;
    configuration = settings.getConfig();

    window.setMouseCursorVisible(false);
    int value = loop(window, settings);
    window.setMouseCursorVisible(true);

	return value;
}

int loop(sf::RenderWindow& window, settings& settings) {
    sf::RectangleShape rect(sf::Vector2f(100, 200));
    int x = 0;
    int y = 0;
    sf::Vector2u windowSize = window.getSize();
    sf::Mouse::setPosition(sf::Vector2i(windowSize.x / 2, windowSize.y / 2), window);
    rect.setFillColor(sf::Color::Blue);

	while (running) {
		window.clear(sf::Color(90, 20, 20));

        prevSettingsVisible = settingsVisible;
        settingsVisible = settings.visible();
        if (prevSettingsVisible != settingsVisible) {
            window.setMouseCursorVisible(settingsVisible);
            if (!settingsVisible) {
                sf::Vector2u windowSize = window.getSize();
                sf::Mouse::setPosition(sf::Vector2i(windowSize.x / 2, windowSize.y / 2), window);
            }
        }
        else {
            if (!settingsVisible) {
                sf::Vector2u windowSize = window.getSize();
                sf::Vector2i mouse = sf::Mouse::getPosition(window);
                x += mouse.x - windowSize.x / 2;
                y += mouse.y - windowSize.y / 2;
                sf::Mouse::setPosition(sf::Vector2i(windowSize.x / 2, windowSize.y / 2), window);
                rect.setPosition(windowSize.x / 2 + x, windowSize.y / 2 + y);
            }
        }
        window.draw(rect);


        settings.draw(window);

        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i click(event.mouseButton.x, event.mouseButton.y);
                if (settingsVisible) {
                    settings.mouseAction(click);
                    settingsVisible = settings.visible();
                    if (!settingsVisible) { // Might have updated settings
                        configuration = settings.getConfig();
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) settings.toggle();
                if (!settingsVisible) {
                    if (event.key.code == sf::Keyboard::B) return 0;
                }
            }
        }
	}
	return 0;
}