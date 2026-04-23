#pragma once
#include <queue>
#include <mutex>
#include "NetData.hpp"
#include <vector>
#include <cstdint>
#include "LockFreeDelegate.hpp"

namespace sl {
	class DataQueue {
	public:
		LockFreeDelegate<> onDataPushed;
		void push(const std::vector<uint8_t>& data);
		bool empty() const;
		bool tryPop(std::vector<uint8_t>& out);
	private:
		mutable std::mutex queueMutex;
		std::queue<std::vector<uint8_t>> queue;
	};
}