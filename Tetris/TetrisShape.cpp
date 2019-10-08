#include "TetrisShape.h"

TetrisShape::TetrisShape(TetrisType* pTetrisType)
	:posX(0), posY(0), rotation(TetrisRotation::TetrisRotation0)
{
	this->pTetrisType = pTetrisType;
}

void TetrisShape::Rotate()
{
	if (rotation == TetrisRotation::TetrisRotation0)
	{
		rotation = TetrisRotation::TetrisRotation1;
	}
	else if (rotation == TetrisRotation::TetrisRotation1)
	{
		rotation = TetrisRotation::TetrisRotation2;
	}
	else if (rotation == TetrisRotation::TetrisRotation2)
	{
		rotation = TetrisRotation::TetrisRotation3;
	}
	else if (rotation == TetrisRotation::TetrisRotation3)
	{
		rotation = TetrisRotation::TetrisRotation0;
	}
	int row = rotation == TetrisRotation::TetrisRotation0 || rotation == TetrisRotation::TetrisRotation2 ?
		pTetrisType->row : pTetrisType->col;
	posX = posX + pTetrisType->centerX - (row - 1 - pTetrisType->centerY);
	posY = posY + pTetrisType->centerY - pTetrisType->centerX;
}

void TetrisShape::Move()
{
}

void TetrisShape::StepDown()
{
}

void TetrisShape::DropDown()
{
}

bool TetrisShape::ValidateXY(int x, int y)
{
	if (rotation == TetrisRotation::TetrisRotation0 || rotation == TetrisRotation::TetrisRotation2)
	{
		if (x < 0 || x >= pTetrisType->col)
			return false;
		if (y < 0 || y >= pTetrisType->row)
			return false;
	}
	else
	{
		if (x < 0 || x >= pTetrisType->row)
			return false;
		if (y < 0 || y >= pTetrisType->col)
			return false;
	}
	return true;
}

char TetrisShape::GetData(int x, int y)
{
	int originalx, originaly;
	if (rotation == TetrisRotation::TetrisRotation1)
	{
		originalx = pTetrisType->row - 1 - y;
		originaly = x;
	}
	else if (rotation == TetrisRotation::TetrisRotation2)
	{
		originalx = pTetrisType->col - 1 - x;
		originaly = pTetrisType->row - 1 - y;
	}
	else if (rotation == TetrisRotation::TetrisRotation3)
	{
		originalx = y;
		originaly = pTetrisType->col - 1 - x;
	}
	return 0;
}

bool TetrisShape::IsSolid(int x, int y)
{
	return GetData(x,y) != '0';
}

int TetrisShape::GetWidth()
{
	if (rotation == TetrisRotation::TetrisRotation0 || rotation == TetrisRotation::TetrisRotation2)
		return pTetrisType->col;
	else
		return pTetrisType->row;
}

int TetrisShape::GetHeight()
{
	if (rotation == TetrisRotation::TetrisRotation0 || rotation == TetrisRotation::TetrisRotation2)
		return pTetrisType->row;
	else
		return pTetrisType->col;
}

int TetrisShape::GetLeft()
{
	return posX;
}

int TetrisShape::GetRight()
{
	return posX + GetWidth();
}

int TetrisShape::GetTop()
{
	return posY;
}

int TetrisShape::GetBottom()
{
	return posY + GetHeight();
}

int TetrisShape::GetColor()
{
	return pTetrisType->color;
}
