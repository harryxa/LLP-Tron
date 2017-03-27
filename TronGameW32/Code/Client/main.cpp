#include <SFML/Graphics.hpp>
#include "Client.h"


std::unique_ptr<Client> client;
void draw();

int main()
{
	client = std::make_unique<Client>();								//creates unique pointer to Client
		//creates window
	sf::Thread thread(&draw);
	thread.launch();
	
	//run a program whilst window is open

	client->client();
	

	return 0;
}

void draw()
{	
	sf::RenderWindow window(sf::VideoMode(800, 600), "LLP-TRON!");
	sf::CircleShape shape(100.f);										//creates sprite circle
	shape.setFillColor(sf::Color::Green);
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
	}
	
}