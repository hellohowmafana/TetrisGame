#include "GameFrame.h"
#include "PromptFrame.h"
#include "InfoFrame.h"
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
	startLevel = pConfiguration->startLevel;
	startLine = pConfiguration->startLine;
	startLineBlankRate = pConfiguration->startLineBlankRate;
	vecRemoveScores = pConfiguration->vecRemoveScores;
	droppedScore = pConfiguration->droppedScore;
	useColor = pConfiguration->useColor;
	useColorRandom = pConfiguration->useColorRandom;
	useMassColor = pConfiguration->useMassColor;

	pBorderColor = &pConfiguration->colorBorder;
	pSeparatorColor = &pConfiguration->colorSeparator;
	pTetrisColors = &pConfiguration->vecTetrisColors;
	pMassColor = &pConfiguration->colorMass;
}

void GameFrame::SetPromptFrame(PromptFrame* pPromptFrame)
{
	this->pPromptFrame = pPromptFrame;
}

void GameFrame::SetInfoFrame(InfoFrame* pInfoFrame)
{
	this->pInfoFrame = pInfoFrame;
}

void GameFrame::InitializeGame()
{
	tetrisShape.SetFrame(this);
	tetrisShape.InitializeRandom();
	tetrisShape.SetUseRandomColor(useColorRandom);
	tetrisShape.SetTopCenterPostion(false, true);
	mass.SetGameFrame(this);
	mass.Initialize();
	nextTetrisShape.SetFrame(pPromptFrame);
	nextTetrisShape.InitializeRandom();
	nextTetrisShape.SetUseRandomColor(useColorRandom);
	nextTetrisShape.CenterPostion(false, false);
	pPromptFrame->SetTetrisShape(&nextTetrisShape);
	score = 0;
	level = startLevel;
	pInfoFrame->SetInfomations(&level, &score, &startLine);
}

void GameFrame::Start()
{
	gameState = GameState::Start;
	tetrisShape.InitializeRandom();
	tetrisShape.SetTopCenterPostion(false, true);
	mass.Clear();
	mass.Initialize();
	nextTetrisShape.InitializeRandom();
	nextTetrisShape.CenterPostion(false, false);
	score = 0;
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

GameState GameFrame::GetGameState()
{
	return gameState;
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
		EndDrop();
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
	EndDrop();
}

void GameFrame::EndDrop()
{
	score += droppedScore;
	mass.Union(&tetrisShape);
	int removeLinesCount = mass.RemoveFullLines(tetrisShape.GetTop(), tetrisShape.GetBottom());
	if(0 != removeLinesCount)
		score += vecRemoveScores[removeLinesCount - 1];
	if (mass.IsFull())
	{
		End();
		return;
	}
	tetrisShape.Reborn(nextTetrisShape.GetType(), nextTetrisShape.GetRotation(), nextTetrisShape.GetRandomColor());
	nextTetrisShape.InitializeRandom();
	nextTetrisShape.CenterPostion(false, false);
	//if (mass.HitTest(&tetrisShape))
	//{
	//	mass.Union(&tetrisShape);
	//	End();
	//	return;
	//}
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
