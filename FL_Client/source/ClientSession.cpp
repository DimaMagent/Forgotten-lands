#include "pch.hpp"
#include "ClientSession.hpp"
#include "DataQueue.hpp"
#include "IncomingDataManager.hpp"
#include "OutputDataManager.hpp"

ClientSession::ClientSession(asio::ip::tcp::socket socket, asio::ssl::context& sslContext)
    : sessionSocket(std::move(socket), sslContext), sessionStrand(asio::make_strand(sessionSocket.get_executor()))
{
	incomingQueue = std::make_shared<sl::net::DataQueue>();
	outgoingQueue = std::make_shared<sl::net::DataQueue>();
}

ClientSession::~ClientSession() = default;

void ClientSession::start() {
	doHandshake();
}

void ClientSession::writeOnOutgoingData(std::vector<uint8_t>& data)
{
	auto self = shared_from_this();
    outgoingQueue->push(data);
    asio::post(sessionStrand, [self]() { self->doWrite(); });
}

void ClientSession::doHandshake() {
	auto self = shared_from_this();
	sessionSocket.async_handshake(
		asio::ssl::stream_base::client,
		asio::bind_executor(sessionStrand, [this, self](std::error_code ec) {
			if (ec) {
				std::cout << "Handshake error: " << ec.message() << std::endl;
				return;
			}
			std::cout << "TLS handshake OK" << std::endl;
			doRead();
			}));
}

void ClientSession::doWrite()
{
	auto self(shared_from_this());
	std::shared_ptr<std::vector<uint8_t>> localBuffer = std::make_shared<std::vector<uint8_t>>(1024u);
	if (!outgoingQueue->tryPop(*localBuffer)) {
		return;
	}

	asio::async_write(sessionSocket, asio::buffer(*localBuffer, localBuffer->size()), asio::bind_executor(sessionStrand, [this, self, localBuffer](std::error_code ec, size_t len) {
		if (ec) {
			std::cout << ec.value() << "::" << ec.message() << std::endl;
			return;
		}
		std::cout << "Write, sizeof: " << sizeof(*localBuffer) << "\n";
		doWrite();
		}));
}
void ClientSession::doRead() {
	auto self(shared_from_this());
	std::shared_ptr <std::vector<uint8_t>> localBuffer = std::make_shared<std::vector<uint8_t>>(8192u);
	sessionSocket.async_read_some(asio::buffer(*localBuffer, localBuffer->size()), asio::bind_executor(sessionStrand, [this, self, localBuffer](std::error_code ec, size_t len) {
		if (ec) {
			std::cout << ec.value() << "::" << ec.message() << std::endl;
			return;
		}
		std::cout << "Received data from server" << "\n";
		localBuffer->resize(len);
		incomingQueue->push(*localBuffer);
		doRead();
		}));
}