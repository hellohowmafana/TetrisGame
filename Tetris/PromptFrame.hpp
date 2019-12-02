#pragma once
#include "UnitFrame.hpp"
#include "TetrisShape.hpp"
#include "Configuration.hpp"

class GameFrame;

class PromptFrame :	public UnitFrame
{
private:
	TetrisShape tetrisShape;

public:
	bool Initialize(Configuration* pConfiguration);
	void SetTetrisShape(TetrisShape* pTetrisShape);
	TetrisShape* GetTerisShape();
	void SetGameFrame(GameFrame* pGameFrame);
	GameFrame* GetGameFrame();
	static PromptFrame singleton;

private:
	PromptFrame() {};
	TetrisShape* pTetrisShape;
	GameFrame* pGameFrame;
};

