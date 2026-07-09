#include "pch.hpp"
#include "NetManager.hpp"
#include "Session.hpp"
#include "DataProcessorManager.hpp"
#include "OutputDataManager.hpp"
#include <filesystem>
#include <random>
#include "Packer.hpp"

NetManager::NetManager(asio::io_context& context, short port, DataProcessorManager& dtm) : sslContext(asio::ssl::context::tls_server),
	acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), dataProcessorManager(dtm),
	outputDataManager(std::make_shared<OutputDataManager>(sessions))
{
	logger = spdlog::get("network");
	cleaningTimer = std::make_unique<sl::TimerHandle<void>>(context,
		asio::chrono::seconds(120),
		asio::chrono::seconds(120),
		[this]() {cleaning(); },
		true);
	initSSL();
	Packer::setOutputManager(outputDataManager);
}
NetManager::~NetManager() = default;

void NetManager::doAccept() {
	acceptor.async_accept(
		[this](std::error_code ec, asio::ip::tcp::socket socket)
		{

			logger->info("Incoming connection from: {}", socket.remote_endpoint().address().to_string());

			if (!ec) {
				std::string ip = socket.remote_endpoint().address().to_string();

				if (sessions.size() >= MAX_TOTAL_SESSIONS) {

					logger->warn("Server full, rejecting: {}", ip);
					socket.close();
					doAccept();
					return;
				}
				auto now = std::chrono::steady_clock::now();
				auto& [count, firstTime] = connectionAttempts[ip];
				if (now - firstTime < std::chrono::seconds(60)) {
					if (count >= MAX_CONNECTIONS_PER_IP) {
						logger->warn("Rate limit exceeded for IP: {} ", ip);
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

				uint32_t sessionToken = generateToken();

				logger->info("Client {} connected", sessionToken);

				std::shared_ptr<Session> sessionPtr = std::make_shared<Session>(std::move(socket), sslContext, sessionToken, dataProcessorManager);
				sessionPtr->start();
				sessionPtr->OnAcceptSucceeded.addFunction([this, sessionToken]() { OnAccept.broadcast(sessionToken); });
				sessions.try_emplace(sessionToken, sessionPtr);
				
			}
			else {
				logger->error("{} :: {}", ec.value(), ec.message());
			}
			doAccept();
		});
}

void NetManager::cleaning() {

	auto now = std::chrono::steady_clock::now();

	for (auto it = connectionAttempts.begin(); it != connectionAttempts.end(); ) {
		if (now - it->second.second > std::chrono::seconds(300)) {
			it = connectionAttempts.erase(it);
			logger->info("Cleared connection attempts for IP: {}", it->first);
		}
		else {
			++it;
		}
	}
	/* TODO: реализовать систему очистки невалидных сессий
	if (sessions.size() != 0) {
		for (size_t i = 0; i < sessions.size(); ++i) {
			if (sessions[i].expired()) {
				sessions[i] = std::move(sessions.back());
				sessions.pop_back();
				--i;
			}
		}
	}
	*/
}

uint32_t NetManager::generateToken() const
{
	std::random_device rd;
	std::uniform_int_distribution<uint32_t> dist;
	return dist(rd);
}

void NetManager::initSSL() {

	if (!std::filesystem::exists("server.key") || !std::filesystem::exists("server.crt")) {

		logger->info("Generating TLS certificates...");

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