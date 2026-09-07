#pragma once
#include <vector>
#include <cstdint>
#include "system/handlers/LockFreeDelegate.hpp"
#include "network/packets/PacketDataTypes.hpp"

using DataProcessedDelegate = sl::LockFreeDelegate<sl::net::PacketType, std::vector<uint8_t>&&>;
