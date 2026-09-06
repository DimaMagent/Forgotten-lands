#pragma once
#include "LockFreeDelegate.hpp"

class ConnectionEvents {
public:
	sl::LockFreeDelegate<uint32_t> OnClientConnected;
	sl::LockFreeDelegate<uint32_t> OnClientDisconnected;
};