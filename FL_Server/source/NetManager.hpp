#pragma once
#include <asio\ip\tcp.hpp>
#include <asio\ssl.hpp>
#include <string>
#include <memory>
#include "TimerHandle.hpp"

class Session;
class IncomingDataManager;
class OutputDataManager;
class DataProcessorManager;
namespace sl {
	class NetData;
}
namespace asio {
	class io_context;
}
class NetManager {
public:
	NetManager(asio::io_context& context, short port, DataProcessorManager& dtm);
	~NetManager();

	void doAccept();
private:
	asio::ssl::context sslContext;
	asio::ip::tcp::acceptor acceptor;
	std::vector<std::weak_ptr<Session>> sessions;
	// TODO: Если соединение прекратилось std::shared_ptr<IncomingDataManager> всё равно остаётся, надо как-то чистить
	std::vector<std::shared_ptr<IncomingDataManager>> incomingDataManagers;
	std::unique_ptr<sl::TimerHandle<void>> cleaningTimer;
	DataProcessorManager& dataProcessorManager;

	std::unordered_map<std::string, std::pair<int, std::chrono::steady_clock::time_point>> connectionAttempts;
	static constexpr int MAX_CONNECTIONS_PER_IP = 5;
	static constexpr int MAX_TOTAL_SESSIONS = 10000;

	void initSSL();
	void cleaning();

};