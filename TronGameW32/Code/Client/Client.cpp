#include "Client.h"
#include <Game\MessageTypes.h>
Client::Client()
{
	player.setRadius(10.0f);
	player.setFillColor(sf::Color::Green);
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

//sends input to server and 
void Client::sendPacket(MovementType _mov)
{	
	sf::Packet packet;

	packet << NetMsg::MOVEMENT << _mov;
	socket.send(packet);
}

void Client::draw(sf::RenderWindow & window)
{
	window.draw(player);
}


//this co-ordinates the start up of the client by creating the connection, runs a new thread for recieving messages
//and ends by calling the input function
void Client::client()
{
	/*TcpClient socket;*/
	if (!connect(socket))
	{
		return;
	}

	auto handle = std::async(std::launch::async, [&]
	{
		// keep track of the socket status
		sf::Socket::Status status;

		do
		{
			sf::Packet packet;
			status = socket.receive(packet);
			if (status == sf::Socket::Done)
			{
				
				int header = 0;
				packet >> header;

				//recieves movement packages
				NetMsg msg = static_cast<NetMsg>(header);
				if (msg == NetMsg::MOVEMENT)
				{
					int x = 0;
					packet >> x;
					player.setRadius(x);
				}
				else if (msg == NetMsg::PING)
				{
					sf::Packet pong;
					//sending
					pong << NetMsg::PONG;
					socket.send(pong);
				}
			}
		} 
		while (status != sf::Socket::Disconnected);

	});
}



