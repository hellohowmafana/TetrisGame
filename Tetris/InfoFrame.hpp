#pragma once
#include "Frame.hpp"
#include "Configurable.hpp"

class GameFrame;

class InfoFrame :
	public Frame, public Configurable
{
public:
	bool OnUpdate(Configuration* pConfiguration);
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

