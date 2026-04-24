#include "pch.h"
#include "DataQueue.hpp"

namespace sl {
	void DataQueue::push(const std::vector<uint8_t>& data) {
		queueMutex.lock();
		queue.push(data);
		queueMutex.unlock();
		onDataPushed.broadcast();
	}

	bool DataQueue::empty() const {
		std::lock_guard<std::mutex> lock(queueMutex);
		return queue.empty();
	}

	bool DataQueue::tryPop(std::vector<uint8_t>& out) {
		std::lock_guard<std::mutex> lock(queueMutex);
		if (queue.empty()) { return false; }
		out.resize(queue.front().size());
		out = std::move(queue.front());
		queue.pop();
		return true;
	}
}