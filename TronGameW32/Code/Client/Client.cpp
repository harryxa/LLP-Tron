#include "Client.h"


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
void Client::input(TcpClient &socket)
{
	while (false)
	{
		std::string input;
		//std::getline(std::cin, input);


		//creates a packet and sends it to the server
		sf::Packet packet;
		//sednign packet
		packet << NetMsg::CHAT << input;
		socket.send(packet);
	}
}

//this co-ordinates the start up of the client by creating the connection, runs a new thread for recieving messages
//and ends by calling the input function
void Client::client()
{
	TcpClient socket;
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
					std::string str;
					//recieving
					packet >> str;
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
		} while (status != sf::Socket::Disconnected);

	});

	return input(socket);
}



