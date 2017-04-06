#include <Game\Player.h>

Player::Player()
{
	std::thread thread(&Player::masterMove, this);
	thread.detach();
}

Player::~Player()
{

}

void Player::init(std::string file)
{
	texture.loadFromFile(file);
	sprite.setTexture(texture);
	sprite.setScale(sf::Vector2f(1.0f, 1.0f));
	collider.setSize(sf::Vector2f(0.01f, 0.01f));
	collider.setPosition(sprite.getPosition().x + 45.0f, sprite.getPosition().y + 60.0f);
}

void Player::setPosition(sf::Vector2f _position)
{
	sprite.setPosition(_position);
}
void Player::KillThread()
{
	ThreadAlive = false;
}

//void Player::Draw(sf::RenderWindow & _window)
//{
//	_window.draw(player_sprite);
//}

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

void Player::masterMove()
{
	while (ThreadAlive == true)
	{
		if (moveInt == 0)
		{
			sprite.move(0.00f, -0.0001f);
		}
		if (moveInt == 1)
		{
			sprite.move(0.0f, 0.0001f);
		}
		if (moveInt == 2)
		{
			sprite.move(-0.0001f, 0.0f);
		}
		if (moveInt == 3)
		{
			sprite.move(0.0001f, 0.0f);
		}
	}
}


void Player::setPlayerNum(int Player)
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


