#include "main.h"

int main() {
    sf::ContextSettings context;
    context.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Procedural Wolfenstein 3D", sf::Style::Titlebar | sf::Style::Close, context);
    window.setKeyRepeatEnabled(false);

    int height = 64, width = 200, margin = 30;
    int startY = 360 - (3 * height + 2 * margin) / 2;
    int characterSize = 30;
    int ALIGNMENT = ALIGN_CENTER;
    Button btn_startgame, btn_settings, btn_quit;
    btn_startgame.init(&window, "Start Game", 0.75, (float)(startY + 0*(height + margin)) / 720, (float) width / 720, (float) height / 720, characterSize, ALIGNMENT);
    btn_settings.init(&window, "Settings", 0.75, (float)(startY + 1*(height + margin)) / 720, (float) width / 720, (float) height / 720, characterSize, ALIGNMENT);
    btn_quit.init(&window, "Quit", 0.75, (float)(startY + 2*(height + margin)) / 720, (float) width / 720, (float) height / 720, characterSize, ALIGNMENT);

    Configuration config;
    Settings settings(&window, &config);
    bool settingsVisible;

    while (window.isOpen()) {
        // Draw updates all window-relative sizes
        window.clear(sf::Color(24, 24, 24, 0));
        settingsVisible = settings.visible();
        
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        btn_startgame.draw(mouse, !settingsVisible);
        btn_settings.draw(mouse, !settingsVisible);
        btn_quit.draw(mouse, !settingsVisible);

        settings.draw(mouse);
        window.display();

        // Then check for mouse action or other event
        sf::Event event;
        while (window.pollEvent(event)) {
            // Universal events (Regardless of settings visibility)
            if (event.type == sf::Event::Closed) window.close();
            /*if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) settings.toggle();
            }*/

            // Specific events (depending on settings visibility)
            if (settingsVisible) {
                settings.handleEvent(event);
            }
            else {
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i click(event.mouseButton.x, event.mouseButton.y);
                    if (btn_startgame.isInside(click)) {
                        settings.gameToggle();
                        int val = Game::start(&window, &settings);
                        while (val == 1) val = Game::start(&window, &settings);     // Return 1 for restart
                        settings.gameToggle();
                    }
                    if (btn_settings.isInside(click)) settings.toggle();
                    if (btn_quit.isInside(click)) window.close();
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) settings.toggle();
    }

    return 0;
}


