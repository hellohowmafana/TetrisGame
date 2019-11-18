#pragma once
#include "Frame.h"
#include <Windows.h>
class UnitFrame :
	public Frame
{
public:
	int unitWidth;
	int borderThickness;
	int separatorThickness;
	COLORREF* pBorderColor;
	COLORREF* pSeparatorColor;

public:
	bool ValidateX(int x);
	bool ValidateY(int y);
	bool ValidateXY(int x, int y);

	virtual int GetWidth();
	virtual int GetHeight();
};

