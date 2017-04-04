#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Player
{
public:
	Player();
	~Player() = default;
	void init(std::string sprite);
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void movePlayer();
	void draw(sf::RenderWindow& window);

private:
	int moveInt;
	sf::Texture texture;
	sf::Sprite sprite;

};