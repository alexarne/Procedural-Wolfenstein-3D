#pragma once

#include <SFML/Graphics.hpp>
#include "settings.h"

namespace game {
	int start(sf::RenderWindow &window, settings &settings);
}

int loop(sf::RenderWindow& window, settings &settings);
