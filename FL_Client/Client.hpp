#pragma once
#include "asio.hpp"


class ClientSession;


class Client {
public:
	Client(asio::io_context& context, asio::ip::tcp::endpoint endpoint);
	bool tryWrite(std::vector<char>& data);
private:
	asio::ip::tcp::socket socket;
	std::weak_ptr<ClientSession> session;
	
	void doConnect(asio::ip::tcp::endpoint connectEndpoint);
};