#include "pch.hpp"
#include "Session.hpp"
#include "DataQueue.hpp"
#include "IncomingDataManager.hpp"

namespace {
	constexpr size_t MAX_PACKET_SIZE = 4096;
	constexpr size_t MAX_BIG_PACKET_SIZE = 65535;
}


Session::Session(asio::ip::tcp::socket socket, asio::ssl::context& sslContext, uint32_t token, DataProcessorManager& dpm) :
	token(token), sessionSocket(std::move(socket), sslContext),
	sessionStrand(asio::make_strand(sessionSocket.get_executor())),
	handshakeTimer(sessionSocket.get_executor())
{
	incomingQueue = std::make_shared<sl::net::DataQueue>();
	outgoingQueue = std::make_shared<sl::net::DataQueue>();
	incomingManager = std::make_unique<IncomingDataManager>(incomingQueue, dpm, token);
	incomingManager->OnWrongData.addFunction([this]() { close(); });
}

Session::~Session() = default;

void Session::close() {
	std::error_code ec;
	sessionSocket.lowest_layer().close(ec);
}

void Session::writeOnOutgoingData(std::vector<uint8_t>& data)
{
	auto self = shared_from_this();
	outgoingQueue->push(data);
	asio::post(sessionStrand, [self]() { self->doWrite(); });
}

void Session::doHandshake()
{
	auto self = shared_from_this();
	handshakeTimer.expires_after(asio::chrono::seconds(10));
	handshakeTimer.async_wait([this, self](std::error_code ec) {
		if (!ec) {
			std::cout << "Handshake timeout, closing connection\n";
			sessionSocket.lowest_layer().close();
		}
	});

	sessionSocket.async_handshake(
		asio::ssl::stream_base::server,
		asio::bind_executor(sessionStrand, [this, self](std::error_code ec) {
			handshakeTimer.cancel();
			if (ec) {
				std::cout << "Handshake error: " << ec.message() << std::endl;
				return;
			}
			std::cout << "TLS handshake OK" << std::endl;
			doRead();
			}));
}

void Session::doRead()
{
	auto self(shared_from_this());
	std::shared_ptr<std::vector<uint8_t>> localBuffer = std::make_shared<std::vector<uint8_t>>(8192u);
	sessionSocket.async_read_some(asio::buffer(*localBuffer), asio::bind_executor(sessionStrand, [this, self, localBuffer](std::error_code ec, size_t len) {
		if (ec) {
			std::cout << ec.value() << "::" << ec.message() << std::endl;
			return;
		}
		if (len > MAX_PACKET_SIZE) {
			std::cout << "package size is too big" << "\n";
			close();
			return;
		}
		localBuffer->resize(len);
		incomingQueue->push(*localBuffer);
		doRead();
		}));
}

void Session::doWrite()
{
	auto self(shared_from_this());
	std::shared_ptr<std::vector<uint8_t>> localBuffer = std::make_shared<std::vector<uint8_t>>(1024u);
	if (!outgoingQueue->tryPop(*localBuffer)) {
		std::cout << "No data to write, waiting..." << "\n";
		return;
	}

	asio::async_write(sessionSocket, asio::buffer(*localBuffer), asio::bind_executor(sessionStrand, [this, self, localBuffer](std::error_code ec, size_t len) {
		if (ec) {
			std::cout << ec.value() << "::" << ec.message() << std::endl;
			return;
		}
		std::cout << "Write continues" << "\n";
		doWrite();
		}));
}
