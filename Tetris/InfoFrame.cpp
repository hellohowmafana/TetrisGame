#include "InfoFrame.hpp"
#include "GameFrame.hpp"

bool InfoFrame::OnUpdate(Configuration* pConfiguration)
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

	return true;
}

void InfoFrame::SetGameFrame(GameFrame* pGameFrame)
{
	this->pGameFrame = pGameFrame;
}

GameFrame* InfoFrame::GetGameFrame()
{
	return pGameFrame;
}

int InfoFrame::GetLevel()
{
	return pGameFrame->level;
}

int InfoFrame::GetScore()
{
	return pGameFrame->score;
}

int InfoFrame::GetStartLine()
{
	return pGameFrame->startLine;
}

InfoFrame InfoFrame::singleton;