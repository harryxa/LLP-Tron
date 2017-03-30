#pragma once
#include <future>
#include <iostream>
#include <string>
#include "stdafx.h"
#include <SFML/Network.hpp>
#include <SFML\Graphics.hpp>

#include <Game\Movement.h>

using TcpClient = sf::TcpSocket;
using TcpClientPtr = std::unique_ptr<TcpClient>;
using TcpClients = std::vector<TcpClientPtr>;

const sf::IpAddress SERVER_IP("127.0.0.1");
constexpr int SERVER_TCP_PORT(53000);
constexpr int SERVER_UDP_PORT(53001);


class Client 
{
public:
	Client();
	~Client() = default;
	void client();
	bool connect(TcpClient&);
	void input(sf::Event* pEvent);
	void sendPacket(MovementType _mov);
	void draw(sf::RenderWindow& window);


private:
	sf::CircleShape player;
	TcpClient socket;
	MovementType net_mov = MovementType::RIGHT;
	MovementType previous_mov;
	float movement = 0.1f;

	
};