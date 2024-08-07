#include "TetrisShape.hpp"
#include "PromptFrame.hpp"
#include "GameFrame.hpp"
#include "Utility.hpp"

TetrisShape::TetrisShape() :
	posX(0),
	posY(0),
	rotation(TetrisRotation::Rotation0),
	current(false),
	pUnitFrame(nullptr),
	pTetrisType(nullptr)
{
}

void TetrisShape::InitializeRandom()
{
	pTetrisType = TetrisType::Random();

	TetrisRotation tetrisRotations[4] =
	{
		TetrisRotation::Rotation0,
		TetrisRotation::Rotation1,
		TetrisRotation::Rotation2,
		TetrisRotation::Rotation3,
	};
	int irotation = 0;
	if (pTetrisType->canRotate)
	{
		irotation = Utility::Random(0, pTetrisType->twoRotation ? 1 : 3);
	}
	else
	{
		irotation = 0;
	}
	if (!pTetrisType->clockwiseRotation)
	{
		if (1 == irotation)
			irotation = 3;
	}
	rotation = tetrisRotations[irotation];

	randomColor = TetrisType::GetRandomColor(pTetrisType->group);
}

void TetrisShape::Initialize(TetrisType* pTetrisType, TetrisRotation rotation, int randomColor)
{
	this->pTetrisType = pTetrisType;
	this->rotation = rotation;
	this->randomColor = randomColor;
}

void TetrisShape::RebornRandom()
{
	InitializeRandom();
	SetTopCenterPostion(false, true);
}

void TetrisShape::Reborn(TetrisType* pTetrisType, TetrisRotation rotation, int randomColor)
{
	Initialize(pTetrisType, rotation, randomColor);
	SetTopCenterPostion(false, true);
}

void TetrisShape::SetUseRandomColor(bool useRandomColor)
{
	this->useRandomColor = useRandomColor;
}

TetrisShape* TetrisShape::Clone(TetrisShape* pTetrisShape)
{
	*pTetrisShape = *this;
	return pTetrisShape;
}

TetrisType* TetrisShape::GetType()
{
	return pTetrisType;
}

bool TetrisShape::IsPenetrable()
{
	return pTetrisType->penetrable;
}

TetrisRotation TetrisShape::GetRotation()
{
	return rotation;
}

void TetrisShape::CenterHorizontal(bool leanLeft, bool careOffset)
{
	if (!pUnitFrame) return;

	if (careOffset)
	{
		posX = (pUnitFrame->sizeX - pTetrisType->col + (leanLeft ? 0 : 1)) / 2
			+ pTetrisType->horizontalCenterOffset;
		CalculateRotationPosition(rotation, TetrisRotation::Rotation0, &posX, nullptr);
	}
	else
	{
		posX = (pUnitFrame->sizeX - GetWidth() + (leanLeft ? 0 : 1)) / 2;
	}
}

void TetrisShape::SetTopCenterPostion(bool leanLeft, bool careOffset)
{
	if (pUnitFrame)
	{
		posY = 0;
		CenterHorizontal(leanLeft, careOffset);
	}
}

void TetrisShape::CenterPostion(bool leanLeft, bool leanTop)
{
	if (!pUnitFrame) return;

	posX = (pUnitFrame->sizeX - GetWidth() + (leanLeft ? 0 : 1)) / 2;
	posY = (pUnitFrame->sizeY - GetHeight() + (leanTop ? 0 : 1)) / 2;
}

bool TetrisShape::SetPostion(int x, int y)
{
	int oldPosX = posX;
	int oldPosY = posY;
	posX = x;
	posY = y;
	if (!ValidateShape(posX, posY, true))
	{
		posX = oldPosX;
		posY = oldPosY;
		return false;
	}
	return true;
}

bool TetrisShape::Move(int offsetX, int offsetY)
{
	int oldPosX = posX;
	int oldPosY = posY;
	posX = posX + offsetX;
	posY = posY + offsetY;
	if (!ValidateShape(posX, posY, true))
	{
		posX = oldPosX;
		posY = oldPosY;
		return false;
	}
	return true;
}

bool TetrisShape::MoveTo(int x, int y)
{
	int oldPosX = posX;
	int oldPosY = posY;
	posX = x;
	posY = y;
	if (!ValidateShape(posX, posY, true))
	{
		posX = oldPosX;
		posY = oldPosY;
		return false;
	}
	return true;
}

void TetrisShape::CalculateRotationPosition(TetrisRotation dst, TetrisRotation src, int* pPosX, int* pPosY)
{
	if (dst == src)
		return;

	bool needX = nullptr != pPosX;
	bool needY = nullptr != pPosY;
	if (!needX && !needY)
		return;

	int posX0, posY0;
	// calculate pos 0
	switch (src)
	{
	case TetrisRotation::Rotation0:
		if (needX)
			posX0 = *pPosX;
		if (needY)
			posY0 = *pPosY;
		break;
	case TetrisRotation::Rotation1:
		if (needX)
			posX0 = *pPosX - (pTetrisType->centerX - (pTetrisType->row - 1 - pTetrisType->centerY));
		if (needY)
			posY0 = *pPosY - (pTetrisType->centerY - pTetrisType->centerX);
		break;
	case TetrisRotation::Rotation2:
		if (needX)
			posX0 = *pPosX - (2 * pTetrisType->centerX - (pTetrisType->col - 1));
		if (needY)
			posY0 = *pPosY - (2 * pTetrisType->centerY - (pTetrisType->row - 1));
		break;
	case TetrisRotation::Rotation3:
		if (needX)
			posX0 = *pPosX - (pTetrisType->centerX - pTetrisType->centerY);
		if (needY)
			posY0 = *pPosY - (pTetrisType->centerY + pTetrisType->centerX - (pTetrisType->col - 1));
		break;
	}
	// calculate dst pos
	switch (dst)
	{
	case TetrisRotation::Rotation0:
		if (needX)
			*pPosX = posX0;
		if (needY)
			*pPosY = posY0;
		break;
	case TetrisRotation::Rotation1:
		if (needX)
			*pPosX = posX0 + pTetrisType->centerX - (pTetrisType->row - 1 - pTetrisType->centerY);
		if (needY)
			*pPosY = posY0 + pTetrisType->centerY - pTetrisType->centerX;
		break;
	case TetrisRotation::Rotation2:
		if (needX)
			*pPosX = posX0 + 2 * pTetrisType->centerX - (pTetrisType->col - 1);
		if (needY)
			*pPosY = posY0 + 2 * pTetrisType->centerY - (pTetrisType->row - 1);
		break;
	case TetrisRotation::Rotation3:
		if (needX)
			*pPosX = posX0 + pTetrisType->centerX - pTetrisType->centerY;
		if (needY)
			*pPosY = posY0 + pTetrisType->centerY + pTetrisType->centerX - (pTetrisType->col - 1);
		break;
	}
}

bool TetrisShape::CanRotate()
{
	return pTetrisType->canRotate;
}

bool TetrisShape::Rotate()
{
	if (!pTetrisType->canRotate) return true;

	TetrisRotation oldRotation = rotation;
	int oldPosX = posX;
	int oldPosY = posY;
	switch (oldRotation)
	{
	case TetrisRotation::Rotation0:
		rotation = pTetrisType->clockwiseRotation ? TetrisRotation::Rotation1 : TetrisRotation::Rotation3;
		break;
	case TetrisRotation::Rotation1:
		rotation = !pTetrisType->clockwiseRotation || pTetrisType->twoRotation ?
			TetrisRotation::Rotation0 : TetrisRotation::Rotation2;
		break;
	case TetrisRotation::Rotation2:
		rotation = pTetrisType->clockwiseRotation ? TetrisRotation::Rotation3 : TetrisRotation::Rotation1;
		break;
	case TetrisRotation::Rotation3:
		rotation = pTetrisType->clockwiseRotation || pTetrisType->twoRotation ?
			TetrisRotation::Rotation0 : TetrisRotation::Rotation2;
		break;
	}
	CalculateRotationPosition(rotation, oldRotation, &posX, &posY);

	if (!ValidateShape(posX, posY, true))
	{
		rotation = oldRotation;
		posX = oldPosX;
		posY = oldPosY;
		return false;
	}

	return true;
}

bool TetrisShape::RotateBack()
{
	if (!pTetrisType->canRotate) return true;

	TetrisRotation oldRotation = rotation;
	int oldPosX = posX;
	int oldPosY = posY;
	switch (oldRotation)
	{
	case TetrisRotation::Rotation0:
		rotation = pTetrisType->clockwiseRotation ^ pTetrisType->twoRotation ?
			TetrisRotation::Rotation3 : TetrisRotation::Rotation1;
		break;
	case TetrisRotation::Rotation1:
		rotation = pTetrisType->clockwiseRotation ? TetrisRotation::Rotation0 : TetrisRotation::Rotation2;
		break;
	case TetrisRotation::Rotation2:
		rotation = pTetrisType->clockwiseRotation ? TetrisRotation::Rotation1 : TetrisRotation::Rotation3;
		break;
	case TetrisRotation::Rotation3:
		rotation = pTetrisType->clockwiseRotation ? TetrisRotation::Rotation2 : TetrisRotation::Rotation0;
		break;
	}
	CalculateRotationPosition(rotation, oldRotation, &posX, &posY);

	if (!ValidateShape(posX, posY, true))
	{
		rotation = oldRotation;
		posX = oldPosX;
		posY = oldPosY;
		return false;
	}

	return true;
}

bool TetrisShape::StepLeft()
{
	posX--;
	if (!ValidateX(posX, true))
	{
		posX++;
		return false;
	}
	return true;
}

bool TetrisShape::StepRight()
{
	posX++;
	if (!ValidateX(posX + GetWidth() - 1, true))
	{
		posX--;
		return false;
	}
	return true;
}

bool TetrisShape::StepDown()
{
	posY++;
	if (!ValidateY(posY + GetHeight() - 1, true))
	{
		posY--;
		return false;
	}
	return true;
}

bool TetrisShape::StepUp()
{
	posY--;
	if (!ValidateY(posY, true))
	{
		posY++;
		return false;
	}
	return true;
}

bool TetrisShape::ValidateX(int x, bool frameCoordinate)
{
	if (frameCoordinate)
	{
		if (!pUnitFrame->ValidateX(x))
			return false;
		x = x - posX;
	}
	return x >= 0 && x <= GetWidth() - 1;
}

bool TetrisShape::ValidateY(int y, bool frameCoordinate)
{
	if (frameCoordinate)
	{
		if (!pUnitFrame->ValidateY(y))
			return false;
		y = y - posY;
	}
	return y >= 0 && y <= GetHeight() - 1;
}

bool TetrisShape::ValidateXY(int x, int y, bool frameCoordinate)
{
	return ValidateX(x, frameCoordinate) && ValidateY(y, frameCoordinate);
}

bool TetrisShape::ValidateShape(int posX, int posY, bool frameCoordinate)
{
	return ValidateXY(posX, posY, true) &&
		ValidateXY(posX + GetWidth() - 1, posY + GetHeight() - 1, true);
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
	return useRandomColor ? randomColor : pTetrisType->color;
}

int TetrisShape::GetRandomColor()
{
	return randomColor;
}

bool TetrisShape::IsOnTop()
{
	return GetTop() == 0;
}

bool TetrisShape::IsOnBottom()
{
	return GetBottom() == pUnitFrame->sizeY - 1;
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

void TetrisShape::SetCurrent(bool current)
{
	this->current = current;
}

void TetrisShape::SetFrame(UnitFrame* pUnitFrame)
{
	this->pUnitFrame = pUnitFrame;
}

UnitFrame* TetrisShape::GetFrame()
{
	return pUnitFrame;
}

bool TetrisShape::Save(const wstring label, wstring& value)
{
	if (Archive::labelNext == label)
	{
		wstring str;
		str.append(pTetrisType->group).append(L",")
			.append(pTetrisType->name).append(L",")
			.append(to_wstring(TetrisRotationToInt(rotation)));
		value = str;
	}
	else if(Archive::labelCurrent == label)
	{
		wstring str;
		str.append(pTetrisType->group).append(L",")
			.append(pTetrisType->name).append(L",")
			.append(to_wstring(TetrisRotationToInt(rotation))).append(L",")
			.append(to_wstring(posX)).append(L",")
			.append(to_wstring(posY));
		value = str;
	}
	else
	{
		return false;
	}
	return true;
}

bool TetrisShape::Load(const wstring label, wstring value)
{
	if (Archive::labelNext == label)
	{
		GameFrame* pGameFrame = ((PromptFrame*)pUnitFrame)->GetGameFrame();
		wchar_t* szs[3];
		Utility::SplitString((wchar_t*)(value.c_str()), L',', szs, 3);
		Initialize(TetrisType::GetTetrisType(szs[0], szs[1]), IntToTetrisRotation(stoi(szs[2])), TetrisType::GetRandomColor(pTetrisType->group));
		CenterPostion(false, false);
	}
	else if (Archive::labelCurrent == label)
	{
		GameFrame* pGameFrame = (GameFrame*)pUnitFrame;
		wchar_t* szs[5];
		Utility::SplitString((wchar_t*)(value.c_str()), L',', szs, 5);
		Initialize(TetrisType::GetTetrisType(szs[0], szs[1]), IntToTetrisRotation(stoi(szs[2])), TetrisType::GetRandomColor(pTetrisType->group));
		SetPostion(stoi(szs[3]), stoi(szs[4]));
	}
	else
	{
		return false;
	}
	return true;
}

TetrisRotation TetrisShape::IntToTetrisRotation(int irotation)
{
	switch (irotation)
	{
	case 0:
		return TetrisRotation::Rotation0;
	case 1:
		return TetrisRotation::Rotation1;
	case 2:
		return TetrisRotation::Rotation2;
	case 3:
		return TetrisRotation::Rotation3;
	default:
		return TetrisRotation::Rotation0;
	}
}

int TetrisShape::TetrisRotationToInt(TetrisRotation rotation)
{
	switch (rotation)
	{
	case TetrisRotation::Rotation0:
		return 0;
	case TetrisRotation::Rotation1:
		return 1;
	case TetrisRotation::Rotation2:
		return 2;
	case TetrisRotation::Rotation3:
		return 3;
	default:
		return 0;
	}
}
