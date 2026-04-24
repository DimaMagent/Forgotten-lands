#pragma once
#include "asio\ip\tcp.hpp"

class Session;
class IncomingDataManager;
class DataProcessorManager;
namespace asio {
	class io_context;
}
class Server {
public:
	Server(asio::io_context& context, short port);
private:
	asio::ip::tcp::acceptor acceptor;
	std::weak_ptr<Session> session;
	std::shared_ptr<IncomingDataManager> incomingDataManager;
	std::optional<DataProcessorManager> dataProcessorManager;
	void doAccept();
};