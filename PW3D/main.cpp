#include "main.h"
#include "MainMenu.h"

#include <SFML/Graphics.hpp>
#include <iostream>

// To remove console, go to Project -> Properties -> Linker -> System and set SubSystem to "Windows (/SUBSYSTEM:WINDOWS)" (default: "Console (/SUBSYSTEM:CONSOLE)")

using std::cout;
using std::endl;

//void resizeWindow(sf::RenderWindow &window, int newWidth, int newHeight);

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Procedural Wolfenstein 3D", sf::Style::Titlebar | sf::Style::Close);
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    //resize(window, 500, 500);
    MainMenu mainMenu;

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
        font.loadFromFile("AlexandriaFLF.ttf");
        text.setFont(font);
        text.setString("loler");
        text.setPosition(100, 100);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(70);
        window.draw(text);

        sf::Texture texture;
        texture.loadFromFile("test.png");
        texture.setSmooth(true);
        sf::Sprite sprite;
        sprite.setTexture(texture);
        window.draw(sprite);
        //texture.update(window);

        window.display();
    }

    return 0;
}

void resize(sf::RenderWindow &window, int newWidth, int newHeight) {
    window.setSize(sf::Vector2u(newWidth, newHeight));
}


