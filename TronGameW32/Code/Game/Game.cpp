#include "Game.h"
#include <Game\Player.h>

Game::Game()
{
}

Game::~Game()
{
}

//std::vector<Player*> Game::getPlayers()
//{
//	return players;
//}
//
//Player* Game::getPlayer()
//{
//	return player;
//}

void Game::createNewPlayer(std::string file)
{
	player = new Player();
	player->init(file);
	players.push_back(player);
}

void Game::initNewPlayer(int numberofplayers)
{
	if (numberofplayers == 1)
	{
		for (std::vector<Player*>::iterator i = players.begin(); i != players.end(); i++)
		{
			(*i)->KillThread();
		}
		players.clear();
		createNewPlayer("..\\..\\Resources\\jake.png");
		getPlayers()[0]->setPosition(sf::Vector2f(10.0f, 10.0f));
		getPlayers()[0]->setPlayerNum(1);
		getPlayers()[0]->getCollider()->setFillColor(sf::Color::Yellow);
	}
	if (numberofplayers == 2)
	{
		for (std::vector<Player*>::iterator i = players.begin(); i != players.end(); i++)
		{
			(*i)->KillThread();
		}
		players.clear();
		createNewPlayer("..\\..\\Resources\\jake.png");
		getPlayers()[0]->setPosition(sf::Vector2f(10.0f, 10.0f));
		getPlayers()[0]->setPlayerNum(1);
		getPlayers()[0]->getCollider()->setFillColor(sf::Color::Yellow);
		createNewPlayer("..\\..\\Resources\\finn.png");
		getPlayers()[1]->setPosition(sf::Vector2f(710.0f, 510.0f));
		getPlayers()[1]->setPlayerNum(2);
		getPlayers()[1]->getCollider()->setFillColor(sf::Color::Blue);
	}
}

void Game::input(sf::Event* _event)
{
	/*Stores the previous movement state so that
	the player isn't sending unecessary packets
	of the same type to the server. */
	prevType = moveType;

	//Sets the movement state based on WASD input
	//if (player->getPlayerNum() == 1)
	//{
	if (_event->key.code == sf::Keyboard::W)
	{
		moveType = MovementType::UP;
	}
	if (_event->key.code == sf::Keyboard::S)
	{
		moveType = MovementType::DOWN;
	}
	if (_event->key.code == sf::Keyboard::A)
	{
		moveType = MovementType::LEFT;
	}
	if (_event->key.code == sf::Keyboard::D)
	{
		moveType = MovementType::RIGHT;
	}
	if (_event->key.code == sf::Keyboard::T)
	{
		moveType = MovementType::PLAYER2UP;
	}
	if (_event->key.code == sf::Keyboard::G)
	{
		moveType = MovementType::PLAYER2DOWN;
	}
	if (_event->key.code == sf::Keyboard::F)
	{
		moveType = MovementType::PLAYER2LEFT;
	}
	if (_event->key.code == sf::Keyboard::H)
	{
		moveType = MovementType::PLAYER2RIGHT;
	}
}

MovementType Game::getMoveType()
{
	return moveType;
}