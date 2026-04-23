#include "pch.hpp"
#include "NetComponent.hpp"
#include "ClientSession.hpp"
#include "Packer.hpp"
#include "IncomingDataManager.hpp"
#include "OutputDataManager.hpp"

NetComponent::NetComponent(asio::io_context& context) : socket(context), endpoint(asio::ip::make_address(serverAddress), serverPort) {}

NetComponent::~NetComponent() = default;

void NetComponent::doConnect()
{
	std::cout << "client started connecting address: " << serverAddress << ":" << serverPort << std::endl;

	socket.async_connect(endpoint, [this](std::error_code ec) {
		if (ec) {
			std::cout << ec.value() << "::" << ec.message() << std::endl;
			return;
		}
		std::cout << "Connect to server" << std::endl;
		std::shared_ptr<ClientSession> sessionPtr = std::make_shared<ClientSession>(std::move(socket));
		session = sessionPtr;
		sessionPtr->start();
		inputManager = std::make_unique<IncomingDataManager>(sessionPtr->getIncomingQueue());
		outputManager = std::make_shared<OutputDataManager>(sessionPtr);
		Packer::setOutputManager(outputManager);
		});
}