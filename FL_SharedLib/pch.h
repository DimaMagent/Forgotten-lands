// pch.h: это предварительно скомпилированный заголовочный файл.
// Перечисленные ниже файлы компилируются только один раз, что ускоряет последующие сборки.
// Это также влияет на работу IntelliSense, включая многие функции просмотра и завершения кода.
// Однако изменение любого из приведенных здесь файлов между операциями сборки приведет к повторной компиляции всех(!) этих файлов.
// Не добавляйте сюда файлы, которые планируете часто изменять, так как в этом случае выигрыша в производительности не будет.

#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
// Добавьте сюда заголовочные файлы для предварительной компиляции
#define ASIO_STANDALONE          // говорим что используем standalone asio, не boost
#define ASIO_HAS_STD_CHRONO      // явно включаем std::chrono

// убеждаемся что boost date_time НЕ используется
#ifdef ASIO_HAS_BOOST_DATE_TIME
#undef ASIO_HAS_BOOST_DATE_TIME
#endif
#include "framework.h"
#include <vector>
#include <functional>
#include <mutex>
#include <cstdint>
#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>

#endif //PCH_H
