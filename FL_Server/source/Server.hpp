#pragma once
#include <asio\ip\tcp.hpp>
#include <asio/ssl.hpp>
#include <vector>
#include <memory>
#include <spdlog/logger.h>
#include <SFML/System/Clock.hpp>
#include "system/handlers/TimerHandle.hpp"

class DataProcessorManager;
class NetManager;
class World;
class PlayerManager;
class ConnectionEvents;
class ServerEntityFactory;
class ServerSystemUpdater;

namespace asio {
	class io_context;
}
namespace sl {
	class DelegateToken;
}

class Server {
public:
	Server(short port);
	~Server();
	void start();
private:
	//loggers
	std::shared_ptr<spdlog::logger> net_logger;
	std::shared_ptr<spdlog::logger> system_logger;

	std::unique_ptr<asio::io_context> serverContext;

	std::unique_ptr<ConnectionEvents> connectionEvents;
	std::unique_ptr<World> world;
	std::unique_ptr<DataProcessorManager> dataProcessorManager;
	std::unique_ptr<PlayerManager> playerManager;
	std::unique_ptr<NetManager> netManager;
	std::unique_ptr<ServerSystemUpdater> systemUpdater;
	std::unique_ptr<sl::TimerHandle<void>> cleaningTimer;

	std::shared_ptr<asio::steady_timer> tickTimer;
	sf::Clock clock;

	std::shared_ptr<sl::DelegateToken> delegateToken;

	short port;

	void onClientAccept(uint32_t token);
	
	void initLogging();

	void update(float dt);

	void sheduleTick();
};