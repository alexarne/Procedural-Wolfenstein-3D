#include "Assets.hpp"

sf::Color Assets::getMapColor(int tile) {
	if (tile > NUM_TEXTURES) return sf::Color::Magenta;
	if (tile == 0) return sf::Color::White;
	return MAP_COLORS[tile - 1];
}
int checked = 0;
sf::Color Assets::getTextureColor(int tile, int x, int y, bool shadow) {
	if (tile-- > NUM_TEXTURES || tile < 0) tile = 0;
	x += (tile % TEXTURES_PER_ROW) * 2 * TEXTURE_WIDTH + (shadow ? TEXTURE_WIDTH : 0);
	y += (tile / TEXTURES_PER_ROW) * TEXTURE_HEIGHT;
	int index = (TEXTURES_PER_ROW * TEXTURE_WIDTH * 2 * y + x) * 3;
	return sf::Color(
		textures[index],
		textures[index + 1],
		textures[index + 2]
	);
}