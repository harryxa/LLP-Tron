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
	~Client();
	void runClient();
	void input(sf::Event* _event); //Checks which key is pressed
	void draw();//used to draw
	void createGrid();//creates tile grids
	void client(std::unique_ptr<Game>& game); //Connects to the server
	bool connect(TcpClient&); //connection
	void sendPacket(MovementType _state); //Sends a packet

private:
	std::atomic<MovementType> move_state = MovementType::IDLE; //current move
	MovementType previous_state; //preivous move
	std::unique_ptr<Game> m_game = std::make_unique<Game>();
	std::vector<sf::RectangleShape> tiles;
	int amountOfClients;
	std::thread movement;
	TcpClient socket; // Socket for the client

	
};