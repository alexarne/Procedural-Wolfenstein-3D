#pragma once

#include <SFML/Graphics.hpp>
#include "Settings.h"
#include "Map.h"
#include "Player.h"
#include "assets.hpp"

namespace game {
	int start(sf::RenderWindow* win, Settings* set);
}

int loop();
void drawScreen(int w, int h);
void drawLine(sf::Image& screen, int x, int drawStart, int drawEnd, sf::Color color);