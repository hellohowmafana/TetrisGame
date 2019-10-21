#pragma once
#include "UnitFrame.h"
#include "TetrisShape.h"
#include "Configuration.h"
class PromptFrame :	public UnitFrame
{
private:
	TetrisShape tetrisShape;

public:
	bool Initialize(Configuration* pConfiguration);
	void SetTetrisShape(TetrisShape* pTetrisShape);
	TetrisShape* GetTerisShape();
	static PromptFrame singleton;

private:
	PromptFrame() {};
	TetrisShape* pTetrisShape;
};

