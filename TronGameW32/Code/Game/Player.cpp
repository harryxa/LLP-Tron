#include "Player.h"
#include <thread>

Player::Player()
{
	std::thread movethread(&Player::movePlayer, this);
	movethread.detach();
}

void Player::moveLeft()
{
	moveInt = 2;
}

void Player::moveRight()
{
	moveInt = 3;
}

void Player::moveUp()
{
	moveInt = 0;
}

void Player::moveDown()
{
	moveInt = 1;
}

void Player::movePlayer()
{
	while (true)
	{
		if (moveInt == 0)
		{
			sprite.move(0, -0.0001f);
		}
		if (moveInt == 1)
		{
			sprite.move(0, 0.0001f);
		}
		if (moveInt == 2)
		{
			sprite.move(-0.0001f, 0);
		}
		if (moveInt == 3)
		{
			sprite.move(0.0001f, 0);
		}
	}
}

void Player::init(std::string spritefile)
{
	texture.loadFromFile(spritefile);
	sprite.setTexture(texture);
	sprite.setScale(sf::Vector2f(1, 1));
}

void Player::draw(sf::RenderWindow & _window)
{
	_window.draw(sprite);
}