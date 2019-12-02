#pragma once
#include "Frame.hpp"
#include "Configuration.hpp"

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

	Color colorInfo;
	wstring fontFace;
	int fontHeight;
	int fontWidth;
	int fontWeight;

	GameFrame* pGameFrame;
};

