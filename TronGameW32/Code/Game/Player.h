#pragma once

#include <SFML\Graphics.hpp>
#include <string>
#include <atomic>
#include <thread>

class Player : public sf::Sprite
{
public:
	Player();
	~Player();
	
	void init(std::string file);

	sf::Sprite getSprite();
	sf::RectangleShape* getCollider();

	void Move();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	
	void setPosition(sf::Vector2f pos);
	void setPlayer(int Player);
	int getPlayerNum();
	

	void DestroyThread();

private:
	int playerNum = 0;
	bool ThreadAlive = true;
	std::atomic<int> moveInt;

	sf::Vector2f position = sf::Vector2f(0, 0);
	sf::Texture texture;
	sf::Sprite sprite;
	sf::RectangleShape collider;
};