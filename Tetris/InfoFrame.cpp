#include "InfoFrame.h"

void InfoFrame::Initialize(Configuration* pConfiguration)
{
	left = pConfiguration->infoFrameLeft;
	top = pConfiguration->infoFrameTop;
	sizeX = pConfiguration->infoFrameSizeX;
	sizeY = pConfiguration->infoFrameSizeY;
	colorInfo = pConfiguration->colorInfo;
	fontFace = pConfiguration->infoFontFace;
	fontHeight = pConfiguration->infoFontHeight;
	fontWidth = pConfiguration->infoFontWidth;
	fontWeight = pConfiguration->infoFontWeight;
}

void InfoFrame::SetInfomations(int* pLevel, int* pScore, int* pStartLine)
{
	this->pLevel = pLevel;
	this->pScore = pScore;
	this->pStartLine = pStartLine;
}

InfoFrame InfoFrame::singleton;