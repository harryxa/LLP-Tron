#include "Game.h"
#include <Game\Player.h>

Game::Game()
{
}

Game::~Game()
{
}

void Game::createNewPlayer(std::string file)
{
	player = new Player();
	player->init(file);
	players.push_back(player);
}

void Game::initNewPlayer(int number_of_players)
{
	//sets up 1 player game
	if (number_of_players == 1)
	{
		for (std::vector<Player*>::iterator i = players.begin(); i != players.end(); i++)
		{
			(*i)->DestroyThread();
		}
		players.clear();
		createNewPlayer("..\\..\\Resources\\jake.png");
		getPlayers()[0]->setPosition(sf::Vector2f(15.0f, 15.0f));
		getPlayers()[0]->setPlayerNum(1);
		getPlayers()[0]->getCollider()->setFillColor(sf::Color::Magenta);
	}
	//sets up two player game
	if (number_of_players == 2)
	{
		for (std::vector<Player*>::iterator i = players.begin(); i != players.end(); i++)
		{
			(*i)->DestroyThread();
		}
		players.clear();
		createNewPlayer("..\\..\\Resources\\jake.png");
		getPlayers()[0]->setPosition(sf::Vector2f(15.0f, 15.0f));
		getPlayers()[0]->setPlayerNum(1);
		getPlayers()[0]->getCollider()->setFillColor(sf::Color::Magenta);
		createNewPlayer("..\\..\\Resources\\finn.png");
		getPlayers()[1]->setPosition(sf::Vector2f(710.0f, 510.0f));
		getPlayers()[1]->setPlayerNum(2);
		getPlayers()[1]->getCollider()->setFillColor(sf::Color::Blue);
	}
}

void Game::input(sf::Event* _event)
{
	//cant repeatedly send same info
	prevType = moveType;

	//check so players cannot move back on themselves and checks movement
	if (_event->key.code == sf::Keyboard::W && prevType != MovementType::DOWN)
	{
		moveType = MovementType::UP;
	}
	if (_event->key.code == sf::Keyboard::S && prevType != MovementType::UP)
	{
		moveType = MovementType::DOWN;
	}
	if (_event->key.code == sf::Keyboard::A && prevType != MovementType::RIGHT)
	{
		moveType = MovementType::LEFT;
	}
	if (_event->key.code == sf::Keyboard::D && prevType != MovementType::LEFT)
	{
		moveType = MovementType::RIGHT;
	}
	if (_event->key.code == sf::Keyboard::T && prevType != MovementType::PLAYER2DOWN)
	{
		moveType = MovementType::PLAYER2UP;
	}
	if (_event->key.code == sf::Keyboard::G && prevType != MovementType::PLAYER2UP)
	{
		moveType = MovementType::PLAYER2DOWN;
	}
	if (_event->key.code == sf::Keyboard::F && prevType != MovementType::PLAYER2RIGHT)
	{
		moveType = MovementType::PLAYER2LEFT;
	}
	if (_event->key.code == sf::Keyboard::H && prevType != MovementType::PLAYER2LEFT)
	{
		moveType = MovementType::PLAYER2RIGHT;
	}
}

MovementType Game::getMoveType()
{
	return moveType;
}