#pragma once
#include <future>
#include <iostream>
#include <string>
#include "stdafx.h"
#include <SFML/Network.hpp>
#include "MessageTypes.h"

using TcpClient = sf::TcpSocket;
using TcpClientPtr = std::unique_ptr<TcpClient>;
using TcpClients = std::vector<TcpClientPtr>;

const sf::IpAddress SERVER_IP("127.0.0.1");
constexpr int SERVER_TCP_PORT(53000);
constexpr int SERVER_UDP_PORT(53001);


class Client 
{
public:
	void client();
	bool connect(TcpClient&);
	void input(TcpClient&);
}