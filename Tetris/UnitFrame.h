#pragma once
#include "Frame.h"
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
	bool ValidateX(int x);
	bool ValidateY(int y);
	bool ValidateXY(int x, int y);

	virtual int GetWidth();
	virtual int GetHeight();
};

