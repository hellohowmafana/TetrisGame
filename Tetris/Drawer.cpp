#include "Drawer.h"

#define CreateSolidPen(width, color) CreatePen(PS_SOLID, (width), (color))

Drawer Drawer::singleton;

Drawer::Drawer()
:hpnBorder(NULL), hpnSeparator(NULL), hdc(NULL), initialized(false)
{

}

bool Drawer::Initialize(Configuration* pConfiguration)
{
	try {
		this->pConfiguration = pConfiguration;

		hpnBorder = CreateSolidPen(pConfiguration->borderThickness, pConfiguration->colorBorder);
		hpnSeparator = CreateSolidPen(pConfiguration->separatorThickness, pConfiguration->colorSeparator);

		for (size_t i = 0; i < pConfiguration->vecTetrisColors.size(); i++)
		{
			vecTetrisBrushes.push_back(CreateSolidBrush(pConfiguration->vecTetrisColors[i]));
		}

		hbsAccretion = CreateSolidBrush(pConfiguration->colorAccretion);
	}
	catch (...){
		return false;
	}

	initialized = true;

	return true;
}

void Drawer::AttachDC(HDC hdc)
{
	this->hdc = hdc;
}

void Drawer::DetachDC()
{
	this->hdc = NULL;
}

void Drawer::DrawElements()
{
	if (initialized && hdc)
	{
		DrawFrame();

		DrawUnit(0, 0, vecTetrisBrushes[1]);
		DrawUnit(3, 1, vecTetrisBrushes[0]);
		DrawUnit(1, 2, vecTetrisBrushes[5]);
		DrawUnit(1, 5, vecTetrisBrushes[6]);
		DrawUnit(9, 19, vecTetrisBrushes[3]);
	}
}

void Drawer::DrawFrame()
{
	if (!initialized || NULL == hdc) return;

	DrawBorder();
	DrawSeparators();
}

void Drawer::DrawBorder()
{
	if (!initialized || NULL == hdc) return;

	if (pConfiguration->borderThickness == 0) return;

	SelectObject(hdc, hpnBorder);
	int innerWidth = (pConfiguration->unitWidth + pConfiguration->separatorThickness) * pConfiguration->frameSizeX
		+ pConfiguration->separatorThickness;
	int innerHeight = (pConfiguration->unitWidth + pConfiguration->separatorThickness) * pConfiguration->frameSizeY
		+ pConfiguration->separatorThickness;
	int totalWidth = pConfiguration->borderThickness * 2 + innerWidth;
	int totalHeight = pConfiguration->borderThickness * 2 + innerHeight;
	int left = pConfiguration->frameLeft + pConfiguration->borderThickness / 2;
	int top = pConfiguration->frameTop + pConfiguration->borderThickness / 2;
	int width = totalWidth - pConfiguration->borderThickness;
	int height = totalHeight - pConfiguration->borderThickness;
	Rectangle(hdc, left, top, left + width + 1, top + height + 1);
}

void Drawer::DrawSeparators()
{
	if (!initialized || NULL == hdc) return;

	if (pConfiguration->separatorThickness == 0) return;

	SelectObject(hdc, hpnSeparator);

	int top = pConfiguration->frameTop + pConfiguration->borderThickness;
	int bottom = top +
		(pConfiguration->unitWidth + pConfiguration->separatorThickness) * pConfiguration->frameSizeY
		+ pConfiguration->separatorThickness;

	for (int n = 0; n <= pConfiguration->frameSizeX; n++)
	{
		int x1 = pConfiguration->frameLeft + pConfiguration->borderThickness
			+ n * (pConfiguration->unitWidth + pConfiguration->separatorThickness)
			+ pConfiguration->separatorThickness / 2;
		int x2 = x1 + 1;
		Rectangle(hdc, x1, top, x2, bottom + 1);
	}

	int left = pConfiguration->frameLeft + pConfiguration->borderThickness;
	int right = left +
		(pConfiguration->unitWidth + pConfiguration->separatorThickness) * pConfiguration->frameSizeX
		+ pConfiguration->separatorThickness;

	for (int n = 0; n <= pConfiguration->frameSizeY; n++)
	{
		int y1 = pConfiguration->frameTop + pConfiguration->borderThickness
			+ n * (pConfiguration->unitWidth + pConfiguration->separatorThickness)
			+ pConfiguration->separatorThickness / 2;
		int y2 = y1 + 1;
		Rectangle(hdc, left, y1, right, y2);
	}
}

void Drawer::DrawBackgroud()
{
	if (!initialized || NULL == hdc) return;
}

void Drawer::DrawTetrisShape(TetrisShape* pTetrisShape)
{
	for (int i = pTetrisShape->GetLeft(); i < pTetrisShape->GetRight(); i++)
	{
		for (int j = pTetrisShape->GetTop(); j < pTetrisShape->GetBottom(); j++)
		{
			DrawUnit(i, j, vecTetrisBrushes[pTetrisShape->GetColor()]);
		}
	}
}

void Drawer::DrawAccretion(Accretion* pAccretion)
{
	for (size_t i = pAccretion->GetTop(); i < (size_t)Configuration::singleton.frameSizeY; i++)
	{
		for (size_t j = 0; j < (size_t)Configuration::singleton.frameSizeX; j++)
		{
			if (pAccretion->IsSolid(j, i))
				DrawUnit(j, i, hbsAccretion);
		}
	}
}

void Drawer::DrawUnit(int x, int y, HBRUSH brush)
{
	if (!initialized || NULL == hdc) return;
	if (x < 0 || x >= pConfiguration->frameSizeX
		|| y < 0 || y >= pConfiguration->frameSizeY)
		return;

	LONG left = pConfiguration->frameLeft + pConfiguration->borderThickness
		+ pConfiguration->separatorThickness * (x + 1) + pConfiguration->unitWidth * x;
	LONG right = left + pConfiguration->unitWidth;
	LONG top = pConfiguration->frameTop + pConfiguration->borderThickness
		+ pConfiguration->separatorThickness * (y + 1) + pConfiguration->unitWidth * y;
	LONG bottom = top + pConfiguration->unitWidth;
	RECT rect = { left, top, right, bottom };
	FillRect(hdc, &rect, brush);
}

void Drawer::DrawShape()
{
	if (!initialized || NULL == hdc) return;
}

void Drawer::DrawPromptFrame()
{
}

void Drawer::DrawPromptUnit(int x, int y, HBRUSH brush)
{
}

void Drawer::DrawPromptShape()
{
}

void Drawer::DrawInfoFrameFrame()
{
}

Drawer::~Drawer()
{
	DeleteObject(hpnBorder);
	hpnBorder = NULL;
	DeleteObject(hpnSeparator);
	hpnSeparator = NULL;
}
