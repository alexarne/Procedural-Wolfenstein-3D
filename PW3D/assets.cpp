#include "assets.hpp"

sf::Color getMapColor(int tile) {
	if (tile > NUM_TEXTURES) return sf::Color::Magenta;
	if (tile == 0) return sf::Color::White;
	return MAP_COLORS[tile - 1];
}

sf::Color getTextureColor(int id, int x, int y) {
	return sf::Color(
		textures[0][TEXTURE_WIDTH * y + x],
		textures[0][TEXTURE_WIDTH * y + x + 1],
		textures[0][TEXTURE_WIDTH * y + x + 2],
		textures[0][TEXTURE_WIDTH * y + x + 3]
	);
}

const unsigned char* textures[] = {
	BLUE_STONE,
	COLOR_STONE,
	EAGLE,
	GREY_STONE,
	MOSSY_STONE,
	PURPLE_STONE,
	RED_BRICKS,
	WOOD
};

const unsigned int textures_len[] = {
	BLUE_STONE_len,
	COLOR_STONE_len,
	EAGLE_len,
	GREY_STONE_len,
	MOSSY_STONE_len,
	PURPLE_STONE_len,
	RED_BRICKS_len,
	WOOD_len
};