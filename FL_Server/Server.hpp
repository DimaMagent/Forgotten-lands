#pragma once
#include "asio.hpp"

class Session;
struct NetData;

class Server {
public:
	Server(asio::io_context& context, short port);
	bool tryWrite(NetData& data);
private:
	asio::ip::tcp::acceptor acceptor;
	std::weak_ptr<Session> session;
	void doAccept();
};