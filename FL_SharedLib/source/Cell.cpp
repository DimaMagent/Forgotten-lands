#include "pch.h"
#include "Cell.hpp"

sl::Cell::Cell(float posX, float posY)
{
	if (posX <= 0) { x = 0; }
	if (posY <= 0) { y = 0; }

	x = static_cast<cellIndex>(posX / cellSize);
	y = static_cast<cellIndex>(posY / cellSize);
}

sl::Cell::Cell(cellIndex cellNumX, cellIndex cellNumY)
{
	if (cellNumX < 0) { x = 0; }
	if (cellNumY < 0) { y = 0; }

	x = cellNumX;
	y = cellNumY;
}