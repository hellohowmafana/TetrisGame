#pragma once
#include <vector>
#include "GameFrame.h"
#include "PromptFrame.h"
using namespace std;

class Drawer
{
public:
	static Drawer singleton;

public:
	bool Initialize(GameFrame* pGameFrame, PromptFrame* pPromptFrame);
	void AttachDC(HDC hdc);
	void DetachDC();
	void DrawElements();

private:
	Drawer();
	~Drawer();

	bool IsValid();

	void DrawFrame(Frame* pFrame);
	void DrawBorder(UnitFrame* pUnitFrame);
	void DrawSeparators(UnitFrame* pUnitFrame);

	void DrawPromptFrame();
	void DrawPromptUnit(int x, int y, HBRUSH brush);
	void DrawPromptShape();

	void DrawInfoFrameFrame();

	void DrawBackgroud();

	void DrawShape(UnitFrame* pUnitFrame, TetrisShape* pTetrisShape);
	void DrawMass(GameFrame* pGameFrame, Mass* pMass);
	void DrawMassLine(GameFrame* pGameFrame, MassLine* pMassLine, int y);
	void DrawUnit(UnitFrame* pUnitFrame, int x, int y, HBRUSH brush);

	void GetDCSize(HDC hdc, LONG * pWidth, LONG * pHeight);
	HBRUSH GetRandomTetrisBrush();

private:
	GameFrame* pGameFrame;
	PromptFrame* pPromptFrame;
	HDC hdc;
	HPEN hpnBorder;
	HPEN hpnSeparator;
	vector<HBRUSH> vecTetrisBrushes;
	HBRUSH hbsMass;
	bool initialized;

	HDC hdcCmp;
	HBITMAP hbmCmp;
	LONG dcWidth, dcHeight;
};

