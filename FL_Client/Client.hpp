#pragma once
#include "asio.hpp"

class ClientSession;
struct NetData;

class Client {
public:
	Client(asio::io_context& context, asio::ip::tcp::endpoint endpoint);
	bool tryWrite(NetData& data);
private:
	asio::ip::tcp::socket socket;
	std::weak_ptr<ClientSession> session;
	
	void doConnect(asio::ip::tcp::endpoint connectEndpoint);
};