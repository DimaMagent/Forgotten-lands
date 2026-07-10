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
	logger = spdlog::get("network");
	incomingQueue = std::make_shared<sl::net::DataQueue>();
	outgoingQueue = std::make_shared<sl::net::DataQueue>();
	incomingManager = std::make_unique<IncomingDataManager>(incomingQueue, dpm, token);
	incomingManager->OnWrongData.addFunction([this]() { close(); });
}

Session::~Session() = default;

void Session::close() {
	logger->info("Session {} closed", token);
	std::error_code ec;
	sessionSocket.lowest_layer().close(ec);
	OnClientDisconnected.broadcast(token);
}

void Session::writeOnOutgoingData(std::vector<uint8_t>& data)
{
	auto self = shared_from_this();
	//std::cout << "push data size:" << data.size() << "\n";
	outgoingQueue->push(data);
	asio::post(sessionStrand, [self]() { self->doWrite(); });
}

void Session::doHandshake()
{

	logger->info("Starting TLS handshake for session: {}", token);

	auto self = shared_from_this();
	handshakeTimer.expires_after(asio::chrono::seconds(10));
	handshakeTimer.async_wait([this, self](std::error_code ec) {
		if (!ec) {
			logger->warn("Handshake timeout for session: {}. Closing connection", token);
			
			sessionSocket.lowest_layer().close();
		}
	});

	sessionSocket.async_handshake(
		asio::ssl::stream_base::server,
		asio::bind_executor(sessionStrand, [this, self](std::error_code ec) {
			handshakeTimer.cancel();
			if (ec) {
				logger->error("Handshake error for session {}: {}", token, ec.message());
				return;
			}
			logger->info("TLS handshake successful for session: {}", token);
			OnAcceptSucceeded.broadcast();
			doRead();
			}));
}

void Session::doRead()
{
	auto self(shared_from_this());
	std::shared_ptr<std::vector<uint8_t>> localBuffer = std::make_shared<std::vector<uint8_t>>(8192u);
	sessionSocket.async_read_some(asio::buffer(*localBuffer), asio::bind_executor(sessionStrand, [this, self, localBuffer](std::error_code ec, size_t len) {
		if (ec) {
			if (ec == asio::error::eof || ec == asio::ssl::error::stream_truncated) {
				logger->info("Session {} closed by peer", token);
			}
			else {
				logger->error("Session {} read error {}: {}", token, ec.value(), ec.message());
			}
			close();
			return;
		}
		if (len > MAX_PACKET_SIZE) {
			logger->warn("Session {} received packet size {} exceeding maximum allowed size {}", token, len, MAX_PACKET_SIZE);
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
		return;
	}

	asio::async_write(sessionSocket, asio::buffer(*localBuffer), asio::bind_executor(sessionStrand, [this, self, localBuffer](std::error_code ec, size_t len) {
		if (ec) {
			logger->error("Session {} write error {}: {}", token, ec.value(), ec.message());
			close();
			return;
		}
		doWrite();
		}));
}
