#include "Assets.hpp"

sf::Color Assets::getMapColor(int tile) {
	if (tile > NUM_TEXTURES) return sf::Color::Magenta;
	if (tile == 0) return sf::Color::White;
	return MAP_COLORS[tile - 1];
}

sf::Color Assets::getTextureColor(int tile, int x, int y) {
	if (tile-- > NUM_TEXTURES || tile < 0) tile = 0;
	int index = (TEXTURE_WIDTH * y + x) * 4;
	return sf::Color(
		textures[tile][index],
		textures[tile][index + 1],
		textures[tile][index + 2],
		textures[tile][index + 3]
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