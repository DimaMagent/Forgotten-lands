#pragma once
#include <vector>

struct NetData {
public:
	NetData() = default;
	NetData(const std::vector<char>& data) : data(data) {}
	NetData(size_t size) : data(size) {}
	~NetData() = default;
	std::vector<char>& getData() { return data; }
private:
	std::vector<char> data;
};