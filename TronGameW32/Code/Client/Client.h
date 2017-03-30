#pragma once
#include <future>
#include <iostream>
#include <string>
#include "stdafx.h"
#include "Movement.h"

#include <SFML/Network.hpp>
#include <SFML\Graphics.hpp>


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
	void sendPacket(NetMov _mov);
	void draw(sf::RenderWindow& window);


private:
	sf::CircleShape player;
	TcpClient socket;
	NetMov net_mov = NetMov::RIGHT;
	float movement = 0.1f;
	
};