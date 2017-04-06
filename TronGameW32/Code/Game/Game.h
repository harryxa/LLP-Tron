#pragma once
#include <vector>
#include "Player.h"
#include <SFML\Network.hpp>
#include <Game/Movement.h>
class Game
{
public:
	Game();
	~Game();
	std::vector<Player*> getPlayers() { return players; };
	Player* getPlayer() {return player;	}
	void createNewPlayer(std::string file);
	void initNewPlayer(int numberofplayers);
	void Game::input(sf::Event* _event);
	MovementType getMoveType();

protected:
	std::atomic<MovementType> moveType = MovementType::IDLE; //Holds the current move state
	MovementType prevType; //Holds the preivous move state
	std::vector<Player*> players;
	Player* player;
};