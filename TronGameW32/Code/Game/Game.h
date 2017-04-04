#pragma once
#include <vector>
#include "Player.h"
#include <SFML\Network.hpp>
class Game
{
public:

	friend sf::Packet& operator <<(sf::Packet& packet, const Game& game);
	friend sf::Packet& operator >>(sf::Packet& packet, const Game& game);

	std::vector<Player*> getPlayers();
	void createNewPlayer(int numberofplayers);
	Player* getPlayer();
private:
	std::vector<Player*> players;
	Player* player;
};