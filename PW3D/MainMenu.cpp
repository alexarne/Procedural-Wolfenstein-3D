#include "MainMenu.h"
#include <iostream>


MainMenu::MainMenu() {
    //if (!font.loadFromFile("dir")) cout << "Font not found!" << endl;

    for (int i = 0; i < MAX_MENU; i++) {
        //menu[i].setFont(font);
        menu[i].setFillColor(sf::Color::White);
        menu[i].setCharacterSize(70);
        menu[1].setPosition(200, 100 + 100 * i);
    }
    menu[0].setString("Start Game");
    menu[1].setString("Help");
    menu[2].setString("Settings");
    menu[3].setString("Quit");
}

MainMenu::~MainMenu() {

}

void MainMenu::draw(sf::RenderWindow& window) {
    for (int i = 0; i < MAX_MENU; i++) window.draw(menu[0]);
}

void MainMenu::reset() {

}