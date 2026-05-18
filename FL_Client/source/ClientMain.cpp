#include "pch.hpp"
#include <memory>
#include "Client.hpp"


int main()
{
	std::unique_ptr<Client> client = std::make_unique<Client>();
	client->start();
	return 0;
}


