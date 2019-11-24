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
	pathUnitBitmap = pConfiguration->pathUnitBitmapFile;

	pathPauseIcon = pConfiguration->pathPauseIcon;
	pathResumeIcon = pConfiguration->pathResumeIcon;
	pathBeginSplash = pConfiguration->pathBeginSplash;
	pathGameOverSplash = pConfiguration->pathGameOverSplash;
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
	tetrisShape.SetCurrent(true);
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

bool GameFrame::Save(const wstring label, wstring& value)
{
	if (Archive::labelFrame == label)
	{
		value = to_wstring(sizeX) + L"," + to_wstring(sizeY);
	}
	else if (Archive::labelNext == label)
	{
		nextTetrisShape.Save(label, value);
	}
	else if (Archive::labelCurrent == label)
	{
		tetrisShape.Save(label, value);
	}
	else if (Archive::labelMass == label)
	{
		mass.Save(label, value);
	}
	else if (Archive::labelScore == label)
	{
		value = to_wstring(score);
	}
	else if (Archive::labelStartLevel == label)
	{
		value = to_wstring(startLevel);
	}
	else if (Archive::labelStartLine == label)
	{
		value = to_wstring(startLine);
	}
	return true;
}

bool GameFrame::Load(const wstring label, wstring value)
{
	if (Archive::labelFrame == label)
	{
		wchar_t* szs[2];
		Utility::Spliwstring((wchar_t*)(value.c_str()), L',', szs, 2);
		sizeX = stoi(szs[0]);
		sizeY = stoi(szs[1]);
	}
	else if (Archive::labelNext == label)
	{
		nextTetrisShape.Load(label,	value);
	}
	else if (Archive::labelCurrent == label)
	{
		tetrisShape.Load(label, value);
	}
	else if (Archive::labelMass == label)
	{
		mass.Load(label, value);
	}
	else if (Archive::labelScore == label)
	{
		score = stoi(value);
	}
	else if (Archive::labelStartLevel == label)
	{
		startLevel = stoi(value);
	}
	else if (Archive::labelStartLine == label)
	{
		startLine = stoi(value);
	}
	return true;
}
