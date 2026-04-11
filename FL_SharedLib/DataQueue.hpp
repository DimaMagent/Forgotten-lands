#pragma once
#include <queue>
#include <vector>
#include <mutex>
#include "NetData.hpp"

class DataQueue {
public:
	void push(const NetData& data);
	bool empty() const;
	bool tryPop(NetData& out);
private:
	mutable std::mutex queueMutex;
	std::queue<NetData> queue;
};