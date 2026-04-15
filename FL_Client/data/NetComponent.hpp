#pragma once
#include "asio\ip\tcp.hpp"
#include <string>

class ClientSession;
class NetData;
namespace asio {
	class io_context;
}
class NetComponent {
	public:
		NetComponent(asio::io_context& context);
		~NetComponent() = default;

		bool tryWrite(NetData& data);
		void doConnect();
	private:
		std::string serverAddress = "127.0.0.1";
		unsigned short serverPort = 2001;
		asio::ip::tcp::socket socket;
		asio::ip::tcp::endpoint endpoint;
		std::weak_ptr<ClientSession> session;


};