#pragma once
#include "asio.hpp"

class Server {
public:
	Server(asio::io_context& context, short port);

private:
	asio::ip::tcp::acceptor acceptor;
	void doAccept();
};