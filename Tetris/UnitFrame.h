#pragma once
#include "Frame.h"
class UnitFrame :
	public Frame
{
public:
	int sizeX;
	int sizeY;
	int unitWidth;
	int borderThickness;
	int separatorThickness;

public:
	bool ValidateX(int x);
	bool ValidateY(int y);
	bool ValidateXY(int x, int y);
};

