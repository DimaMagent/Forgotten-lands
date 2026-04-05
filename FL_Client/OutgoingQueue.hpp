#pragma once
#include <queue>
#include <vector>
#include <mutex>

class OutgoingQueue {
public:
	void push(const std::vector<char>& data);
	bool empty() const;
	bool tryPop(std::vector<char>& out);
private:
	std::queue<std::vector<char>> queue;
	mutable std::mutex queueMutex;
};