#include "Client.h"

#include <Game\MessageTypes.h>

Client::Client()
{
}

Client::~Client()
{
}

void Client::sendPacket(MovementType _state)
{
	//Creates a packet
	sf::Packet packet;


	packet << NetMsg::MOVEMENT << _state;

	//Sends the packet
	socket.send(packet);
}

void Client::client(std::unique_ptr<Game>& game)
{
	//Checks if the player can connect
	if (!connect(socket))
	{
		return;
	}
	sf::Packet packet;
	packet << NetMsg::CLIENT_COUNT;
	socket.send(packet);
	//managerHeader = _manager;
	auto handle = std::async(std::launch::async, [&]
	{		
		sf::Socket::Status status;

		do
		{
			sf::Packet packet;
			status = socket.receive(packet);
			if (status == sf::Socket::Done)
			{
				int header = 0;
				packet >> header;

				NetMsg msg = static_cast<NetMsg>(header);

				if (msg == NetMsg::CLIENT_COUNT)
				{
					int numOfPlayers;
					packet >> numOfPlayers;
					game->initNewPlayer(numOfPlayers);
				}



				if (msg == NetMsg::MOVEMENT)
				{
					int move_state;
					packet >> move_state;
					if (move_state == 0)
					{
						game->getPlayers()[0]->moveUp();
					}
					else if (move_state == 1)
					{
						game->getPlayers()[0]->moveDown();
					}
					else if (move_state == 2)
					{
						game->getPlayers()[0]->moveLeft();
					}
					else if (move_state == 3)
					{
						game->getPlayers()[0]->moveRight();
					}


					if (move_state == 4)
					{
						if (game->getPlayers().size() == 2)
							game->getPlayers()[1]->moveUp();
					}
					else if (move_state == 5)
					{
						if (game->getPlayers().size() == 2)
							game->getPlayers()[1]->moveDown();
					}
					else if (move_state == 6)
					{
						if (game->getPlayers().size() == 2)
							game->getPlayers()[1]->moveLeft();
					}
					else if (move_state == 7)
					{
						if (game->getPlayers().size() == 2)
							game->getPlayers()[1]->moveRight();
					}
				}
				else if (msg == NetMsg::NEW_CLIENT)
				{
					int createdPlayer;
					packet >> createdPlayer;
					if (amountOfClients != 1 || amountOfClients != 2)
					{
						amountOfClients = createdPlayer;
					}
					game->initNewPlayer(createdPlayer);

				}
				else if (msg == NetMsg::PONG)
				{
				}
			}
		} while (status != sf::Socket::Disconnected);

	});
}

bool Client::connect(TcpClient& _socket)
{
	auto status = _socket.connect(SERVER_IP, SERVER_TCP_PORT);

	//If the player failed to connect
	if (status != sf::Socket::Done)
	{
		std::cout << "Error connecting to server:" << SERVER_IP << std::endl;
		return false;
	}

	//If the player managed to connect
	std::cout << "Connected to server: " << SERVER_IP <<
		". on port: " << SERVER_TCP_PORT << "." << std::endl;
	_socket.setBlocking(false);
	return true;
}

void Client::createGrid()
{
	tiles.reserve(900);

	int x = 0;
	int y = 0;

	//for (int i = 0; i < 2025; i++)
	for (int i = 0; i < 8100; i++)
	{
		tiles.push_back(sf::RectangleShape());

		tiles[i].setFillColor(sf::Color::Transparent);
		tiles[i].setSize(sf::Vector2f(10, 10));
		tiles[i].setPosition((i % 90) * 10, (i / 90) * 10);
	}
}

void Client::input(sf::Event* _event)
{

	previous_state = move_state;
	m_game->input(_event);
	move_state = m_game->getMoveType();

	if (previous_state != move_state)
	{
		sendPacket(m_game->getMoveType());
	}
		
}

void Client::draw()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "LLP-TRON!");

	while (window.isOpen())
	{
		
		sf::Event event;

		while (window.pollEvent(event))
		{
			
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			
			if (event.type == sf::Event::KeyPressed)
			{
				
				this->input(&event);
			}
		}
		//Clear the game window
		window.clear();

	/*	for (int i = 0; i < m_game->getPlayers().size; i++)
		{
			for (int i = 0; i < tiles.size(); i++)
			{*/
		for (auto& player : m_game->getPlayers())
		{
			for (auto& tile : tiles)
			{

				if (player->getCollider()->getGlobalBounds().intersects(tile.getGlobalBounds()))
				{
					if (tile.getFillColor() == sf::Color::Transparent)
					{
						tile.setFillColor(player->getCollider()->getFillColor());
					}
					else if (tile.getFillColor() != sf::Color::Transparent)
					{
						//window.close();
					}
				}
				window.draw(tile);
			}
			player->getCollider()->setPosition(player->getSprite().getPosition().x + 45.0f, player->getSprite().getPosition().y + 60.0f);
			window.draw(player->getSprite());
		}
		//player_manager->getPlayer()->Draw(window);
		//client->draw(window);
		window.display();
	}
}

void Client::runClient()
{
	createGrid();
	std::thread thread(&Client::draw, this);
	client(m_game);
	draw();
	thread.join();
}