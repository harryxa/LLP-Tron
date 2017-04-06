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
	/*void setPosition(sf::Vector2f _position);
	void movePosition(sf::Vector2f _position);*/
	//void Draw(sf::RenderWindow& _window);
	sf::Sprite getSprite();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void masterMove();
	void setPosition(sf::Vector2f pos);
	void KillThread();
	void setPlayerNum(int Player);
	int getPlayerNum();
	sf::RectangleShape* getCollider();

private:
	int playerNum = 0;
	bool ThreadAlive = true;
	std::atomic<int> moveInt;
	sf::Vector2f position = sf::Vector2f(0, 0);
	sf::Texture texture;
	sf::Sprite sprite;
	sf::RectangleShape collider;
};