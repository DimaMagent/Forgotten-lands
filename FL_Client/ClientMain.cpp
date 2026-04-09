
#define ASIO_STANDALONE

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "asio.hpp"
#include "Client.hpp"


int main()
{
	try {
		setlocale(LC_ALL, "Russian");
		asio::io_context clientContext;
		asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1"), 2001);
		Client client(clientContext, endpoint);
		std::cout << "client start" << std::endl;
		clientContext.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		std::cin.get();
	}
	std::cout << "Client end" << std::endl;
	std::cin.get();
	return 0;
	
}


