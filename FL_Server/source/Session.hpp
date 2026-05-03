#pragma once
#include <asio.hpp>
#include <asio/ssl.hpp> 
#include <memory>
#include <vector>
#include <cstdint>

namespace sl {
	class DataQueue;
	class NetData;
}

using ssl_socket = asio::ssl::stream<asio::ip::tcp::socket>;

class Session: public std::enable_shared_from_this<Session> {
public:
	Session(asio::ip::tcp::socket socket, asio::ssl::context& sslContext);
	~Session() = default;
	void start() { doHandshake(); }
	void close();
	void writeOnOutgoingData(std::vector<uint8_t>& data);
	std::shared_ptr<sl::DataQueue> getIncomingQueue() const { return incomingQueue; }
private:
	ssl_socket sessionSocket;
	asio::strand<asio::ip::tcp::socket::executor_type> sessionStrand;
	std::shared_ptr<sl::DataQueue> incomingQueue;
	std::shared_ptr<sl::DataQueue> outgoingQueue;
	asio::steady_timer handshakeTimer;

	void doHandshake();
	void doRead();
	void doWrite();
};