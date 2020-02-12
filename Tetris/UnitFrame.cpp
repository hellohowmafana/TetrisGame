#include "UnitFrame.hpp"

bool UnitFrame::TestX(int x)
{
	return x >= 0 && x < sizeX;
}

bool UnitFrame::TestY(int y)
{
	return y >= 0 && y < sizeY;
}

bool UnitFrame::TestXY(int x, int y)
{
	return TestX(x) && TestY(y);
}

int UnitFrame::GetWidth()
{
	return borderThickness * 2 + unitWidth * sizeX + separatorThickness * (sizeX + 1);
}

int UnitFrame::GetHeight()
{
	return borderThickness * 2 + unitWidth * sizeY + separatorThickness * (sizeY + 1);
}
