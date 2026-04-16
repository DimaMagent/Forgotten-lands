#pragma once
#include <queue>
#include <mutex>
#include "NetData.hpp"

namespace sl {
	class DataQueue {
	public:
		void push(const NetData& data);
		bool empty() const;
		bool tryPop(NetData& out);
	private:
		mutable std::mutex queueMutex;
		std::queue<NetData> queue;
	};
}