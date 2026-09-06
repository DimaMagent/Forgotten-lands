#include "pch.h"
#include "system/handlers/DefferedFunctionStorage.hpp"

sl::LoopOptions::LoopOptions(bool isEndless, uint8_t loopCount) {
	this->isEndless = isEndless;
	this->loopCount = loopCount;
}