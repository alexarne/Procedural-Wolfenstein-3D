#include "main.h"
#include "MainMenu.h"
#include "assets.h"
#include "button.h"

#include <SFML/Graphics.hpp>
#include <iostream>

// To remove console, go to Project -> Properties -> Linker -> System and set SubSystem to "Windows (/SUBSYSTEM:WINDOWS)" (default: "Console (/SUBSYSTEM:CONSOLE)")

using std::cout;
using std::endl;
using std::to_string;

//void resizeWindow(sf::RenderWindow &window, int newWidth, int newHeight);

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Procedural Wolfenstein 3D", sf::Style::Titlebar | sf::Style::Close);
    
    button btn_startgame("assets/menu_startgame.png", 0.75, 0.2, (float) 256/window.getSize().x, (float) 64/window.getSize().y);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseButtonPressed) printf("Game Started");
        }
        window.clear(sf::Color(24, 24, 24, 0));

        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        btn_startgame.draw(window, mouse);

        window.display();
    }

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
    }

    return 0;
}

void resize(sf::RenderWindow &window, int newWidth, int newHeight) {
    window.setSize(sf::Vector2u(newWidth, newHeight));
}


