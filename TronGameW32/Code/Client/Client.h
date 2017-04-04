#pragma once
#include <future>
#include <iostream>
#include <string>
#include "stdafx.h"
#include <SFML/Network.hpp>
#include <Game/Game.h>
#include <SFML/Graphics.hpp>
#include <Game/Movement.h>
#include <Game/Game.h>

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
	bool connect(TcpClient&);
	void move();
	void input(sf::Event* _event);
	void sendPacket(MovementType net_mov);
	void runClient();
	void draw();
	void client(std::unique_ptr<Game>& game);


private:
	std::atomic<MovementType> net_mov = MovementType::NONE;
	MovementType previous_mov;
	std::thread movethread;
	TcpClient socket;
	std::unique_ptr<Game> game = std::make_unique<Game>();

	
};