#pragma once

#include <SFML/Graphics.hpp>
#include "Settings.h"

namespace game {
	int start(sf::RenderWindow* win, Settings* set);
}

int loop();
