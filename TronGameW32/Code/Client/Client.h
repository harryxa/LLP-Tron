#pragma once
#include <future>
#include <iostream>
#include <string>
#include "stdafx.h"
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
	void input(TcpClient&);
	void draw(sf::RenderWindow& window);
	void update(float deltaTime);

private:
	sf::CircleShape player;
	//std::atomic<NetMov> net_mov = NetMov::NONE;

	float movement = 0.1f;
	
};