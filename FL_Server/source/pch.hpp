#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define ASIO_STANDALONE          // говорим что используем standalone asio, не boost
#define ASIO_HAS_STD_CHRONO      // явно включаем std::chrono
// убеждаемся что boost date_time НЕ используется
#ifdef ASIO_HAS_BOOST_DATE_TIME
#undef ASIO_HAS_BOOST_DATE_TIME
#endif
#include <asio.hpp>

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <chrono>
#include <cstdint>