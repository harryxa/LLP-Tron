#include <Game\Player.h>

Player::Player()
{
	std::thread thread(&Player::Move, this);
	thread.detach();
}

Player::~Player()
{
}

//initialises player with texture
void Player::init(std::string file)
{
	texture.loadFromFile(file);
	sprite.setTexture(texture);
	sprite.setScale(sf::Vector2f(1.0f, 1.0f));
	collider.setSize(sf::Vector2f(0.01f, 0.01f));
	collider.setPosition(sprite.getPosition().x + 45.0f, sprite.getPosition().y + 60.0f);
}

//set position
void Player::setPosition(sf::Vector2f _position)
{
	sprite.setPosition(_position);
}

void Player::DestroyThread()
{
	ThreadAlive = false;
}

sf::Sprite Player::getSprite()
{
	return sprite;
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

void Player::Move()
{
	while (ThreadAlive == true)
	{
		if (moveInt == 0)
		{
			//move up
			sprite.move(0.00f, -0.00012f);
		}
		if (moveInt == 1)
		{
			//move down
			sprite.move(0.0f, 0.00012f);
		}
		if (moveInt == 2)
		{
			//move left
			sprite.move(-0.00012f, 0.0f);
		}
		if (moveInt == 3)
		{
			//move right
			sprite.move(0.00012f, 0.0f);
		}
	}
}

void Player::setPlayer(int Player)
{
	playerNum = Player;
}

int Player::getPlayerNum()
{
	return playerNum;
}

sf::RectangleShape* Player::getCollider()
{
	return &collider;
}


