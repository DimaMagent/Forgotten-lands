#pragma once

#define ASIO_STANDALONE
#define DEBUG

#include <asio.hpp>
#include <asio/ssl.hpp>

#include <SFML/Graphics.hpp>

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <chrono>
#include <cstdint>
#include <algorithm>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>
#include <span>