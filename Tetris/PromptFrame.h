#pragma once
#include "UnitFrame.h"
#include "TetrisShape.h"
class PromptFrame :	public UnitFrame
{
private:
	TetrisShape tetrisShape;

public:
	bool Initialize();
};

