#pragma once

#include <SFML/Graphics.hpp>
#include "Settings.h"
#include "Map.h"
#include "Player.h"
#include "Assets.hpp"
#include <array>

namespace Game {
	int start(sf::RenderWindow* win, Settings* set);
}

int loop();
void drawScreen(int w, int h);
void setPixel(sf::Uint8* screen, int x, int y, int w, int h, sf::Color color);
<<<<<<< HEAD
void drawBuffer(sf::Uint8* buffer, int w, int h);
=======
void drawBuffer(sf::Uint8* buffer, int w, int h);
>>>>>>> 7abe8dde0b83e18cbc88c66b107642297f1d66b2
