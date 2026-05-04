#include "pch.hpp"
#include "NetManager.hpp"
#include "Session.hpp"
#include "IncomingDataManager.hpp"
#include "DataProcessorManager.hpp"
#include <filesystem>

NetManager::NetManager(asio::io_context& context, short port, DataProcessorManager& dtm) : sslContext(asio::ssl::context::tls_server),
acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), dataProcessorManager(dtm) {
	cleaningTimer = std::make_unique<sl::TimerHandle<void>>(context,
		asio::chrono::seconds(120),
		asio::chrono::seconds(120),
		[this]() {cleaning(); },
		true);
	initSSL();
}
NetManager::~NetManager() = default;

void NetManager::doAccept() {
	acceptor.async_accept(
		[this](std::error_code ec, asio::ip::tcp::socket socket)
		{
			if (!ec) {
				std::string ip = socket.remote_endpoint().address().to_string();

				if (sessions.size() >= MAX_TOTAL_SESSIONS) {
					std::cout << "Server full, rejecting: " << ip << "\n";
					socket.close();
					doAccept();
					return;
				}
				auto now = std::chrono::steady_clock::now();
				auto& [count, firstTime] = connectionAttempts[ip];
				if (now - firstTime < std::chrono::seconds(60)) {
					if (count >= MAX_CONNECTIONS_PER_IP) {
						std::cout << "Rate limit exceeded for IP: " << ip << "\n";
						socket.close();
						doAccept();
						return;
					}
					++count;
				}
				else {
					firstTime = now;
					count = 1;
				}

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

void NetManager::cleaning() {

	auto now = std::chrono::steady_clock::now();

	for (auto it = connectionAttempts.begin(); it != connectionAttempts.end(); ) {
		if (now - it->second.second > std::chrono::seconds(300)) {
			it = connectionAttempts.erase(it);
		}
		else {
			++it;
		}
	}

	if (sessions.size() != 0) {
		for (size_t i = 0; i < sessions.size(); ++i) {
			if (sessions[i].expired()) {
				sessions[i] = std::move(sessions.back());
				sessions.pop_back();
				--i;
			}
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