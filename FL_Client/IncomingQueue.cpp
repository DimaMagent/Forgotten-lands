#include "IncomingQueue.hpp"

void IncomingQueue::push(const std::vector<char>& data) {
	std::lock_guard<std::mutex> lock(queueMutex);
	queue.push(data);
}

bool IncomingQueue::empty() const {
	std::lock_guard<std::mutex> lock(queueMutex);
	return queue.empty();
}

bool IncomingQueue::tryPop(std::vector<char>& out) {
	std::lock_guard<std::mutex> lock(queueMutex);
	if (queue.empty()) { return false; }
	out = std::move(queue.front());
	queue.pop();
	return true;
}