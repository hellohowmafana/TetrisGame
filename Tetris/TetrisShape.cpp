#include "TetrisShape.h"
#include "GameFrame.h"
#include "Utility.h"

TetrisShape::TetrisShape()
	:posX(0),
	posY(0),
	rotation(TetrisRotation::Rotation0),
	pGameFrame(nullptr),
	pTetrisType(nullptr)
{
}

void TetrisShape::InitializeRandom()
{
	TetrisRotation tetrisRotations[4] =
	{
		TetrisRotation::Rotation0,
		TetrisRotation::Rotation1,
		TetrisRotation::Rotation2,
		TetrisRotation::Rotation3,
	};

	rotation = tetrisRotations[Utility::Random(0, 3)];

	pTetrisType = TetrisType::Random();
}

void TetrisShape::Initialize(TetrisType* pTetrisType, TetrisRotation rotation)
{
	this->pTetrisType = pTetrisType;
	this->rotation = rotation;
}

bool TetrisShape::Move(int offsetX, int offsetY)
{
	int oldPosX = posX;
	int oldPosY = posY;
	posX = posX + offsetX;
	posY = posY + offsetY;
	if (!(ValidateXY(posX, posY, true) && (ValidateXY(posX + GetWidth() - 1, posY + GetHeight() - 1, true))))
	{
		posX = oldPosX;
		posY = oldPosY;
		return false;
	}
	return true;
}

bool TetrisShape::Rotate()
{
	if (!pTetrisType->canRotate) return true;

	TetrisRotation newRotation;
	if (rotation == TetrisRotation::Rotation0)
	{
		newRotation = TetrisRotation::Rotation1;
	}
	else if (rotation == TetrisRotation::Rotation1)
	{
		newRotation = TetrisRotation::Rotation2;
	}
	else if (rotation == TetrisRotation::Rotation2)
	{
		newRotation = TetrisRotation::Rotation3;
	}
	else if (rotation == TetrisRotation::Rotation3)
	{
		newRotation = TetrisRotation::Rotation0;
	}
	int row = newRotation == TetrisRotation::Rotation0 || rotation == TetrisRotation::Rotation2 ?
		pTetrisType->row : pTetrisType->col;
	int newPosX = posX + pTetrisType->centerX - (row - 1 - pTetrisType->centerY);
	int newPosY = posY + pTetrisType->centerY - pTetrisType->centerX;

	if (!ValidateXY(newPosX, newPosY, true))
		return false;

	rotation = newRotation;
	posX = newPosX;
	posY = newPosY;

	return true; 
}

bool TetrisShape::StepLeft()
{
	if(!ValidateX(posX - 1, true))
		return false;
	posX--;
	return true;
}

bool TetrisShape::StepRight()
{
	if (!ValidateX(posX + 1, true))
		return false;
	posX++;
	return true;
}

bool TetrisShape::StepDown()
{
	if (!ValidateY(posY + 1, true))
		return false;
	posY++;
	return true;
}

bool TetrisShape::ValidateX(int x, bool frameCoordinate)
{
	if (frameCoordinate)
		x = x - posX;
	return x >= 0 && x <= GetWidth() - 1;
}

bool TetrisShape::ValidateY(int y, bool frameCoordinate)
{
	if (frameCoordinate)
		y = y - posY;
	return y >= 0 && y <= GetHeight() - 1;
}

bool TetrisShape::ValidateXY(int x, int y, bool frameCoordinate)
{
	return ValidateX(x, frameCoordinate) && ValidateY(y, frameCoordinate);
}

char TetrisShape::GetData(int x, int y, bool frameCoordinate)
{
	if (!ValidateXY(x, y, frameCoordinate))
		return '0';

	if (frameCoordinate)
	{
		x = x - posX;
		y = y - posY;
	}

	int originalx, originaly;
	if (rotation == TetrisRotation::Rotation0)
	{
		originalx = x;
		originaly = y;
	}
	else if (rotation == TetrisRotation::Rotation1)
	{
		originalx = y;
		originaly = pTetrisType->row - 1 - x;
	}
	else if (rotation == TetrisRotation::Rotation2)
	{
		originalx = pTetrisType->col - 1 - x;
		originaly = pTetrisType->row - 1 - y;
	}
	else if (rotation == TetrisRotation::Rotation3)
	{
		originalx = pTetrisType->col - 1 - y;
		originaly = x;
	}

	return pTetrisType->pData[originalx + originaly * pTetrisType->col];
}

bool TetrisShape::IsSolid(int x, int y, bool frameCoordinate)
{
	if (!ValidateXY(x, y, frameCoordinate)) return false;

	return GetData(x, y, frameCoordinate) != '0';
}

int TetrisShape::GetWidth()
{
	if (rotation == TetrisRotation::Rotation0 || rotation == TetrisRotation::Rotation2)
		return pTetrisType->col;
	else
		return pTetrisType->row;
}

int TetrisShape::GetHeight()
{
	if (rotation == TetrisRotation::Rotation0 || rotation == TetrisRotation::Rotation2)
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
	return posX + GetWidth() - 1;
}

int TetrisShape::GetTop()
{
	return posY;
}

int TetrisShape::GetBottom()
{
	return posY + GetHeight() - 1;
}

int TetrisShape::GetColor()
{
	return pTetrisType->color;
}

int TetrisShape::GetBottommostSolidY(int x, bool frameCoordinate)
{
	if (!ValidateX(x, frameCoordinate)) return -1;

	int begin = frameCoordinate ? GetBottom() : GetHeight() - 1;
	int end = frameCoordinate ? GetTop() : 0;

	for (int i = begin; i >= end; i--)
	{
		if (IsSolid(x, i, frameCoordinate))
			return i;
	}
	return -1;
}

void TetrisShape::SetGameFrame(GameFrame* pGameFrame)
{
	this->pGameFrame = pGameFrame;
}
