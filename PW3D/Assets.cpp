#include "Assets.hpp"

sf::Color Assets::getMapColor(int tile) {
	if (tile > NUM_TEXTURES) return sf::Color::Magenta;
	if (tile == 0) return sf::Color::White;
	return MAP_COLORS[tile - 1];
}

sf::Vector2f Assets::getTextureCoords(int tile, float x, float y, bool shadow) {
	if (tile-- > NUM_TEXTURES || tile < 0) tile = 0;
	sf::Vector2i texture_tile(
		tile * TEXTURE_WIDTH * 2 % (TEXTURES_PER_ROW * TEXTURE_WIDTH * 2) + (shadow ? TEXTURE_WIDTH : 0),
		(tile / TEXTURES_PER_ROW) * TEXTURE_HEIGHT
	);
	x += texture_tile.x;
	y += texture_tile.y;
	return sf::Vector2f((float) x + 0.5, (float) y);	// + 0.5 because otherwise neighbouring pixels might be interpreted as the same pixel (but I don't know why)
}

sf::Color Assets::getTextureColor(int tile, float x, float y, bool shadow) {
	sf::Vector2f coords = Assets::getTextureCoords(tile, x, y, shadow);
	if (tile-- > NUM_TEXTURES || tile < 0) tile = 0;
	int index = (TEXTURES_PER_ROW * TEXTURE_WIDTH * 2 * coords.y + coords.x) * 3;
	return sf::Color(
		textures[index],
		textures[index + 1],
		textures[index + 2]
	);
}