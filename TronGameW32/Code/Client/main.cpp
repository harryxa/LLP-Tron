#include "Client.h"

std::unique_ptr<Client> client;
void draw();

int main()
{
	client = std::make_unique<Client>();//creates unique pointer to Client
	
	//creates thread to draw window and cube
	sf::Thread thread(&draw);
	thread.launch();
	
	client->client();
	

	return 0;
}

void draw()
{	
	sf::RenderWindow window(sf::VideoMode(800, 600), "LLP-TRON!");

	while (window.isOpen())
	{
		//check all events that were triggered since the last iteration of the loop
		sf::Event event;

		while (window.pollEvent(event))
		{
			//close window
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				client->input(&event);
			}
		}

		window.clear();
		client->draw(window);
		window.display();
	}
}