#pragma once
#include "asio\ip\tcp.hpp"
#include <vector>
#include <memory>

class Session;
class IncomingDataManager;
class DataProcessorManager;
namespace asio {
	class io_context;
}
class Server {
public:
	Server(asio::io_context& context, short port);
	~Server();
private:
	asio::ip::tcp::acceptor acceptor;
	std::vector<std::weak_ptr<Session>> sessions;
	std::shared_ptr<IncomingDataManager> incomingDataManager;
	std::unique_ptr<DataProcessorManager> dataProcessorManager;
	void doAccept();
};