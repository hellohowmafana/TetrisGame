#include "Drawer.h"

#define CreateSolidPen(width, color) CreatePen(PS_SOLID, (width), (color))

Drawer Drawer::singleton;

Drawer::Drawer()
:hpnBorder(NULL), hpnSeparator(NULL), hdc(NULL), initialized(false)
{

}

bool Drawer::Initialize(GameFrame* pGameFrame)
{
	try {
		this->pGameFrame = pGameFrame;

		hpnBorder = CreateSolidPen(pGameFrame->borderThickness, *pGameFrame->pBorderColor);
		hpnSeparator = CreateSolidPen(pGameFrame->separatorThickness, *pGameFrame->pSeparatorColor);

		for (size_t i = 0; i < pGameFrame->pTetrisColors->size(); i++)
		{
			vecTetrisBrushes.push_back(CreateSolidBrush(pGameFrame->pTetrisColors->at(i)));
		}

		hbsMass = CreateSolidBrush(*pGameFrame->pMassColor);
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

		/*DrawUnit(0, 0, vecTetrisBrushes[1]);
		DrawUnit(3, 1, vecTetrisBrushes[0]);
		DrawUnit(1, 2, vecTetrisBrushes[5]);
		DrawUnit(1, 5, vecTetrisBrushes[6]);
		DrawUnit(9, 19, vecTetrisBrushes[3]);*/

		//TetrisShape t1;
		//t1.Initialize(TetrisType::GetTetrisType(_T("classic"), _T("T")), TetrisRotation::Rotation0);
		//t1.SetGameFrame(&GameFrame::singleton);
		//t1.Move(4,4);
		//DrawShape(&t1);

		//TetrisShape t2;
		//t2.Initialize(TetrisType::GetTetrisType(_T("classic"), _T("Z")), TetrisRotation::Rotation0);
		//t2.SetGameFrame(&GameFrame::singleton);
		//DrawShape(&t2);

		TetrisShape t3;
		//t3.Initialize(TetrisType::GetTetrisType(_T("classic"), _T("L")), TetrisRotation::Rotation3);
		//t3.SetGameFrame(&GameFrame::singleton);
		//t3.Move(2, 2);
		//DrawShape(&t3);
		DrawShape(pGameFrame->GetShape());
		//DrawMass(pGameFrame->GetMass());
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

	if (pGameFrame->borderThickness == 0) return;

	SelectObject(hdc, hpnBorder);
	int innerWidth = (pGameFrame->unitWidth + pGameFrame->separatorThickness) * pGameFrame->sizeX
		+ pGameFrame->separatorThickness;
	int innerHeight = (pGameFrame->unitWidth + pGameFrame->separatorThickness) * pGameFrame->sizeY
		+ pGameFrame->separatorThickness;
	int totalWidth = pGameFrame->borderThickness * 2 + innerWidth;
	int totalHeight = pGameFrame->borderThickness * 2 + innerHeight;
	int left = pGameFrame->left + pGameFrame->borderThickness / 2;
	int top = pGameFrame->top + pGameFrame->borderThickness / 2;
	int width = totalWidth - pGameFrame->borderThickness;
	int height = totalHeight - pGameFrame->borderThickness;
	Rectangle(hdc, left, top, left + width + 1, top + height + 1);
}

void Drawer::DrawSeparators()
{
	if (!initialized || NULL == hdc) return;

	if (pGameFrame->separatorThickness == 0) return;

	SelectObject(hdc, hpnSeparator);

	int top = pGameFrame->top + pGameFrame->borderThickness;
	int bottom = top +
		(pGameFrame->unitWidth + pGameFrame->separatorThickness) * pGameFrame->sizeY
		+ pGameFrame->separatorThickness;

	for (int n = 0; n <= pGameFrame->sizeX; n++)
	{
		int x1 = pGameFrame->left + pGameFrame->borderThickness
			+ n * (pGameFrame->unitWidth + pGameFrame->separatorThickness)
			+ pGameFrame->separatorThickness / 2;
		int x2 = x1 + 1;
		Rectangle(hdc, x1, top, x2, bottom + 1);
	}

	int left = pGameFrame->left + pGameFrame->borderThickness;
	int right = left +
		(pGameFrame->unitWidth + pGameFrame->separatorThickness) * pGameFrame->sizeX
		+ pGameFrame->separatorThickness;

	for (int n = 0; n <= pGameFrame->sizeY; n++)
	{
		int y1 = pGameFrame->top + pGameFrame->borderThickness
			+ n * (pGameFrame->unitWidth + pGameFrame->separatorThickness)
			+ pGameFrame->separatorThickness / 2;
		int y2 = y1 + 1;
		Rectangle(hdc, left, y1, right, y2);
	}
}

void Drawer::DrawBackgroud()
{
	if (!initialized || NULL == hdc) return;
}

void Drawer::DrawShape(TetrisShape* pTetrisShape)
{
	if (!initialized || NULL == hdc) return;

	for (int i = pTetrisShape->GetLeft(); i <= pTetrisShape->GetRight(); i++)
	{
		for (int j = pTetrisShape->GetTop(); j <= pTetrisShape->GetBottom(); j++)
		{
			if(pTetrisShape->IsSolid(i, j, true))
				DrawUnit(i, j, vecTetrisBrushes[pTetrisShape->GetColor()]);
		}
	}
}

void Drawer::DrawMass(Mass* pMass)
{
	if (!initialized || NULL == hdc) return;

	//for (pMass->GetFirstLineIterator(); i = pMass->GetTop(); i < pMass->GetBottom(); i++)
	//{
	//	for (int j = 0; j < pMass->GetRight(); j++)
	//	{
	//		if (pMass->IsSolid(j, i))
	//			DrawUnit(j, i, hbsMass);
	//	}
	//}
}

void Drawer::DrawUnit(int x, int y, HBRUSH brush)
{
	if (!initialized || NULL == hdc) return;

	if (x < 0 || x >= pGameFrame->sizeX
		|| y < 0 || y >= pGameFrame->sizeY)
		return;

	LONG left = pGameFrame->left + pGameFrame->borderThickness
		+ pGameFrame->separatorThickness * (x + 1) + pGameFrame->unitWidth * x;
	LONG right = left + pGameFrame->unitWidth;
	LONG top = pGameFrame->top + pGameFrame->borderThickness
		+ pGameFrame->separatorThickness * (y + 1) + pGameFrame->unitWidth * y;
	LONG bottom = top + pGameFrame->unitWidth;
	RECT rect = { left, top, right, bottom };
	FillRect(hdc, &rect, brush);
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
