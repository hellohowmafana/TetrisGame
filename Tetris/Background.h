#pragma once
#include <tchar.h>
#include "tcharstd.h"
#include <string>
#include <Windows.h>
using namespace std;

class Configuration;

enum class BackgroundMode
{ Color = 0, General = 1, Strech = 2, Tile = 3, UniformFill = 4, Uniform = 5 };

class Background
{
public:
	void Initialize(Configuration* pConfiguration);
	static Background singleton;

	tstring pathBackground;
	COLORREF colorBackground;
	BackgroundMode backgroundMode;

private:
	Background() {};
};

