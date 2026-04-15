#pragma once
#include "asio\ip\tcp.hpp"

class Session;
class NetData;
namespace asio {
	class io_context;
}
class Server {
public:
	Server(asio::io_context& context, short port);
	bool tryWrite(NetData& data);
private:
	asio::ip::tcp::acceptor acceptor;
	std::weak_ptr<Session> session;
	void doAccept();
};