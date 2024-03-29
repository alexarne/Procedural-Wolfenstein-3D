#include "Player.h"

Player::Player(float x, float y) {
	this->x = x;
	this->y = y;
	angle = 0;
	dir = sf::Vector2f(0, -1);
	plane = sf::Vector2f(1, 0);
	heightOrigin = 0.5;
}

Player::~Player() {

}

void Player::moveForward(float dist) {
	x += dir.x * dist;
	y += dir.y * dist;
}

void Player::moveBackward(float dist) {
	x -= dir.x * dist;
	y -= dir.y * dist;
}

void Player::moveRight(float dist) {
	x += plane.x * dist;
	y += plane.y * dist;
}

void Player::moveLeft(float dist) {
	x -= plane.x * dist;
	y -= plane.y * dist;
}

void Player::turnRight(float rad) {
	angle += rad;
	rotate_vec(dir, rad);
	rotate_vec(plane, rad);
}

void Player::lookUp(float amt) {
	heightOrigin += amt;
	if (heightOrigin > 1) heightOrigin = 1;
	if (heightOrigin < 0) heightOrigin = 0;
}

sf::Vector2f Player::getPos() {
	return sf::Vector2f(x, y);
}

sf::Vector2f Player::getDir() {
	return dir;
}

sf::Vector2f Player::getPlane(float fov) {
	float factor = tan(fov * 3.14159265359 / 360);
	
	return sf::Vector2f(plane.x * factor, plane.y * factor);
}

float Player::getAngle() {
	return angle;
}

int Player::getHeightOrigin(int h) {
	return heightOrigin * h;
}

void Player::rotate_vec(sf::Vector2f &vec, float rad) {
	float x = vec.x * cos(rad) - vec.y * sin(rad);
	vec.y = vec.x * sin(rad) + vec.y * cos(rad);
	vec.x = x;
}
