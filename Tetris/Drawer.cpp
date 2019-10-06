#include "Drawer.h"

#define CreateSolidPen(hpn, width, color) (hpn) = CreatePen(PS_SOLID, (width), (color))

Drawer Drawer::mainDrawer;

Drawer::Drawer()
:hpnBorder(NULL), hpnSeparator(NULL), hdc(NULL), initialized(false)
{

}

bool Drawer::Initialize(Configuration* pConfiguration)
{
	try {
		this->pConfiguration = pConfiguration;

		COLORREF color;
		GetColorFromFile(pConfiguration->pathFrameColorFile.c_str(), &color);
		CreateSolidPen(hpnBorder, pConfiguration->borderThickness, color);
		GetColorFromFile(pConfiguration->pathSeparatorColorFile.c_str(), &color);
		CreateSolidPen(hpnSeparator, pConfiguration->separatorThickness, color);

		vector<COLORREF> colors;
		GetColorsFromFile(pConfiguration->pathColorFile.c_str(), &colors);
		for (size_t i = 0; i < colors.size(); i++)
		{
			vecColorBrushes.push_back(CreateSolidBrush(colors[i]));
		}
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

		DrawUnit(0, 0, vecColorBrushes[1]);
		DrawUnit(3, 1, vecColorBrushes[0]);
		DrawUnit(1, 2, vecColorBrushes[5]);
		DrawUnit(1, 5, vecColorBrushes[6]);
		DrawUnit(9, 19, vecColorBrushes[3]);
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

bool Drawer::GetColorFromFile(const TCHAR* file, COLORREF* pColor)
{
	try {
		Bitmap* pbmp;
		Color color;
		pbmp = Bitmap::FromFile(file);
		pbmp->GetPixel(0, 0, &color);
		*pColor = color.ToCOLORREF();
		return true;
	}
	catch (...)
	{
		return (COLORREF)0;
	}
}

bool Drawer::GetColorsFromFile(const TCHAR* file, vector<COLORREF>* pvecColors)
{
	try {
		pvecColors->clear();
		Bitmap* pbmp;
		Color color;
		pbmp = Bitmap::FromFile(file);
		for (UINT i = 0; i < pbmp->GetWidth(); i++)
		{
			for (UINT j = 0; j < pbmp->GetHeight(); j++)
			{
				pbmp->GetPixel(i, j, &color);
				if (pvecColors->end() ==
					find(pvecColors->begin(), pvecColors->end(), color.ToCOLORREF()))
				{
					pvecColors->push_back(color.ToCOLORREF());
				}
			}
		}
		return true;
	}
	catch (...)
	{
		pvecColors->clear();
		return false;
	}
}

Drawer::~Drawer()
{
	DeleteObject(hpnBorder);
	hpnBorder = NULL;
	DeleteObject(hpnSeparator);
	hpnSeparator = NULL;
}
