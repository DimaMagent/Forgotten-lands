// FL_Server.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "pch.hpp"
#include "Server.hpp"
#include <memory>

int main()
{
	try {
		setlocale(LC_ALL, "Russian");
		std::unique_ptr<Server> server = std::make_unique<Server>(2001);
		std::cout << "Server start" << std::endl;
		server->start();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		std::cin.get();
	}
	std::cout << "Server end" << std::endl;
	std::cin.get();
	return 1;
}