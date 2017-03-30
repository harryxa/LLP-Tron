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
	if (pEvent->key.code == sf::Keyboard::W)
	{
		net_mov = NetMov::UP;
	}
	else if (pEvent->key.code == sf::Keyboard::S)
	{
		net_mov = NetMov::DOWN;
	}
	else if (pEvent->key.code == sf::Keyboard::A)
	{
		net_mov = NetMov::LEFT;
	}
	else if (pEvent->key.code == sf::Keyboard::D)
	{
		net_mov = NetMov::RIGHT;
	}
	sendPacket(net_mov);
}

void Client::sendPacket(NetMov _mov)
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

				NetMsg msg = static_cast<NetMsg>(header);
				if (msg == NetMsg::CHAT)
				{
					//std::string str;
					//recieving
					//packet >> str;
					//std::cout << "< " << str << std::endl;

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



