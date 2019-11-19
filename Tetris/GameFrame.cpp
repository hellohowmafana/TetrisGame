#include "GameFrame.h"
#include "PromptFrame.h"
#include "InfoFrame.h"
#include "Level.h"
GameFrame GameFrame::singleton;

GameFrame::GameFrame()
{
}

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
	iconScaleRatio = pConfiguration->iconScaleRatio;
	maskTransparency = pConfiguration->maskTransparency;

	pBorderColor = &pConfiguration->colorBorder;
	pSeparatorColor = &pConfiguration->colorSeparator;
	pTetrisColors = &pConfiguration->vecTetrisColors;
	pMassColor = &pConfiguration->colorMass;

	pathPauseIcon = pConfiguration->pathPauseIcon;
	pathResumeIcon = pConfiguration->pathResumeIcon;
	pathGameOver = pConfiguration->pathGameOver;
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
	mass.GenerateLines(sizeY - startLine, startLine, startLineBlankRate);
	nextTetrisShape.SetFrame(pPromptFrame);
	nextTetrisShape.InitializeRandom();
	nextTetrisShape.SetUseRandomColor(useColorRandom);
	nextTetrisShape.CenterPostion(false, false);
	pPromptFrame->SetTetrisShape(&nextTetrisShape);
	score = 0;
	level = startLevel;
	pInfoFrame->SetInfomations(&level, &score, &startLine);
}

void GameFrame::Reborn()
{
	tetrisShape.InitializeRandom();
	tetrisShape.SetTopCenterPostion(false, true);
	mass.Clear();
	mass.GenerateLines(sizeY - startLine, startLine, startLineBlankRate);
	nextTetrisShape.InitializeRandom();
	nextTetrisShape.CenterPostion(false, false);
	score = 0;
	level = startLevel;
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

bool GameFrame::StepDown()
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

	return isDropped;
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
}

int GameFrame::Union()
{
	score += (int)(droppedScore * Level::GetLevel(level)->scoreRate);
	level = level > Level::JudgeLevel(score)->level ? level : Level::JudgeLevel(score)->level;

	mass.Union(&tetrisShape);
	vecLastFullLines.clear();
	for (int i = tetrisShape.GetTop(); i <= tetrisShape.GetBottom(); i++)
	{
		if (mass.IsLineFull(i))
		{
			vecLastFullLines.push_back(mass.GetLine(i));
		}
	}
	return (int)vecLastFullLines.size();
}

bool GameFrame::HasFullLine()
{
	return mass.HasFullLine();
}

void GameFrame::RemoveFullLines()
{
	int removeLinesCount = mass.RemoveFullLines(tetrisShape.GetTop(), tetrisShape.GetBottom());
	if (0 != removeLinesCount)
	{
		score += (int)(vecRemoveScores[removeLinesCount - 1] * Level::GetLevel(level)->scoreRate);
		level = level > Level::JudgeLevel(score)->level ? level : Level::JudgeLevel(score)->level;
	}
}

void GameFrame::Rotate()
{
	if(tetrisShape.Rotate())
		if (mass.HitTest(&tetrisShape))
			tetrisShape.RotateBack();
}

void GameFrame::RebornTetrisShape()
{
	tetrisShape.Reborn(nextTetrisShape.GetType(), nextTetrisShape.GetRotation(), nextTetrisShape.GetRandomColor());
	nextTetrisShape.InitializeRandom();
	nextTetrisShape.CenterPostion(false, false);
}

bool GameFrame::IsFull()
{
	return mass.IsFull();
}

bool GameFrame::IsLastFullLine(const MassLine* pMassLine)
{
	return vecLastFullLines.end() != find(vecLastFullLines.begin(), vecLastFullLines.end(), pMassLine);
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

bool GameFrame::Save(const wchar_t* szSection, wchar_t** pszString)
{
	return false;
}

bool GameFrame::Load(const wchar_t* szSection, wchar_t* szString)
{
	wstring str(szString);
	if (Archive::labelFrame == szSection)
	{
		wchar_t* szs[2];
		Utility::Spliwstring((wchar_t*)(str.c_str()), L',', szs, 2);
		sizeX = stoi(szs[0]);
		sizeY = stoi(szs[1]);
	}
	else if (Archive::labelNext == szSection)
	{
		nextTetrisShape.Load(szSection,	szString);
	}
	else if (Archive::labelCurrent == szSection)
	{
		tetrisShape.Load(szSection, szString);
	}
	else if (Archive::labelMass == szSection)
	{
		mass.Load(szSection, szString);
	}
	else if (Archive::labelScore == szSection)
	{
		score = stoi(str);
	}
	return true;
}
