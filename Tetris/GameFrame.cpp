#include "GameFrame.h"

GameFrame GameFrame::singleton;

void GameFrame::Initialize(Configuration* pConfiguration)
{
	left = pConfiguration->frameLeft;
	top = pConfiguration->frameTop;
	sizeX = pConfiguration->frameSizeX;
	sizeY = pConfiguration->frameSizeY;
	unitWidth = pConfiguration->unitWidth;
	borderThickness = pConfiguration->borderThickness;
	separatorThickness = pConfiguration->separatorThickness;
	startLineBlankRate = pConfiguration->startLineBlankRate;
	useColor = pConfiguration->useColor;
	useColorRandom = pConfiguration->useColorRandom;
	useMassColor = pConfiguration->useMassColor;

	pBorderColor = &pConfiguration->colorBorder;
	pSeparatorColor = &pConfiguration->colorSeparator;
	pTetrisColors = &pConfiguration->vecTetrisColors;
	pMassColor = &pConfiguration->colorMass;
}

void GameFrame::Start()
{
	tetrisShape.SetGameFrame(this);
	mass.SetGameFrame(this);
	
	tetrisShape.InitializeRandom();
	tetrisShape.SetTopCenterPostion(false);
	mass.Initialize();

	nextTetrisShape.InitializeRandom();

	score = 0;
}

void GameFrame::Stop()
{
}

void GameFrame::Pause()
{
}

void GameFrame::Resume()
{
}

bool GameFrame::ValidateX(int x)
{
	return x >= 0 && x < sizeX;
}

bool GameFrame::ValidateY(int y)
{
	return y >= 0 && y < sizeY;
}

bool GameFrame::ValidateXY(int x, int y)
{
	return ValidateX(x) && ValidateY(y);
}

void GameFrame::Insert(Mass* pMass, TetrisShape* pTetrisShape)
{
}

bool GameFrame::HasTouched(Mass* pMass, TetrisShape* pTetrisShape)
{
	return false;
}

void GameFrame::StepLeft()
{
	if(tetrisShape.StepLeft())
		if(mass.HitTest(&tetrisShape))
			tetrisShape.StepRight();
}

void GameFrame::StepRight()
{
	if(tetrisShape.StepRight())
		if(mass.HitTest(&tetrisShape))
			tetrisShape.StepLeft();
}

void GameFrame::StepDown()
{
	if (tetrisShape.StepDown())
	{
		if (mass.IsTouched(&tetrisShape))
		{
			tetrisShape.StepUp();
			mass.Union(&tetrisShape);
			tetrisShape.Reborn();
		}
	}
	else
	{
		mass.Union(&tetrisShape);
		tetrisShape.Reborn();
	}
}

void GameFrame::Drop()
{
	int distance = mass.CalculateDistanceY(&tetrisShape);
	tetrisShape.Move(0, distance - 1);
	mass.Union(&tetrisShape);
	tetrisShape.Reborn();
}

void GameFrame::Rotate()
{
	tetrisShape.Rotate();
}

Mass* GameFrame::GetMass()
{
	return &mass;
}

TetrisShape* GameFrame::GetShape()
{
	return &tetrisShape;
}

TetrisShape* GameFrame::GetNextShape()
{
	return &nextTetrisShape;
}

bool GameFrame::Save(TCHAR* szString)
{
	return false;
}

bool GameFrame::Load(TCHAR* szString)
{
	int pos;
	tstring str(szString);
	if (tstring::npos != (pos = str.find(Archive::labelFrame)))
	{
		TCHAR* szs[2];
		Utility::SplitString((TCHAR*)(str.substr(pos + 1).c_str()), _T(','), szs, 2);
		sizeX = stoi(szs[0]);
		sizeY = stoi(szs[1]);
		return false;
	}
	else if (tstring::npos != (pos = str.find(Archive::labelScore)))
	{
		score = stoi(str.substr(pos + 1));
	}
	return true;
}
