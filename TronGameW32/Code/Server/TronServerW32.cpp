// ChatServer.cpp : Defines the entry point for the console application.

#include "stdafx.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <SFML\Network.hpp>
#include <SFML\System.hpp>

#include "Client.h"
#include <Game\MessageTypes.h>
#include <Game\Player.h>
#include <functional>

constexpr int SERVER_TCP_PORT(53000);
constexpr int SERVER_UDP_PORT(53001);

using TcpClient = sf::TcpSocket;
using TcpClientPtr = std::unique_ptr<TcpClient>;
using TcpClients = std::vector<Client>;

NetMsg msg;
// prototypes
bool bindServerPort(sf::TcpListener&);
void clearStaleCli(TcpClients & tcp_clients);
void connect(sf::TcpListener& tcp_listener, sf::SocketSelector& selector, TcpClients& tcp_clients);
void listen(sf::TcpListener&, sf::SocketSelector&, TcpClients&);
void processPlayerMovement(sf::Packet &packet, Client & sender, TcpClients & tcp_clients);
void ping(TcpClients& tcp_clients);
void receiveMsg(TcpClients& tcp_clients, sf::SocketSelector& selector);
void runServer();

// ChatServer.cpp : Defines the entry point for the console application.

void ping(TcpClients& tcp_clients)
{
	constexpr auto timeout = 10s;
	for (auto& client : tcp_clients)
	{
		const auto& timestamp = client.getPingTime();
		const auto  now = std::chrono::steady_clock::now();
		auto delta = now - timestamp;
		if (delta > timeout)
		{
			client.ping();
		}
	}
}

void runServer()
{
	sf::TcpListener tcp_listener;
	if (!bindServerPort(tcp_listener))
	{
		return;
	}

	sf::SocketSelector selector;
	selector.add(tcp_listener);

	TcpClients tcp_clients;
	return listen(tcp_listener, selector, tcp_clients);
}

void listen(sf::TcpListener& tcp_listener, sf::SocketSelector& selector, TcpClients& tcp_clients)
{
	while (true)
	{
		const sf::Time timeout = sf::Time(sf::milliseconds(250));
		if (selector.wait(timeout))
		{
			if (selector.isReady(tcp_listener))
			{
				connect(tcp_listener, selector, tcp_clients);
			}
			else
			{
				receiveMsg(tcp_clients, selector);
				clearStaleCli(tcp_clients);
			}
		}
		else
		{
			//ping(tcp_clients);
		}
	}
}

void connect(sf::TcpListener& tcp_listener, sf::SocketSelector& selector, TcpClients& tcp_clients)
{
	auto  client_ptr = new sf::TcpSocket;
	auto& client_ref = *client_ptr;
	if (tcp_listener.accept(client_ref) == sf::Socket::Done)
	{
		selector.add(client_ref);

		auto client = Client(client_ptr);
		tcp_clients.push_back(std::move(client));
		std::cout << "Client (" << client.getClientID() << ") connected." << std::endl;

		sf::Packet packet;
		packet << NetMsg::NEW_CLIENT << tcp_clients.size();
		client_ref.send(packet);
	}
}

void receiveMsg(TcpClients& tcp_clients, sf::SocketSelector& selector)
{
	for (auto& sender : tcp_clients)
	{
		auto& sender_socket = sender.getSocket();
		if (selector.isReady(sender_socket))
		{
			sf::Packet packet;
			if (sender_socket.receive(packet) == sf::Socket::Disconnected)
			{
				selector.remove(sender_socket);
				sender_socket.disconnect();
				std::cout << "Client (" << sender.getClientID()
					<< ") Disconnected" << std::endl;
				break;
			}

			int header = 0;
			packet >> header;

			msg = static_cast<NetMsg>(header);
			if (msg == NetMsg::MOVEMENT)
			{
				processPlayerMovement(packet, sender, tcp_clients);
			}
			else if (msg == NetMsg::PONG)
			{
				sender.pong();
			}
			else if (msg == NetMsg::CLIENT_COUNT)
			{
				sf::Packet packet2;
				packet2 << NetMsg::CLIENT_COUNT << tcp_clients.size();
				processPlayerMovement(packet2, sender, tcp_clients);
			}
		}
	}
}

void clearStaleCli(TcpClients & tcp_clients)
{
	tcp_clients.erase(
		std::remove_if(tcp_clients.begin(), tcp_clients.end(), [](const Client& client)
	{
		return(!client.isConnected());
	}), tcp_clients.end());
}

void processPlayerMovement(sf::Packet& packet, Client& sender, TcpClients& tcp_clients)
{
	int movement_state = 0;
	packet >> movement_state;

	std::cout << "Client (" << sender.getClientID() << ") movement state: "
		<< movement_state << std::endl;

	std::cout << "Latency: " << sender.getLatency().count()
		<< "us" << std::endl;

	// send the packet to clients
	for (auto& client : tcp_clients)
	{
		if (msg != NetMsg::MOVEMENT &&sender == client)
		{
			continue;
		}
		client.getSocket().send(packet);
	}
}

bool bindServerPort(sf::TcpListener& listener)
{
	if (listener.listen(SERVER_TCP_PORT) != sf::Socket::Done)
	{
		std::cout << "Could not bind server port, is another app using it?";
		std::cout << std::endl << "Port: " << SERVER_TCP_PORT;
		std::cout << std::endl;
		return false;
	}

	std::cout << "Server launched on port: " << SERVER_TCP_PORT << std::endl;;
	std::cout << "Searching for extraterrestrial life..." << std::endl;
	return true;
}

int main()
{
	runServer();
	return 0;
}
