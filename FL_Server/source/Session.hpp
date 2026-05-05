#pragma once
#include <asio.hpp>
#include <asio/ssl.hpp> 
#include <memory>
#include <vector>
#include <cstdint>

namespace sl::net {
	class DataQueue;
	class NetData;
}
class IncomingDataManager;
class DataProcessorManager;

using ssl_socket = asio::ssl::stream<asio::ip::tcp::socket>;

class Session: public std::enable_shared_from_this<Session> {
public:
	Session(asio::ip::tcp::socket socket, asio::ssl::context& sslContext, uint32_t token, DataProcessorManager& dpm);
	~Session();
	void start() { doHandshake(); }
	void close();
	void writeOnOutgoingData(std::vector<uint8_t>& data);
	std::weak_ptr<sl::net::DataQueue> getIncomingQueue() const { return incomingQueue; }
private:
	uint32_t token;
	ssl_socket sessionSocket;
	asio::strand<asio::ip::tcp::socket::executor_type> sessionStrand;
	std::shared_ptr<sl::net::DataQueue> incomingQueue;
	std::shared_ptr<sl::net::DataQueue> outgoingQueue;
	std::unique_ptr<IncomingDataManager> incomingManager;
	asio::steady_timer handshakeTimer;

	void doHandshake();
	void doRead();
	void doWrite();
};