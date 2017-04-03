// ChatServer.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <algorithm>

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <SFML\Network.hpp>
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <Game\Movement.h>
#include <Game\MessageTypes.h>

#include "Client.h"

constexpr int SERVER_TCP_PORT(53000);
constexpr int SERVER_UDP_PORT(53001);

using TcpClient = sf::TcpSocket;
using TcpClientPtr = std::unique_ptr<TcpClient>;
using TcpClients = std::vector<Client>;

bool bindServerPort(sf::TcpListener&);
void clearStaleCli(TcpClients & tcp_clients);//
void connect(sf::TcpListener& tcp_listener, sf::SocketSelector& selector, TcpClients& tcp_clients);
void listen(sf::TcpListener&, sf::SocketSelector&, TcpClients&);
void processMov(sf::Packet &packet, Client & sender, TcpClients & tcp_clients);//
void ping(TcpClients& tcp_clients);//
void runServer();
void recievePlayerMov(TcpClients& tcp_clients, sf::SocketSelector& selector);
void sendPacket(Client & sender, sf::Packet &packet, TcpClients & tcp_clients);

bool bindServerPort(sf::TcpListener& listener)
{
	//binds the server to a network port
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

//this creates a new socket. The client is added to both the selector and the list of all clients connected
void connect(sf::TcpListener& tcp_listener, sf::SocketSelector& selector, TcpClients& tcp_clients)
{
	auto  client_ptr = new sf::TcpSocket;
	auto& client_ref = *client_ptr;
	if (tcp_listener.accept(client_ref) == sf::Socket::Done)
	{
		//client added to selector
		selector.add(client_ref);

		auto client = Client(client_ptr);
		tcp_clients.push_back(std::move(client));
		std::cout << "client connected" << std::endl;

		std::string welcome_msg;
		std::string client_count = std::to_string(tcp_clients.size());
		welcome_msg = "Welcome to Huxy's chat room \n";
		welcome_msg += "There are " + client_count + " connected clients";

		sf::Packet packet;
		packet << NetMsg::CHAT << welcome_msg;
		client_ref.send(packet);
	}
}

void recievePlayerMov(TcpClients& tcp_clients, sf::SocketSelector& selector)
{
	//loops through all clients to find the sender
	for (auto& sender : tcp_clients)
	{
		auto& sender_socket = sender.getSocket();
		if (selector.isReady(sender_socket))
		{
			sf::Packet packet;

			//if clients disconnected remove sender socket
			if (sender_socket.receive(packet) == sf::Socket::Disconnected)
			{
				selector.remove(sender_socket);
				sender_socket.disconnect();
				std::cout << "Client (" << sender.getClientID()
					<< ") Disconnected" << std::endl;
				break;
			}
		
			int type = 0;			
			//recieving packet
			packet >> type;
						
			NetMsg msg = static_cast<NetMsg>(type);
			
			if (msg == NetMsg::MOVEMENT)
			{
				processMov(packet, sender, tcp_clients);
				sendPacket(sender, packet, tcp_clients);
			}
			else if (msg == NetMsg::PONG)
			{
				sender.pong();
			}
		}
	}
}

void processMov(sf::Packet &packet, Client & sender, TcpClients & tcp_clients)
{
	//recieving packet
	int mov_type;	
	packet >> mov_type;

	//printing clients details and movement types
	std::cout << "Client: (" << sender.getClientID() << ") Move type: "
		<< mov_type << std::endl;

	//printing clients latency
	std::cout << "Latency: " << sender.getLatency().count()
		<< "us" << std::endl;


	//impliment movement called from game???

	// send the packet to other clients
	for (auto& client : tcp_clients)
	{
		if (sender == client)
			continue;

		client.getSocket().send(packet);
	}
}

void sendPacket(Client & sender, sf::Packet &packet, TcpClients & tcp_clients)
{

	for (auto& client : tcp_clients)
	{
		if (sender == client)
			continue;
		
		packet << NetMsg::MOVEMENT << 30.0f;
		client.getSocket().send(packet);
	}
}

//endless loop to poll the selector for network activity. eg. connection request or clients sending data
//it is polled overtime as to not block the thread
void listen(sf::TcpListener& tcp_listener, sf::SocketSelector& selector, TcpClients& tcp_clients)
{
	while (true)
	{
		//sf::Time represents a time period
		const sf::Time timeout = sf::Time(sf::milliseconds(250));
		if (selector.wait(timeout))
		{
			//new connection request
			if (selector.isReady(tcp_listener))
			{
				connect(tcp_listener, selector, tcp_clients);
			}
			//recieve message and remove stale clients
			else
			{
				recievePlayerMov(tcp_clients, selector);
				clearStaleCli(tcp_clients);
			}
		}
		else
		{
			ping(tcp_clients);
		}
	}
}

//listener, clients and selector are all l ocal to this function (dont exist in global scope) this means data is passed into the function that needs it
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

void clearStaleCli(TcpClients & tcp_clients)
{
	tcp_clients.erase(
		std::remove_if(tcp_clients.begin(), tcp_clients.end(), [](const Client& client)
	{
		return(!client.isConnected());
	}), tcp_clients.end());
}

int main()
{
	runServer();
	return 0;
}
