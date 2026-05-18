#pragma once
#include "asio\ip\tcp.hpp"
#include "asio\ssl.hpp"
#include <string>
#include "LockFreeDelegate.hpp"

class ClientSession;
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
		sl::LockFreeDelegate<> OnAccept;

		NetManager(asio::io_context& context, DataProcessorManager& dataProcessorManager);
		~NetManager();

		void doConnect();
	private:
		std::string serverAddress = "127.0.0.1";
		unsigned short serverPort = 2001;
		asio::ssl::context sslContext;
		asio::ip::tcp::socket socket;
		asio::ip::tcp::endpoint endpoint;
		std::weak_ptr<ClientSession> session;
		std::shared_ptr<OutputDataManager> outputManager;
		DataProcessorManager& dataProcessorManager;

};