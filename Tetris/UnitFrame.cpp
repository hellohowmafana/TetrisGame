#include "UnitFrame.hpp"

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

int UnitFrame::GetWidth()
{
	return borderThickness * 2 + unitWidth * sizeX + separatorThickness * (sizeX + 1);
}

int UnitFrame::GetHeight()
{
	return borderThickness * 2 + unitWidth * sizeY + separatorThickness * (sizeY + 1);
}
