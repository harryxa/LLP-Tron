#include "Game.h"

std::vector<Player*> Game::getPlayers()
{
	return players;
}

void Game::createNewPlayer(int numberofplayers)
{
	player = new Player();
	player->init("..\\..\\Resources\\jake.png");
	players.push_back(player);
}

Player * Game::getPlayer()
{
	return player;
}
