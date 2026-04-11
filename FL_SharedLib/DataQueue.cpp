#include "pch.h"
#include "DataQueue.hpp"

void DataQueue::push(const NetData& data) {
	std::lock_guard<std::mutex> lock(queueMutex);
	queue.push(data);
}

bool DataQueue::empty() const {
	std::lock_guard<std::mutex> lock(queueMutex);
	return queue.empty();
}

bool DataQueue::tryPop(NetData& out) {
	std::lock_guard<std::mutex> lock(queueMutex);
	if (queue.empty()) { return false; }
	out = std::move(queue.front());
	queue.pop();
	return true;
}