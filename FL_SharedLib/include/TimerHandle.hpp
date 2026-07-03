#pragma once
#include <asio/steady_timer.hpp>
#include <asio/io_context.hpp>
#include <asio.hpp>
#include <asio/ssl/context.hpp>
#include <functional>


namespace sl {

	template <typename T>
	class TimerHandle {
	public:
		TimerHandle(asio::io_context& io, asio::chrono::seconds firstResponseTime, asio::chrono::seconds delay, std::function<T()> expiresFunction, int replyCount = 1)
			: timer_(io, firstResponseTime), counter_(0), expiresFunction_(expiresFunction), callDelay(delay), repeatQuantity_(replyCount), isInfinity(false)
		{
			timer_.async_wait(std::bind(&TimerHandle::functionCall, this));
		}
		TimerHandle(asio::io_context& io, asio::chrono::seconds firstResponseTime, asio::chrono::seconds delay, std::function<T()> expiresFunction, bool isInf)
			: timer_(io, firstResponseTime), counter_(0), expiresFunction_(expiresFunction), callDelay(delay), repeatQuantity_(0), isInfinity(isInf)
		{
			timer_.async_wait(std::bind(&TimerHandle::functionCall, this));
		}

		void functionCall() {
			if (counter_ < repeatQuantity_ || isInfinity) {
				expiresFunction_();
				if (!isInfinity) {
					++counter_;
				}
				timer_.expires_at(timer_.expiry() + callDelay);
				timer_.async_wait([this](const std::error_code&) { functionCall(); });
			}
		}

	private:
		asio::steady_timer timer_;
		int counter_ = 0;
		int repeatQuantity_;
		bool isInfinity;
		std::function<T()> expiresFunction_;
		asio::chrono::seconds callDelay;
	};
}
