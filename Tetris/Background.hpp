#pragma once
#include <string>
#include <Windows.h>
#include <gdiplus.h>
#include "RenderMode.hpp"
using namespace std;
using Color = Gdiplus::Color;

class Configuration;

class Background
{
public:
	void Initialize(Configuration* pConfiguration);
	static Background singleton;

	wstring pathBackground;
	Color colorBackground;
	RenderMode renderMode;
	RenderAlignmentHorizontal renderAlignmentHorizontal;
	RenderAlignmentVertical renderAlignmentVertical;

private:
	Background() {};
};

