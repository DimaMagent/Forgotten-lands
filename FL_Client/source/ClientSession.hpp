#pragma once
#include <memory>
#include <vector>
#include "asio\ip\tcp.hpp"
#include "asio/ssl.hpp"

namespace sl::net {
	class DataQueue;
}
class IncomingDataManager;
class OutputDataManager;

using ssl_socket = asio::ssl::stream<asio::ip::tcp::socket>;

class ClientSession : public std::enable_shared_from_this<ClientSession> {
public:
	ClientSession(asio::ip::tcp::socket socket, asio::ssl::context& sslContext);
	~ClientSession();

	void writeOnOutgoingData(std::vector<uint8_t>& data);
	void start();
	std::weak_ptr<sl::net::DataQueue> getIncomingQueue() { return incomingQueue; }
	std::weak_ptr<sl::net::DataQueue> getOutgoingQueue() { return outgoingQueue; }
private:
	ssl_socket sessionSocket;
	asio::strand<asio::ip::tcp::socket::executor_type> sessionStrand;
	std::shared_ptr<sl::net::DataQueue> incomingQueue;
	std::shared_ptr<sl::net::DataQueue> outgoingQueue;

	void doHandshake();
	void doWrite();
	void doRead();

};