#include <SFML/Graphics.hpp>
#include "Client.h"


std::unique_ptr<Client> client;

int main()
{
	client = std::make_unique<Client>();								//creates unique pointer to Client
	sf::RenderWindow window(sf::VideoMode(800, 600), "LLP-TRON!");	//creates window
	sf::CircleShape shape(100.f);										//creates sprite circle
	shape.setFillColor(sf::Color::Green);

	//run a program whilst window is open
	while (window.isOpen())
	{
		//check all events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			//close window
			if (event.type == sf::Event::Closed)
				window.close();
		}
		
		window.clear();
		window.draw(shape);
		window.display();
		client->client();
	}

	return 0;
}