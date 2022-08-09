#pragma once

#include <SFML/Graphics.hpp>
#include <math.h>


class Player
{
public:
	Player(float x, float y);

	void moveForward(float dist);
	void moveBackward(float dist);
	void moveRight(float dist);
	void moveLeft(float dist);
	void turn(float rad);

	sf::Vector2f getPos();
	sf::Vector2f getDir();
	float getAngle();

	~Player();
private:
	float x;
	float y;
	float angle;
	sf::Vector2f dir;
	sf::Vector2f plane;
	void rotate_vec(sf::Vector2f &vec, float rad);
};

