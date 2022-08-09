#include "Player.h"

Player::Player(float x, float y) {
	this->x = x;
	this->y = y;
	angle = 1.57079632679;		// PI / 2
	dir = sf::Vector2f(0, 1);
}

Player::~Player() {

}

void Player::moveForward(float dist) {
	y -= dist;
}

void Player::moveBackward(float dist) {
	y += dist;
}

void Player::moveRight(float dist) {
	x += dist;
}

void Player::moveLeft(float dist) {
	x -= dist;
}

void Player::turn(float rad) {
	angle -= rad;
	rotate_vec(dir, -rad);
	printf("new dir: %f %f\n", dir.x, dir.y);
}

sf::Vector2f Player::getPos() {
	return sf::Vector2f(x, y);
}

sf::Vector2f Player::getDir() {
	return dir;
}

float Player::getAngle() {
	return angle;
}

void Player::rotate_vec(sf::Vector2f &vec, float rad) {
	float x = vec.x * cos(rad) - vec.y * sin(rad);
	vec.y = vec.x * sin(rad) + vec.y * cos(rad);
	vec.x = x;
}
