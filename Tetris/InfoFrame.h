#pragma once
#include "Frame.h"
#include "Configuration.h"

class GameFrame;

class InfoFrame :
	public Frame
{
public:
	void Initialize(Configuration* pConfiguration);
	void SetGameFrame(GameFrame* pGameFrame);
	GameFrame* GetGameFrame();
	int GetLevel();
	int GetScore();
	int GetStartLine();

	static InfoFrame singleton;

	COLORREF colorInfo;
	wstring fontFace;
	int fontHeight;
	int fontWidth;
	int fontWeight;

	GameFrame* pGameFrame;
};

