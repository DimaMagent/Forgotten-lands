#include "Session.hpp"
#include <iostream>
#include <string>
#include <array>

Session::Session(asio::ip::tcp::socket socket) : sessionSocket(std::move(socket))
{
}

void Session::doRead()
{
	auto self(shared_from_this());
	sessionSocket.async_read_some(asio::buffer(buffer), [this, self](std::error_code ec, size_t len) {
		if (ec) {
			std::cout << ec.value() << "::" << ec.message() << std::endl;
			return;
		}
		std::cout << "Received: " << std::string(buffer.data(), len) << std::endl;
		doRead();
		});

}

void Session::doWrite(std::size_t length)
{
}
