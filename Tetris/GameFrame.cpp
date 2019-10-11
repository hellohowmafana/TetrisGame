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

void GameFrame::Insert(Mass* pMass, TetrisShape* pTetrisShape)
{
}

bool GameFrame::HasTouched(Mass* pMass, TetrisShape* pTetrisShape)
{
	return false;
}

void GameFrame::StepLeft()
{
	
}

void GameFrame::StepRight()
{
}

void GameFrame::StepDown()
{
}

void GameFrame::Drop()
{
}

void GameFrame::Rotate()
{

}

Mass* GameFrame::GetMass()
{
	return &mass;
}

TetrisShape* GameFrame::GetShape()
{
	return &tetrisShape;
}
