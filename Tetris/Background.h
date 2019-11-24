#pragma once
#include <string>
#include <Windows.h>
#include "RenderMode.h"
using namespace std;

class Configuration;

class Background
{
public:
	void Initialize(Configuration* pConfiguration);
	static Background singleton;

	wstring pathBackground;
	COLORREF colorBackground;
	RenderMode renderMode;
	RenderAlignment renderAlignment;

private:
	Background() {};
};

