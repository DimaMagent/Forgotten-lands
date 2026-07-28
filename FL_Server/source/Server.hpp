#pragma once
#include <asio\ip\tcp.hpp>
#include <asio/ssl.hpp>
#include <vector>
#include <memory>
#include "TimerHandle.hpp"

class DataProcessorManager;
class NetManager;
class World;
class PlayerManager;
class ConnectionEvents;
class ServerEntityFactory;

namespace asio {
	class io_context;
}

class Server {
public:
	Server(short port);
	~Server();
	void start();
private:
	//loggers
	std::shared_ptr<spdlog::logger> net_logger;

	std::unique_ptr<asio::io_context> serverContext;

	std::unique_ptr<ConnectionEvents> connectionEvents;
	std::unique_ptr<World> world;
	std::unique_ptr<PlayerManager> playerManager;
	std::unique_ptr<DataProcessorManager> dataProcessorManager;
	std::unique_ptr<NetManager> netManager;
	std::unique_ptr<ServerEntityFactory> entityFactory;
	std::unique_ptr<sl::TimerHandle<void>> cleaningTimer;



	void onClientAccept(uint32_t token);
	
	void initLogging();
};