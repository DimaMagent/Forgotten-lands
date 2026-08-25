#include "pch.h"
#include "DefferedFunctionStorage.hpp"


void sl::DefferedFunctionStorage::addDefferedCall(std::function<void()>&& func, std::chrono::milliseconds time) {
	if (!io) return;

	auto timer = std::make_shared<asio::steady_timer>(*io, time);
	timer->async_wait([timer, callback = std::move(func)](const asio::error_code& ec) {
		if (!ec && callback) {
			callback();
		}
		});
}

sl::LoopOptions::LoopOptions(bool isEndless, uint8_t loopCount) {
	this->isEndless = isEndless;
	this->loopCount = loopCount;
}