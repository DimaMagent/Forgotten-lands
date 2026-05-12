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
	try {
		sslContext.load_verify_file("server.crt");
		sslContext.set_verify_mode(asio::ssl::verify_peer);
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

NetManager::~NetManager() = default;

void NetManager::doConnect()
{
	std::cout << "client started connecting address: " << serverAddress << ":" << serverPort << std::endl;

	socket.async_connect(endpoint, [this](std::error_code ec) {
		if (ec) {
			std::cout << ec.value() << "::" << ec.message() << std::endl;
			return;
		}
		std::cout << "Connect to server" << std::endl;
		std::shared_ptr<ClientSession> sessionPtr = std::make_shared<ClientSession>(std::move(socket), sslContext, dataProcessorManager);
		session = sessionPtr;
		sessionPtr->start();
		outputManager = std::make_shared<OutputDataManager>(sessionPtr);
		Packer::setOutputManager(outputManager);
		});
}