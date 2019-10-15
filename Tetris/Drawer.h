#pragma once
#include <vector>
#include "GameFrame.h"
using namespace std;

class Drawer
{
public:
	static Drawer singleton;

public:
	bool Initialize(GameFrame* pConfiguration);
	void AttachDC(HDC hdc);
	void DetachDC();
	void DrawElements();

private:
	Drawer();

	void DrawFrame();
	void DrawBorder();
	void DrawSeparators();

	void DrawPromptFrame();
	void DrawPromptUnit(int x, int y, HBRUSH brush);
	void DrawPromptShape();

	void DrawInfoFrameFrame();

	void DrawBackgroud();

	void DrawShape(TetrisShape* pTetrisShape);
	void DrawMass(Mass* pMass);
	void DrawMassLine(MassLine* pMassLine, int y);
	void DrawUnit(int x, int y, HBRUSH brush);

	~Drawer();

private:
	GameFrame* pGameFrame;
	HDC hdc;
	HPEN hpnBorder;
	HPEN hpnSeparator;
	vector<HBRUSH> vecTetrisBrushes;
	HBRUSH hbsMass;

	bool initialized;
};

