#include "pch.h"
#include "DefferedFunctionStorage.hpp"

sl::LoopOptions::LoopOptions(bool isEndless, uint8_t loopCount) {
	this->isEndless = isEndless;
	this->loopCount = loopCount;
}