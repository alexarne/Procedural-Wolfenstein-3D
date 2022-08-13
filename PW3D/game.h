#pragma once

#include <SFML/Graphics.hpp>
#include "Settings.h"
#include "Map.h"
#include "Player.h"
#include "assets.hpp"
#include <array>

namespace game {
	int start(sf::RenderWindow* win, Settings* set);
}

int loop();
void drawScreen(int w, int h);
void setPixel(sf::Uint8* screen, int x, int y, int w, sf::Color color);
void drawBuffer(sf::Uint8* buffer, int w, int h);