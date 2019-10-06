#pragma once
#include <Windows.h>
#include <vector>
#include <Unknwn.h>
#include <gdiplus.h>
#include "Configuration.h"
using namespace std;
using namespace Gdiplus;

class Drawer
{
public:
	static Drawer mainDrawer;

public:
	bool Initialize(Configuration* pConfiguration);
	void AttachDC(HDC hdc);
	void DetachDC();
	void DrawElements();

private:
	Drawer();

	void DrawFrame();
	void DrawBorder();
	void DrawSeparators();
	void DrawUnit(int x, int y, HBRUSH brush);
	void DrawShape();

	void DrawPromptFrame();
	void DrawPromptUnit(int x, int y, HBRUSH brush);
	void DrawPromptShape();

	void DrawInfoFrameFrame();

	void DrawBackgroud();

	bool GetColorFromFile(const TCHAR* file, COLORREF* pColor);
	bool GetColorsFromFile(const TCHAR* file, vector<COLORREF>* pvecColors);
	~Drawer();

private:
	Configuration* pConfiguration;
	HDC hdc;
	HPEN hpnBorder;
	HPEN hpnSeparator;
	vector<HBRUSH> vecColorBrushes;

	bool initialized;
};

