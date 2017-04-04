#include "Client.h"


int main()
{
	std::unique_ptr<Client> client = std::make_unique<Client>();
	client->runClient();
	return 0;
}