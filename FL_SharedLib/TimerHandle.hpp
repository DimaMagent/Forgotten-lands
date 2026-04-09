#pragma once
#include "asio.hpp"
#include <iostream>

template <typename T>
class TimerHandle {
public:
	TimerHandle(asio::chrono::seconds firstResponseTime, asio::chrono::seconds delay, std::function<T()> expiresFunction, int replyCount = 1)
		: timer_(io, firstResponseTime), counter_(0), expiresFunction_(expiresFunction), callDelay(delay), repeatQuantity_(replyCount)
	{
		timer_.async_wait(std::bind(&TimerHandle::functionCall, this));
		io.run();
	}

	void functionCall() {
		if (counter_ < repeatQuantity_) {
			std::cout << "Counter: " << counter_ << std::endl;
			expiresFunction_();
			++counter_;
			timer_.expires_at(timer_.expiry() + callDelay);
			timer_.async_wait([this](const std::error_code&) { functionCall(); });
		}
	}

private:
	asio::io_context io;
	asio::steady_timer timer_;
	int counter_ = 0;
	int repeatQuantity_;
	std::function<T()> expiresFunction_;
	asio::chrono::seconds callDelay;
};

