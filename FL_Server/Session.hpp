#pragma once
#include <memory>
#include "asio.hpp"

class Session: public std::enable_shared_from_this<Session> {
public:
	Session(asio::ip::tcp::socket socket);
	void start() { doRead(); }
private:
	asio::ip::tcp::socket sessionSocket;
	std::array<char, 128> buffer = {};

	void doRead();
	void doWrite(std::size_t length);
};