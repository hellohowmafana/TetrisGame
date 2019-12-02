#include "PromptFrame.hpp"

bool PromptFrame::Initialize(Configuration* pConfiguration)
{
	left = pConfiguration->promptFrameLeft;
	top = pConfiguration->promptFrameTop;
	sizeX = pConfiguration->promptFrameSizeX;
	sizeY = pConfiguration->promptFrameSizeY;
	unitWidth = pConfiguration->unitWidth;
	borderThickness = pConfiguration->borderThickness;
	separatorThickness = pConfiguration->separatorThickness;
	backgroundColor = pConfiguration->colorFrameBackground;
	borderColor = pConfiguration->colorBorder;
	separatorColor = pConfiguration->colorSeparator;

	return true;
}

void PromptFrame::SetTetrisShape(TetrisShape* pTetrisShape)
{
	this->pTetrisShape = pTetrisShape;
}

TetrisShape* PromptFrame::GetTerisShape()
{
	return pTetrisShape;
}

void PromptFrame::SetGameFrame(GameFrame* pGameFrame)
{
	this->pGameFrame = pGameFrame;
}

GameFrame* PromptFrame::GetGameFrame()
{
	return pGameFrame;
}

PromptFrame PromptFrame::singleton;
