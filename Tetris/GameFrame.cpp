#include "GameFrame.h"

GameFrame GameFrame::singleton;

void GameFrame::Initialize(Configuration* pConfiguration)
{
	// initialize from configuration
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

	// initialize game
	tetrisShape.SetFrame(this);
	tetrisShape.InitializeRandom();
	tetrisShape.SetTopCenterPostion(false);
	mass.SetGameFrame(this);
	mass.Initialize();
	nextTetrisShape.InitializeRandom();
	score = 0;
}

void GameFrame::Start()
{
	gameState = GameState::Start;
}

void GameFrame::End()
{
	gameState = GameState::End;
}

void GameFrame::Pause()
{
	gameState = GameState::Pause;
}

void GameFrame::Resume()
{
	gameState = GameState::Start;
}

bool GameFrame::IsGameOver()
{
	return GameState::End == gameState;
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
	bool isDropped = false;
	if (tetrisShape.IsOnBottom())
	{
		isDropped = true;
	}
	else if (tetrisShape.StepDown())
	{
		if (!tetrisShape.GetType()->penetrable)
		{
			if (mass.IsTouched(&tetrisShape))
			{
				tetrisShape.StepUp();
				isDropped = true;
			}
		}
		else
		{
			if (tetrisShape.GetTop() - 1 == mass.FindBottommostBlankY(&tetrisShape, tetrisShape.GetLeft()))
			{
				tetrisShape.StepUp();
				isDropped = true;
			}
		}
	}

	if(isDropped)
	{
		mass.Union(&tetrisShape);
		mass.RemoveFullLines(tetrisShape.GetTop(), tetrisShape.GetBottom());
		if (mass.IsFull())
		{
			End();
			return;
		}
		tetrisShape.Reborn(nextTetrisShape.GetType(), nextTetrisShape.GetRotation());
		nextTetrisShape.InitializeRandom();
	}
}

void GameFrame::Drop()
{
	if (!tetrisShape.GetType()->penetrable)
	{
		int distance = mass.CalculateDistanceY(&tetrisShape);
		tetrisShape.Move(0, distance - 1);
	}
	else
	{
		int y = mass.FindBottommostBlankY(&tetrisShape, tetrisShape.GetLeft());
		tetrisShape.MoveTo(tetrisShape.GetLeft(), y);
	}
	mass.Union(&tetrisShape);
	mass.RemoveFullLines(tetrisShape.GetTop(), tetrisShape.GetBottom());
	if (mass.IsFull())
	{
		End();
		return;
	}
	tetrisShape.Reborn(nextTetrisShape.GetType(), nextTetrisShape.GetRotation());
	nextTetrisShape.InitializeRandom();
}

void GameFrame::Rotate()
{
	if(tetrisShape.Rotate())
		if (mass.HitTest(&tetrisShape))
			tetrisShape.RotateBack();
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

bool GameFrame::Save(const TCHAR* szSection, TCHAR** pszString)
{
	return false;
}

bool GameFrame::Load(const TCHAR* szSection, TCHAR* szString)
{
	tstring str(szString);
	if (Archive::labelFrame == szSection)
	{
		TCHAR* szs[2];
		Utility::SplitString((TCHAR*)(str.c_str()), _T(','), szs, 2);
		sizeX = stoi(szs[0]);
		sizeY = stoi(szs[1]);
		return false;
	}
	else if (Archive::labelScore == szSection)
	{
		score = stoi(str);
	}
	return true;
}
