#include "PromptFrame.h"

bool PromptFrame::Initialize(Configuration* pConfiguration)
{
	// initialize from configuration
	left = pConfiguration->promptFrameLeft;
	top = pConfiguration->promptFrameTop;
	sizeX = pConfiguration->promptFrameSizeX;
	sizeY = pConfiguration->promptFrameSizeY;
	unitWidth = pConfiguration->unitWidth;
	borderThickness = pConfiguration->borderThickness;
	separatorThickness = pConfiguration->separatorThickness;
	pBorderColor = &pConfiguration->colorBorder;
	pSeparatorColor = &pConfiguration->colorSeparator;

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

PromptFrame PromptFrame::singleton;
