#pragma once
#include "asio.hpp"

class Session;

class Server {
public:
	Server(asio::io_context& context, short port);
	bool tryWrite(std::vector<char>& data);
private:
	asio::ip::tcp::acceptor acceptor;
	std::weak_ptr<Session> session;
	void doAccept();
};