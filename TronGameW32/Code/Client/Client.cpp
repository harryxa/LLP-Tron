#include "Client.h"

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
void Client::input(TcpClient &socket)
{	
	while (true)
	{		
		//creates a packet and sends it to the server
		sf::Packet packet;
		//sending packet
		//if (net_mov == NetMov::DOWN)
		//{
		//	//packet << NetMov::DOWN;
		//}
		//if (net_mov == NetMov::UP)
		//{
		//	//packet << NetMov::UP;
		//}
		//if (net_mov == NetMov::LEFT)
		//{
		//	//packet << NetMov::LEFT;
		//}		
		//if (net_mov == NetMov::RIGHT)
		//{
		//	//packet << NetMov::RIGHT;
		//}		
		socket.send(packet);
	}
}

void Client::draw(sf::RenderWindow & window)
{
	window.draw(player);
}

void Client::update(float deltaTime)
{
	sf::Vector2f moveamount = sf::Vector2f(0, 0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		net_mov = NetMov::LEFT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		net_mov = NetMov::DOWN;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		net_mov = NetMov::RIGHT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		net_mov = NetMov::UP;
	}

	if (net_mov == NetMov::DOWN)
	{
		moveamount.y += movement * deltaTime;
	}
	if (net_mov == NetMov::UP)
	{
		moveamount.y -= movement * deltaTime;
	}
	if (net_mov == NetMov::LEFT)
	{
		moveamount.x -= movement * deltaTime;
	}
	if (net_mov == NetMov::RIGHT)
	{
		moveamount.x += movement * deltaTime;
	}

	player.move(moveamount);
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

	return input(socket);
}



