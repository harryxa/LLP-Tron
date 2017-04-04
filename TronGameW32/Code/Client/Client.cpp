#include "Client.h"
#include <Game\MessageTypes.h>
Client::Client()
{
	std::thread movethread(&Client::move, this);
	movethread.detach();
}

bool Client::connect(TcpClient& socket)
{
	//attempt to connect to server
	auto status = socket.connect(SERVER_IP, SERVER_TCP_PORT);// arguements are IpAddress and then the port to connect to
	if (status != sf::Socket::Done)
	{
		return false;
	}

	//std::cout << "Connected to server: " << SERVER_IP << std::endl;
	socket.setBlocking(false);
	return true;
}

void Client::move()
{
	while (true)
	{

	}
}

//main client "game-loop" this waits for input
void Client::input(sf::Event* pEvent)
{	
	previous_mov = net_mov;

	if (pEvent->key.code == sf::Keyboard::W)
	{
		net_mov = MovementType::UP;
	}
	else if (pEvent->key.code == sf::Keyboard::A)
	{
		net_mov = MovementType::LEFT;
	}
	else if (pEvent->key.code == sf::Keyboard::S)
	{
		net_mov = MovementType::DOWN;
	}
	else if (pEvent->key.code == sf::Keyboard::D)
	{
		net_mov = MovementType::RIGHT;
	}

	if (previous_mov != net_mov)
	{
		sendPacket(net_mov);
	}
}

void Client::sendPacket(MovementType _mov)
{	



	sf::Packet packet;
	packet << NetMsg::MOVEMENT << _mov;
	socket.send(packet);



}





//this co-ordinates the start up of the client by creating the connection, runs a new thread for recieving messages
//and ends by calling the input function
void Client::client(std::unique_ptr<Game>& game)
{
	if (!connect(socket))
	{
		return;
	}


	sf::Packet packet;

	packet << NetMsg::CLIENT_COUNT;

	socket.send(packet);
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
					int player_count = 0;
					packet >> player_count;
					game->createNewPlayer(player_count);

				}

				if (msg == NetMsg::MOVEMENT)
				{
					int move_state;
					packet >> move_state;
					if (move_state == 0 || move_state == MovementType::UP)
					{
						game->getPlayer()->moveUp();
					}
					if (move_state == MovementType::LEFT)
					{
						game->getPlayer()->moveDown();
					}
					if (move_state == MovementType::DOWN)
					{
						game->getPlayer()->moveLeft();
					}
					if (move_state == MovementType::RIGHT)
					{
						game->getPlayer()->moveRight();
					}
				}
				else if (msg == NEW_CLIENT)
				{


				}
				else if (msg == NetMsg::PONG)
				{
				}
			}

		} while (status != sf::Socket::Disconnected);

	});
}

void Client::runClient()
{
	std::thread thread(&Client::draw, this);
	client(game);
	draw();
	thread.join();
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
				input(&event);
			}
		}

		window.clear();
		for (auto& player : game->getPlayers())
		{
			player->draw(window);
		}
		window.display();
	}
}



