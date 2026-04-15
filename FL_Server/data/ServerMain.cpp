// FL_Server.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include "Server.hpp"

int main()
{
	try {
		setlocale(LC_ALL, "Russian");
		asio::io_context serverContext;
		Server server(serverContext, 2001);
		std::cout << "Server start" << std::endl;
		serverContext.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		std::cin.get();
	}
	std::cout << "Server end" << std::endl;
	std::cin.get();
	return 1;
}