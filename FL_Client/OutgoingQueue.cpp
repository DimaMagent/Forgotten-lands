#include "OutgoingQueue.hpp"

void OutgoingQueue::push(const std::vector<char>& data){
	std::lock_guard<std::mutex> lock(queueMutex);
	queue.push(data);
}

bool OutgoingQueue::empty() const {
	std::lock_guard<std::mutex> lock(queueMutex);
	return queue.empty();
}

bool OutgoingQueue::tryPop(std::vector<char>& out) {
	std::lock_guard<std::mutex> lock(queueMutex);
	if (queue.empty()) { return false; }

	out = std::move(queue.front());
	queue.pop();
	return true;
}