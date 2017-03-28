#pragma once
#include <SFML/Network.hpp>
enum NetMsg : sf::Int32
{
	INVALID = 0,
	CHAT = 1,
	PING = 2,
	PONG = 3,
	CLIENT_COUNT = 4
};

enum NetMov : sf::Int32
{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3,	
	NONE = 4
};

extern std::atomic<NetMov> net_mov = NetMov::NONE;