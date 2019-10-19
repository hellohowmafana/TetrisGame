#include "UnitFrame.h"

bool UnitFrame::ValidateX(int x)
{
	return x >= 0 && x < sizeX;
}

bool UnitFrame::ValidateY(int y)
{
	return y >= 0 && y < sizeY;
}

bool UnitFrame::ValidateXY(int x, int y)
{
	return ValidateX(x) && ValidateY(y);
}