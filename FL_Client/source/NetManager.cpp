#include "pch.hpp"
#include "NetManager.hpp"
#include "ClientSession.hpp"
#include "Packer.hpp"
#include "IncomingDataManager.hpp"
#include "OutputDataManager.hpp"

NetManager::NetManager(asio::io_context& context, DataProcessorManager& dataProcessorManager) : sslContext(asio::ssl::context::tls_client),
	dataProcessorManager(dataProcessorManager),
	socket(context), endpoint(asio::ip::make_address(serverAddress), serverPort)
{
	logger = spdlog::get("network");

	try {
		sslContext.load_verify_file("server.crt");
		sslContext.set_verify_mode(asio::ssl::verify_peer);
	}
	catch (std::exception& e) {
		logger->error("Error loading SSL certificate: {}", e.what());
	}
}

NetManager::~NetManager() = default;

void NetManager::doConnect()
{
	logger->info("client started connecting address: {}:{}", serverAddress, serverPort);

	socket.async_connect(endpoint, [this](std::error_code ec) {
		if (ec) {
			logger->error("Error connecting to server: {}: {}", ec.value(), ec.message());
			return;
		}
		logger->info("Connected to server: {}:{}", serverAddress, serverPort);
		std::shared_ptr<ClientSession> sessionPtr = std::make_shared<ClientSession>(std::move(socket), sslContext, dataProcessorManager);
		session = sessionPtr;
		sessionPtr->start();
		sessionPtr->OnAcceptSucceeded.addFunction([this]() {OnAccept.broadcast(); });
		outputManager = std::make_shared<OutputDataManager>(sessionPtr);
		Packer::setOutputManager(outputManager);
		});
}