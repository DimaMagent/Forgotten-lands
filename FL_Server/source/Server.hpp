#pragma once
#include <asio\ip\tcp.hpp>
#include <asio/ssl.hpp>
#include <vector>
#include <memory>
#include "TimerHandle.hpp"

class DataProcessorManager;
class NetManager;
class World;
namespace asio {
	class io_context;
}
class Server {
public:
	Server(short port);
	~Server();
	void start();
private:
	std::unique_ptr<asio::io_context> serverContext;
	std::unique_ptr<DataProcessorManager> dataProcessorManager;
	std::unique_ptr<NetManager> netManager;
	std::unique_ptr<World> world;
	std::unique_ptr<sl::TimerHandle<void>> cleaningTimer;
};