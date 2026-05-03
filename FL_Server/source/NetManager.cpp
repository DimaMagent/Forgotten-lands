#include "pch.hpp"
#include "NetManager.hpp"
#include "Session.hpp"
#include "IncomingDataManager.hpp"
#include "DataProcessorManager.hpp"
#include <filesystem>

NetManager::NetManager(asio::io_context& context, short port, DataProcessorManager& dtm) : sslContext(asio::ssl::context::tls_server),
acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), dataProcessorManager(dtm) {
	cleaningTimer = std::make_unique<sl::TimerHandle<void>>(context,
		asio::chrono::seconds(600),
		asio::chrono::seconds(600),
		[this]() {cleaningSessions(); },
		true);
	initSSL();
}
NetManager::~NetManager() = default;

void NetManager::doAccept() {
	acceptor.async_accept(
		[this](std::error_code ec, asio::ip::tcp::socket socket)
		{
			if (!ec) {
				std::cout << "Client connected" << std::endl;
				std::shared_ptr<Session> sessionPtr = std::make_shared<Session>(std::move(socket), sslContext);
				sessionPtr->start();
				sessions.push_back(sessionPtr);
				incomingDataManagers.emplace_back(std::make_shared<IncomingDataManager>(sessionPtr->getIncomingQueue(), dataProcessorManager));
			}
			else {
				std::cout << ec.value() << "::" << ec.message() << std::endl;
			}
			doAccept();
		});
}

void NetManager::cleaningSessions() {
	if (sessions.size() == 0) { return; }

	for (size_t i = 0; i < sessions.size(); ++i) {
		if (sessions[i].expired()) {
			sessions[i] = std::move(sessions.back());
			sessions.pop_back();
			--i;
		}
	}
}

void NetManager::initSSL() {
	if (!std::filesystem::exists("server.key") || !std::filesystem::exists("server.crt")) {
		std::cout << "Generating TLS certificates..." << std::endl;
		int result = std::system(
			"openssl req -x509 -newkey rsa:4096 "
			"-keyout server.key -out server.crt "
			"-days 36500 -nodes -subj \"/CN=localhost\""
		);
		if (result != 0) {
			throw std::runtime_error("Failed to generate certificates. Is openssl installed?");
		}
	}

	sslContext.use_certificate_chain_file("server.crt");
	sslContext.use_private_key_file("server.key", asio::ssl::context::pem);
}