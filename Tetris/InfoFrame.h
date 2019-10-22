#pragma once
#include "Frame.h"
#include "Configuration.h"

class InfoFrame :
	public Frame
{
public:
	void Initialize(Configuration* pConfiguration);
	void SetInfomations(int* pLevel, int* pScore, int* pStartLine);

	static InfoFrame singleton;

	COLORREF colorInfo;
	int* pLevel;
	int* pScore;
	int* pStartLine;
};

