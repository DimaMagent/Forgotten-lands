#pragma once
#include <queue>
#include <vector>
#include <mutex>

class IncomingQueue {
public:
	void push(const std::vector<char>& data);
	bool empty() const;
	bool tryPop(std::vector<char>& out);
private:
	mutable std::mutex queueMutex;
	std::queue<std::vector<char>> queue;
};