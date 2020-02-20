#pragma once
#include "UnitFrame.hpp"
#include "TetrisShape.hpp"
#include "Configuration.hpp"
#include "Configurable.hpp"

class GameFrame;

class PromptFrame :	public UnitFrame, public Configurable
{
private:
	TetrisShape tetrisShape;

public:
	bool OnUpdate(Configuration* pConfiguration);
	void SetTetrisShape(TetrisShape* pTetrisShape);
	TetrisShape* GetTerisShape();
	void SetGameFrame(GameFrame* pGameFrame);
	GameFrame* GetGameFrame();
	static PromptFrame singleton;

private:
	PromptFrame() = default;
	TetrisShape* pTetrisShape;
	GameFrame* pGameFrame;
};

