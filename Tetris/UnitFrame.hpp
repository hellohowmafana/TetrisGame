#pragma once
#include "Frame.hpp"
#include <Windows.h>
#include <gdiplus.h>
using Color = Gdiplus::Color;

class UnitFrame :
	public Frame
{
public:
	int unitWidth;
	int borderThickness;
	int separatorThickness;
	Color backgroundColor;
	Color borderColor;
	Color separatorColor;

public:
	bool TestX(int x);
	bool TestY(int y);
	bool TestXY(int x, int y);

	virtual int GetWidth();
	virtual int GetHeight();
};

