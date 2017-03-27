#pragma once
#include "stdafx.h"
#include <algorithm>

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <SFML\Network.hpp>
#include <SFML\System.hpp>

#include "MessageTypes.h"
#include "Client.h"

constexpr int SERVER_TCP_PORT(53000);
constexpr int SERVER_UDP_PORT(53001);

using TcpClient = sf::TcpSocket;
using TcpClientPtr = std::unique_ptr<TcpClient>;
using TcpClients = std::vector<Client>;

class TronServer {

public:
bool bindServerPort(sf::TcpListener&);
void clearStaleCli(TcpClients & tcp_clients);//
void connect(sf::TcpListener& tcp_listener, sf::SocketSelector& selector, TcpClients& tcp_clients);
void listen(sf::TcpListener&, sf::SocketSelector&, TcpClients&);
void processChatMsg(sf::Packet &packet, Client & sender, TcpClients & tcp_clients);//
void ping(TcpClients& tcp_clients);//
void receiveMsg(TcpClients& tcp_clients, sf::SocketSelector& selector);
void runServer();
sf::Thread thread(&TronServerW32::runServer);

private:
};

