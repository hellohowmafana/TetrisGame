#include "Drawer.h"
#include "Utility.h"
#include "Controller.h"
#include <bitset>

Drawer Drawer::singleton;

Drawer::Drawer() :
	pGameFrame(nullptr),
	hdc(NULL), hdcCmp(NULL), hbmCmp(NULL), initialized(false),
	dcWidth(0), dcHeight(0),
	pGifPollerBackground(nullptr)
{

}

Drawer::~Drawer()
{
	if (attached)
	{
		DetachDC();
	}

	if (initialized)
	{
		Deinitialize();
	}
}

bool Drawer::Initialize(Controller* pController, GameFrame* pGameFrame,
	PromptFrame* pPromptFrame, InfoFrame*pInfoFrame, Background* pBackground)
{
	if (NULL == hWnd)
		return false;
	try {
		this->pController = pController;
		this->pGameFrame = pGameFrame;
		this->pPromptFrame = pPromptFrame;
		this->pInfoFrame = pInfoFrame;
		this->pBackground = pBackground;

		HDC hdc = GetDC(hWnd);

		pbrsFrame = new SolidBrush(pGameFrame->backgroundColor);
		ppenBorder = new Pen(pGameFrame->borderColor, (REAL)pGameFrame->borderThickness);
		ppenBorder->SetAlignment(PenAlignmentInset);
		ppenSeprator = new Pen(pGameFrame->separatorColor, (REAL)pGameFrame->separatorThickness);

		if (pGameFrame->useColor)
		{
			for (size_t i = 0; i < pGameFrame->tetrisColors.size(); i++)
			{
				vecTetrisBrushes.push_back(new SolidBrush(pGameFrame->tetrisColors.at(i)));
				if (!pGameFrame->useMassColor)
					vecTetrisBrushesLight.push_back(new SolidBrush(LightColor(pGameFrame->tetrisColors.at(i), 0.5)));
			}

			if (pGameFrame->useMassColor)
			{
				pbrsMass = new SolidBrush(pGameFrame->massColor);
				pbrsMassLight = new SolidBrush(LightColor(pGameFrame->massColor, 0.5));
			}
		}
		else
		{
			Bitmap bitmapUnit(pGameFrame->pathUnitBitmap.c_str());
			pbmpUnit = StretchBitmap(hdc, &bitmapUnit, pGameFrame->unitWidth, pGameFrame->unitWidth);
			pbmpUnitLight = LightBitmap(hdc, &bitmapUnit, 0.5);
		}

		if (RenderMode::Color != pBackground->renderMode)
		{
			pbmpBackground = new Bitmap(pBackground->pathBackground.c_str());
			if (Status::Ok != pbmpBackground->GetLastStatus())
				return false;
			SetBitmapDCResolution(pbmpBackground, hdc);
			if (AnimatedGifPoller::IsAnimatedGif(pbmpBackground))
			{
				pGifPollerBackground = new AnimatedGifPoller(pbmpBackground, BackgroundFrameChangedProcStatic);
				pGifPollerBackground->SetLoopInfinate(true);
				pGifPollerBackground->Start();
			}
			isAnimatedBackground = AnimatedGifPoller::IsAnimatedGif(pbmpBackground);
		}

		LOGFONT logFont;
		logFont.lfHeight = pInfoFrame->fontHeight;
		logFont.lfWidth = pInfoFrame->fontWidth;
		logFont.lfEscapement = 0;
		logFont.lfOrientation = 0;
		logFont.lfWeight = pInfoFrame->fontWeight * 100;
		logFont.lfItalic = FALSE;
		logFont.lfUnderline = FALSE;
		logFont.lfStrikeOut = FALSE;
		logFont.lfCharSet = DEFAULT_CHARSET;
		logFont.lfOutPrecision=OUT_DEFAULT_PRECIS;
		logFont.lfClipPrecision=CLIP_DEFAULT_PRECIS;
		logFont.lfQuality=DEFAULT_QUALITY;
		logFont.lfPitchAndFamily=FF_DONTCARE;
		wcscpy(logFont.lfFaceName, pInfoFrame->fontFace.c_str());
		pfntInfo = new Font(hdc, &logFont);
		pbrsInfo = new SolidBrush(pInfoFrame->colorInfo);

		pbmpBegin = new Bitmap(pGameFrame->pathBeginSplash.c_str());
		if (Status::Ok != pbmpBegin->GetLastStatus())
			return false;
		pbmpGameOver = new Bitmap(pGameFrame->pathGameOverSplash.c_str());
		if (Status::Ok != pbmpGameOver->GetLastStatus())
			return false;

		pbmpPause = new Bitmap(pGameFrame->pathPauseIcon.c_str());
		if (Status::Ok != pbmpPause->GetLastStatus())
			return false;
		pbmpResume = new Bitmap(pGameFrame->pathResumeIcon.c_str());
		if (Status::Ok != pbmpResume->GetLastStatus())
			return false;

		pbrsMask = new SolidBrush(Color((BYTE)(255 * pGameFrame->maskTransparency), 255, 255, 255));

		ReleaseDC(hWnd, hdc);
	}
	catch (...){
		return false;
	}

	initialized = true;

	return true;
}

bool Drawer::Deinitialize()
{
	if (!initialized) return false;

	delete pbrsFrame;
	pbrsFrame = nullptr;
	delete ppenBorder;
	ppenBorder = nullptr;
	delete ppenSeprator;
	ppenSeprator = nullptr;
	if (pGameFrame->useColor)
	{
		for (size_t i = 0; i < vecTetrisBrushes.size(); i++)
		{
			delete vecTetrisBrushes.at(i);
			if (!pGameFrame->useMassColor)
				delete vecTetrisBrushesLight.at(i);
		}
		vecTetrisBrushes.clear();
		if (!pGameFrame->useMassColor)
			vecTetrisBrushesLight.clear();

		if (pGameFrame->useMassColor)
		{
			delete pbrsMass;
			pbrsMass = nullptr;
		}
	}
	else
	{
		delete pbmpUnit;
		pbmpUnit = nullptr;
		delete pbmpUnitLight;
		pbmpUnitLight = nullptr;
	}

	if (RenderMode::Color != pBackground->renderMode)
	{
		delete pbmpBackground;
		pbmpBackground = nullptr;
		if (pGifPollerBackground)
		{
			delete pGifPollerBackground;
			pGifPollerBackground = nullptr;
		}
	}

	delete pfntInfo;
	pfntInfo = nullptr;
	delete pbrsInfo;
	pbrsInfo = nullptr;

	delete pbmpBegin;
	pbmpBegin = nullptr;
	delete pbmpGameOver;
	pbmpGameOver = nullptr;

	delete pbmpPause;
	pbmpPause = nullptr;
	delete pbmpResume;
	pbmpResume = nullptr;

	delete pbrsMask;
	pbrsMask = nullptr;

	this->pGameFrame = nullptr;
	this->pPromptFrame = nullptr;
	this->pInfoFrame = nullptr;
	this->pBackground = nullptr;

	initialized = false;

	return true;
}

void Drawer::SetHWnd(HWND hWnd)
{
	this->hWnd = hWnd;
}

void Drawer::AttachDC(HDC hdc)
{
	this->hdc = hdc;
	GetDCSize(hdc, &dcWidth, &dcHeight);
	
	hdcCmp = CreateCompatibleDC(hdc);
	SaveDC(hdcCmp);
	
	hbmCmp = CreateCompatibleBitmap(hdc, dcWidth, dcHeight);
	SelectObject(hdcCmp, hbmCmp);
	
	attached = true;
}

void Drawer::DetachDC()
{
	this->hdc = NULL;
	dcWidth = dcHeight = 0;
	
	RestoreDC(hdcCmp, 1);
	DeleteDC(hdcCmp);
	hdcCmp = NULL;

	DeleteObject(hbmCmp);
	hbmCmp = NULL;
	
	attached = false;
}

bool Drawer::IsInitialized()
{
	return initialized;
}

void Drawer::DrawElements()
{
	if (IsValid())
	{
		DrawBackground();
		DrawFrame(pGameFrame);
		DrawFrame(pPromptFrame);
		DrawFrame(pInfoFrame);
		DrawInfo(pInfoFrame);

		if (pController->IsStarted())
		{
			DrawShape(pGameFrame, pGameFrame->GetShape());
			DrawMass(pGameFrame, pGameFrame->GetMass());
			DrawMask(pGameFrame);
			DrawIcon(pGameFrame);
		
			DrawShape(pPromptFrame, pPromptFrame->GetTerisShape());
		}
		else
		{
			if(GameState::None == pController->GetGameState())
				DrawFill(pGameFrame, 0);
			else
				DrawRollingLines(pGameFrame);
			DrawSplash(pGameFrame);
		}

		BitBlt(hdc, 0, 0, dcWidth, dcHeight, hdcCmp, 0, 0, SRCCOPY);
	}
}

void Drawer::Invalidate()
{
	InvalidateRgn(hWnd, NULL, FALSE);
}

void Drawer::DrawSolidColor(const Color& color, const Rect& rect)
{
	if (!IsValid()) return;

	Graphics grp(hdcCmp);
	SolidBrush brs(color);
	grp.FillRectangle(&brs, rect);
}

void Drawer::DrawBitmap(Bitmap* pBitmap, RenderMode renderMode,
	RenderAlignmentHorizontal renderAlignmentHorizontal,
	RenderAlignmentVertical renderAlignmentVertical, const Rect& rect)
{
	if (!IsValid()) return;

	if (RenderMode::Color == renderMode)
		return;

	switch (renderMode)
	{
	case RenderMode::General:
	{
		int x = 0;
		switch (renderAlignmentHorizontal)
		{
		case RenderAlignmentHorizontal::Left:
			x = rect.X;
			break;
		case RenderAlignmentHorizontal::Right:
			x = rect.Width - (int)pBitmap->GetWidth() + rect.X;
			break;
		case RenderAlignmentHorizontal::Center:
			x = (rect.Width - (int)pBitmap->GetWidth()) / 2 + rect.X;
			break;
		default:
			break;
		}
		int y = 0;
		switch (renderAlignmentVertical)
		{
		case RenderAlignmentVertical::Top:
			y = rect.Y;
			break;
		case RenderAlignmentVertical::Bottom:
			y = rect.Height - (int)pBitmap->GetHeight() + rect.Y;
			break;
		case RenderAlignmentVertical::Center:
			y = (rect.Height - (int)pBitmap->GetHeight()) / 2 + rect.Y;
			break;
		default:
			break;
		}
		Graphics(hdcCmp).DrawImage(pBitmap, x, y);
		break;
	}
	case RenderMode::Strech:
	{
		Graphics(hdcCmp).DrawImage(pBitmap, rect.X, rect.Y, rect.Width, rect.Height);
		break;
	}
	case RenderMode::Tile:
	{
		TextureBrush* pbrsTile = nullptr;
		if (AnimatedGifPoller::IsAnimatedGif(pBitmap))
		{
			Bitmap bmpBrush(pBitmap->GetWidth(), pBitmap->GetHeight(), pBitmap->GetPixelFormat());
			Graphics grpBrush(&bmpBrush);
			grpBrush.DrawImage(pBitmap,0,0);
			pbrsTile = new TextureBrush(&bmpBrush);
		}
		else
		{
			pbrsTile = new TextureBrush(pBitmap);
		}
		Graphics(hdcCmp).FillRectangle(pbrsTile, rect);
		delete pbrsTile;
		break;
	}
	case RenderMode::UniformFill:
	{
		int bitmapWidth = pBitmap->GetWidth();
		int bitmapHeight = pBitmap->GetHeight();
		int dstWidth = rect.Width;
		int dstHeight = rect.Height;

		REAL rateWidth = (REAL)bitmapWidth / dstWidth;
		REAL rateHeight = (REAL)bitmapHeight / dstHeight;
		REAL rate = rateWidth < rateHeight ? rateWidth : rateHeight;

		bool fitWidth = rate == rateWidth;
		REAL srcWidth = fitWidth ? bitmapWidth : dstWidth * rate;
		REAL srcHeight = !fitWidth ? bitmapHeight : dstHeight * rate;
		REAL srcX = fitWidth ? 0 : (bitmapWidth / rate - dstWidth) / 2;
		REAL srcY = !fitWidth ? 0 : (bitmapHeight / rate - dstHeight) / 2;

		Graphics(hdcCmp).DrawImage(pBitmap,
			RectF((REAL)rect.X, (REAL)rect.Y, (REAL)dstWidth, (REAL)dstHeight),
			srcX, srcY, srcWidth, srcHeight, Unit::UnitPixel);

		break;
	}
	case RenderMode::Uniform:
	{
		int bitmapWidth = pBitmap->GetWidth();
		int bitmapHeight = pBitmap->GetHeight();
		int dstWidth = rect.Width;
		int dstHeight = rect.Height;

		REAL rateWidth = (REAL)bitmapWidth / dstWidth;
		REAL rateHeight = (REAL)bitmapHeight / dstHeight;
		REAL rate = rateWidth > rateHeight ? rateWidth : rateHeight;

		bool fitWidth = rate == rateWidth;
		REAL width = fitWidth ? dstWidth : (bitmapWidth / rate);
		REAL height = !fitWidth ? dstHeight : (bitmapHeight / rate);
		REAL x = (fitWidth ? 0 : (dstWidth - width) / 2) + rect.X;
		REAL y = (!fitWidth ? 0 : (dstHeight - height) / 2) + rect.Y;

		Graphics(hdcCmp).DrawImage(pBitmap, x, y, width, height);

		break;
	}
	}
}

void Drawer::DrawBackground()
{
	if (!IsValid()) return;

	// cover background with solid color
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	Rect rect(clientRect.left, clientRect.top,
		clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
	DrawSolidColor(pBackground->colorBackground, rect);

	// draw bitmap with mode
	if(RenderMode::Color != pBackground->renderMode)
		DrawBitmap(pbmpBackground,
			pBackground->renderMode,
			pBackground->renderAlignmentHorizontal,
			pBackground->renderAlignmentVertical, rect);
}

void Drawer::DrawFrame(Frame* pFrame)
{
	if (!IsValid()) return;

	if (dynamic_cast<const UnitFrame*>(pFrame) != nullptr)
	{
		UnitFrame* pUnitFrame = (UnitFrame*)pFrame;
		DrawBackground(pFrame);
		DrawBorder(pUnitFrame);
		DrawSeparators(pUnitFrame);
	}
	if (dynamic_cast<const InfoFrame*>(pFrame) != nullptr)
	{
	}
}

void Drawer::DrawBackground(Frame* pFrame)
{
	if (!IsValid()) return;

	Rect rect(pFrame->GetLeft(), pFrame->GetTop(), pFrame->GetWidth(), pFrame->GetHeight());
	Graphics(hdcCmp).FillRectangle(pbrsFrame, rect);
}

void Drawer::DrawBorder(UnitFrame* pUnitFrame)
{
	if (!IsValid()) return;

	if (pUnitFrame->borderThickness == 0) return;

	int x = pUnitFrame->GetLeft();
	int y = pUnitFrame->GetTop();
	int width = pUnitFrame->GetWidth();
	int height = pUnitFrame->GetHeight();

	Graphics(hdcCmp).DrawRectangle(ppenBorder, x, y, width, height);
}

void Drawer::DrawSeparators(UnitFrame* pUnitFrame)
{
	if (!IsValid()) return;

	if (pUnitFrame->separatorThickness == 0) return;

	int top = pUnitFrame->top + pUnitFrame->borderThickness;
	int bottom = top +
		(pUnitFrame->unitWidth + pUnitFrame->separatorThickness) * pUnitFrame->sizeY
		+ pUnitFrame->separatorThickness;

	for (int n = 0; n <= pUnitFrame->sizeX; n++)
	{
		int x = pUnitFrame->left + pUnitFrame->borderThickness
			+ n * (pUnitFrame->unitWidth + pUnitFrame->separatorThickness)
			+ pUnitFrame->separatorThickness / 2;
		Graphics(hdcCmp).DrawLine(ppenSeprator, x, top, x, bottom);
	}

	int left = pUnitFrame->left + pUnitFrame->borderThickness;
	int right = left +
		(pUnitFrame->unitWidth + pUnitFrame->separatorThickness) * pUnitFrame->sizeX
		+ pUnitFrame->separatorThickness;

	for (int n = 0; n <= pUnitFrame->sizeY; n++)
	{
		int y = pUnitFrame->top + pUnitFrame->borderThickness
			+ n * (pUnitFrame->unitWidth + pUnitFrame->separatorThickness)
			+ pUnitFrame->separatorThickness / 2;
		Graphics(hdcCmp).DrawLine(ppenSeprator, left, y, right, y);
	}
}

Rect* Drawer::GetUnitRectangle(UnitFrame* pUnitFrame, int x, int y, Rect* pRect)
{
	INT left = pUnitFrame->left + pUnitFrame->borderThickness
		+ pUnitFrame->separatorThickness * (x + 1) + pUnitFrame->unitWidth * x;
	INT width = pUnitFrame->unitWidth;
	INT top = pUnitFrame->top + pUnitFrame->borderThickness
		+ pUnitFrame->separatorThickness * (y + 1) + pUnitFrame->unitWidth * y;
	INT height = pUnitFrame->unitWidth;
	pRect->X = left;
	pRect->Y = top;
	pRect->Width = width;
	pRect->Height = height;
	return pRect;
}

void Drawer::DrawUnit(UnitFrame* pUnitFrame, int x, int y, Brush* pBrush)
{
	if (!IsValid()) return;

	if (!pUnitFrame->ValidateXY(x, y)) return;

	Rect rect;
	GetUnitRectangle(pUnitFrame, x, y, &rect);
	Graphics(hdcCmp).FillRectangle(pBrush, rect);
}

void Drawer::DrawUnit(UnitFrame* pUnitFrame, int x, int y, Bitmap* pBitmap)
{
	if (!IsValid()) return;

	if (!pUnitFrame->ValidateXY(x, y)) return;

	Rect rect;
	GetUnitRectangle(pUnitFrame, x, y, &rect);
	Graphics(hdcCmp).DrawImage(pBitmap, rect);
}

void Drawer::DrawLine(UnitFrame* pUnitFrame, int y, Brush* pBrush)
{
	if (!IsValid()) return;

	if (!pUnitFrame->ValidateY(y)) return;

	for (int i = 0; i < pUnitFrame->sizeX; i++)
	{
		DrawUnit(pUnitFrame, i, y, pBrush);
	}
}

void Drawer::DrawLine(UnitFrame* pUnitFrame, int y, Bitmap* pBitmap)
{
	if (!IsValid()) return;

	if (!pUnitFrame->ValidateY(y)) return;

	for (int i = 0; i < pUnitFrame->sizeX; i++)
	{
		DrawUnit(pUnitFrame, i, y, pBitmap);
	}
}

void Drawer::DrawUnits(UnitFrame* pUnitFrame, double blankRate, Brush* pBrush)
{
	if (!IsValid()) return;
	
	if (1 == blankRate)
		return;

	int count = pUnitFrame->GetWidth() * pUnitFrame->GetHeight();
	vector<bool> vecSolid(count);
	Utility::RandomTrue(&vecSolid, (1 - blankRate), false);
	for (int i = 0; i < count; i++)
	{
		if (vecSolid[i])
		{
			int x = i % pUnitFrame->GetWidth();
			int y = i / pUnitFrame->GetWidth();
			DrawUnit(pUnitFrame, x, y, pBrush);
		}
	}
}

void Drawer::DrawUnits(UnitFrame* pUnitFrame, double blankRate, Bitmap* pBitmap)
{
	if (!IsValid()) return;
	
	if (1 == blankRate)
		return;

	int count = pUnitFrame->GetWidth() * pUnitFrame->GetHeight();
	vector<bool> vecSolid(count);
	Utility::RandomTrue(&vecSolid, (1 - blankRate), false);
	for (int i = 0; i < count; i++)
	{
		if (vecSolid[i])
		{
			int x = i % pUnitFrame->GetWidth();
			int y = i / pUnitFrame->GetWidth();
			DrawUnit(pUnitFrame, x, y, pBitmap);
		}
	}
}

void Drawer::DrawShape(UnitFrame* pUnitFrame, TetrisShape* pTetrisShape)
{
	if (GameState::None == pController->GetGameState() ||
		GameState::RollDown == pController->GetGameState())
		return;

	if (!IsValid()) return;

	if (nullptr == pTetrisShape) return;

	for (int i = pTetrisShape->GetLeft(); i <= pTetrisShape->GetRight(); i++)
	{
		for (int j = pTetrisShape->GetTop(); j <= pTetrisShape->GetBottom(); j++)
		{
			if (pTetrisShape->IsSolid(i, j, true))
			{
				if (pGameFrame->useColor)
					DrawUnit(pUnitFrame, i, j, vecTetrisBrushes[pTetrisShape->GetColor()]);
				else
					DrawUnit(pUnitFrame, i, j, pbmpUnit);
			}
		}
	}
}

void Drawer::DrawMass(GameFrame* pGameFrame, Mass* pMass)
{
	if (GameState::None == pController->GetGameState() ||
		GameState::RollDown == pController->GetGameState())
		return;

	if (!IsValid()) return;

	if (nullptr == pMass) return;

	int y = pMass->GetTop();
	for (MassBlock::iterator it = pMass->GetTopLineIterator();
		it != pMass->GetBottomLineIterator(); it++)
	{
		DrawMassLine(pGameFrame, *it, y++);
	}
}

void Drawer::DrawMassLine(GameFrame* pGameFrame, MassLine* pMassLine, int y)
{
	if (!IsValid()) return;

	int x = 0;
	for (MassLine::const_iterator it = pMassLine->begin(); it != pMassLine->end(); it++)
	{
		if (it->isSolid)
		{
			bool useColor = pGameFrame->useColor;
			if (useColor)
			{
				Brush* pbrs;
				if (GameState::BlinkLight == pController->GetGameState() && pGameFrame->IsLastFullLine(pMassLine))
					pbrs = pGameFrame->useMassColor ? pbrsMassLight : vecTetrisBrushesLight[it->color];
				else
					pbrs = pGameFrame->useMassColor ? pbrsMass : vecTetrisBrushes[it->color];
				DrawUnit(pGameFrame, x, y, pbrs);
			}
			else
			{
				Bitmap* pbmp;
				if (GameState::BlinkLight == pController->GetGameState() && pGameFrame->IsLastFullLine(pMassLine))
					pbmp = pbmpUnitLight;
				else
					pbmp = pbmpUnit;
				DrawUnit(pGameFrame, x, y, pbmp);
			}
		}
		x++;
	}
}

void Drawer::DrawFill(UnitFrame* pUnitFrame, double blankRate)
{
	if (pGameFrame->useColor)
		DrawUnits(pUnitFrame, blankRate, pbrsMass);
	else
		DrawUnits(pUnitFrame, blankRate, pbmpUnit);
}

void Drawer::DrawRollingLines(GameFrame* pGameFrame)
{
	if (GameState::RollUp == pController->GetGameState() ||
		GameState::RollDown == pController->GetGameState())
	{
		for (int i = pGameFrame->sizeY - pGameFrame->rolledRows;
			i < pGameFrame->sizeY; i++)
		{
			if (pGameFrame->useColor)
				DrawLine(pGameFrame, i, pbrsMass);
			else
				DrawLine(pGameFrame, i, pbmpUnit);
		}
	}
}

void Drawer::DrawInfo(InfoFrame* pInfoFrame)
{
	if (!IsValid()) return;

	wstring labels;
	labels.append(L"Level\n\n");
	labels.append(L"Score\n\n");
	labels.append(L"StartLine\n\n");
	wstring infos;
	infos.append(to_wstring(pInfoFrame->GetLevel())).append(L"\n\n");
	infos.append(to_wstring(pInfoFrame->GetScore())).append(L"\n\n");
	infos.append(to_wstring(pInfoFrame->GetStartLine()));

	RectF rcInfo((REAL)pInfoFrame->left, (REAL)pInfoFrame->top,
		(REAL)pInfoFrame->GetWidth(), (REAL)pInfoFrame->GetHeight());
	Graphics grp(hdcCmp);

	StringFormat stringFormatLeft;
	stringFormatLeft.SetAlignment(StringAlignmentNear);
	grp.DrawString(labels.c_str(), (INT)labels.size(),
		pfntInfo, rcInfo, &stringFormatLeft, pbrsInfo);

	StringFormat stringFormatRight;
	stringFormatRight.SetAlignment(StringAlignmentFar);
	grp.DrawString(infos.c_str(), (INT)infos.size(),
		pfntInfo, rcInfo, &stringFormatRight, pbrsInfo);
}

void Drawer::DrawIcon(GameFrame* pGameFrame)
{
	if (!IsValid()) return;

	if (GameState::Pause != pController->GetGameState() && GameState::ResumeDelay != pController->GetGameState())
		return;

	Image* pIcon = GameState::Pause == pController->GetGameState() ? pbmpPause : pbmpResume;

	REAL rate = pIcon->GetWidth() * pIcon->GetHeight() /
		(pGameFrame->GetWidth() * pGameFrame->GetHeight() * (REAL)pGameFrame->iconScaleRatio);
	rate = sqrt(rate);

	REAL width = pIcon->GetWidth() / rate;
	REAL height = pIcon->GetHeight() / rate;
	// ensure icon not out of game frame
	if (width > pGameFrame->GetWidth() || height > pGameFrame->GetHeight())
	{
		if (width / pGameFrame->GetWidth() >= height / pGameFrame->GetHeight())
		{
			rate = pIcon->GetWidth() * 1.0f / pGameFrame->GetWidth();
		}
		else
		{
			rate = pIcon->GetHeight() * 1.0f / pGameFrame->GetHeight();
		}
		width = pIcon->GetWidth() / rate;
		height = pIcon->GetHeight() / rate;
	}

	REAL left = (pGameFrame->GetWidth() - width) / 2 + pGameFrame->GetLeft();
	REAL top = (pGameFrame->GetHeight() - height) / 2 + pGameFrame->GetTop();

	Graphics grp(hdcCmp);
	grp.DrawImage(pIcon, left, top, width, height);
}

void Drawer::DrawMask(GameFrame* pGameFrame)
{
	if (!IsValid()) return;

	if (GameState::Pause != pController->GetGameState() && GameState::ResumeDelay != pController->GetGameState())
		return;
	
	Graphics grp(hdcCmp);
	grp.FillRectangle(pbrsMask, pGameFrame->GetLeft(), pGameFrame->GetTop(),
		pGameFrame->GetWidth(), pGameFrame->GetHeight());
}

void Drawer::DrawSplash(GameFrame* pGameFrame)
{
	if (!IsValid()) return;

	if (pController->IsStarted())
		return;
	
	int x = pGameFrame->GetLeft() + pGameFrame->borderThickness;
	int y = pGameFrame->GetTop() + pGameFrame->borderThickness;
	int width = pGameFrame->GetWidth() - pGameFrame->borderThickness;
	int height = pGameFrame->GetHeight() - pGameFrame->borderThickness;
	Rect rect(x, y, width, height);
//	DrawBitmap(
//GameState::None == pController->GetGameState() ?
//		pbmpBegin : pbmpGameOver,
// RenderMode::Uniform,
//
//		RenderAlignment::HorizontalCenter | RenderAlignment::VerticalCenter,
//		rect);
}

void Drawer::GetDCSize(HDC hdc, LONG * pWidth, LONG * pHeight)
{
	BITMAP bm;
	memset(&bm, 0, sizeof(BITMAP));
	HGDIOBJ hbm = GetCurrentObject(hdc, OBJ_BITMAP);
	GetObject(hbm, sizeof(BITMAP), &bm);
	*pWidth = bm.bmWidth;
	*pHeight = bm.bmHeight;
}

void Drawer::GetDCResolution(HDC hdc, int* px, int* py)
{
	*px = GetDeviceCaps(hdc, LOGPIXELSX);
	*py = GetDeviceCaps(hdc, LOGPIXELSY);
}

void Drawer::SetBitmapDCResolution(Bitmap* pBitmap, HDC hdc)
{
	if (NULL == hdc)
		return;

	int resX, resY;
	GetDCResolution(hdc, &resX, &resY);
	pBitmap->SetResolution((REAL)resX, (REAL)resY);
}

Color Drawer::LightColor(Color& color, float ratio)
{
	BYTE a = color.GetA();
	BYTE r = (BYTE)round(color.GetR() * ratio + (0xFF) * (1 - ratio));
	BYTE g = (BYTE)round(color.GetG() * ratio + (0xFF) * (1 - ratio));
	BYTE b = (BYTE)round(color.GetB() * ratio + (0xFF) * (1 - ratio));
	return Color(a, r, g, b);
}

Bitmap* Drawer::StretchBitmap(HDC hdc, Bitmap* pBmp, int dstWidth, int dstHeight)
{
	Bitmap* pBmpDst = new Bitmap(dstWidth, dstHeight, pBmp->GetPixelFormat());
	Graphics grp(pBmpDst);
	grp.DrawImage(pBmp, 0, 0, dstWidth, dstHeight);
	return pBmpDst;
}

Bitmap* Drawer::LightBitmap(HDC hdc, Bitmap* pBmp, float ratio)
{
	int width = pBmp->GetWidth();
	int height = pBmp->GetHeight();
	Bitmap* pBmpDst = new Bitmap(width, height, pBmp->GetPixelFormat());
	Graphics grp(pBmpDst);
	Rect dstRect(0, 0, width, height);
	ImageAttributes imageAttributes;
	// r,g,b transform to (1 - ratio)*source + ratio*255
	ColorMatrix colorMatrix = {
		1.0f - ratio,0.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f - ratio,0.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f - ratio,0.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f,0.0f,
		ratio,ratio,ratio,0.0f,1.0f };
	imageAttributes.SetColorMatrix(&colorMatrix);
	grp.DrawImage(pBmp, dstRect, 0, 0, width, height, UnitPixel, &imageAttributes);
	return pBmpDst;
}

HBITMAP Drawer::StretchBitmap(HDC hdc, HBITMAP hbm, int dstWidth, int dstHeight)
{
	Bitmap bitmap(hbm, NULL);
	int srcWidth = bitmap.GetWidth();
	int srcHeight = bitmap.GetHeight();

	// create compatible dcs
	HDC hdcDst = CreateCompatibleDC(hdc);
	HBITMAP hbmDst = CreateCompatibleBitmap(hdc, dstWidth, dstHeight);
	HGDIOBJ hbmDstOrignal = SelectObject(hdcDst, hbmDst);

	HDC hdcSrc = CreateCompatibleDC(hdc);
	HBITMAP hbmSrc = CreateCompatibleBitmap(hdc, srcWidth, srcHeight);
	HGDIOBJ hbmSrcOrignal = SelectObject(hdcSrc, hbmSrc);

	// stretch bitmap
	Graphics grp(hdcSrc);
	grp.DrawImage(&bitmap, 0, 0, srcWidth, srcHeight);

	SetStretchBltMode(hdcDst, COLORONCOLOR);
	StretchBlt(hdcDst, 0, 0, dstWidth, dstHeight,
		hdcSrc, 0, 0, srcWidth, srcHeight,
		SRCCOPY);

	// delete compatible dcs
	SelectObject(hdcDst, hbmDstOrignal);
	DeleteDC(hdcDst);

	SelectObject(hdcSrc, hbmSrcOrignal);
	DeleteDC(hdcSrc);
	DeleteObject(hbmSrc);

	return hbmDst;
}

HBITMAP Drawer::LightBitmap(HDC hdc, HBITMAP hbm, float ratio)
{
	Bitmap bitmap(hbm, NULL);
	UINT width = bitmap.GetWidth();
	UINT height = bitmap.GetHeight();

	// create compatible dcs
	HDC hdcDst = CreateCompatibleDC(hdc);
	HBITMAP hbmDst = CreateCompatibleBitmap(hdc, width, height);
	HGDIOBJ hbmDstOrignal = SelectObject(hdcDst, hbmDst);

	HDC hdcSrc = CreateCompatibleDC(hdc);
	HBITMAP hbmSrc = CreateCompatibleBitmap(hdc, width, height);
	HGDIOBJ hbmSrcOrignal = SelectObject(hdcSrc, hbmSrc);

	// alpha blend bitmap
	FloodFill(hdcSrc, 0, 0, RGB(255, 255, 255));

	Graphics grp(hdcDst);
	grp.DrawImage(&bitmap, 0, 0, width, height);

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0;
	bf.SourceConstantAlpha = (BYTE)(0xFF * ratio);
	AlphaBlend(hdcDst, 0, 0, width, height,
		hdcSrc, 0, 0, width, height,
		bf);

	// delete compatible dcs
	SelectObject(hdcDst, hbmDstOrignal);
	DeleteDC(hdcDst);

	SelectObject(hdcSrc, hbmSrcOrignal);
	DeleteDC(hdcSrc);
	DeleteObject(hbmSrc);

	return hbmDst;
}

HBITMAP Drawer::TranslateBitmap(HDC hdc, HBITMAP hbm, int offsetX, int offsetY)
{
	Bitmap bitmap(hbm, NULL);
	UINT width = bitmap.GetWidth();
	UINT height = bitmap.GetHeight();
	offsetX %= width;
	offsetY %= height;
	if (offsetX < 0)
		offsetX += width;
	if (offsetY < 0)
		offsetY += height;

	// create compatible dcs
	HDC hdcDst = CreateCompatibleDC(hdc);
	HBITMAP hbmDst = CreateCompatibleBitmap(hdc, width, height);
	HGDIOBJ hbmDstOrignal = SelectObject(hdcDst, hbmDst);

	HDC hdcSrc = CreateCompatibleDC(hdc);
	HBITMAP hbmSrc = CreateCompatibleBitmap(hdc, width, height);
	HGDIOBJ hbmSrcOrignal = SelectObject(hdcSrc, hbmSrc);

	// translate bitmap
	Graphics grp(hdcSrc);
	grp.DrawImage(&bitmap, 0, 0, width, height);
	BitBlt(hdcDst, 0, 0, offsetX, offsetY,
		hdcSrc, width - offsetX, height - offsetY,
		SRCCOPY);
	BitBlt(hdcDst, offsetX, 0, width - offsetX, offsetY,
		hdcSrc, 0, height - offsetY,
		SRCCOPY);
	BitBlt(hdcDst, offsetX, offsetY, width - offsetX, height - offsetY,
		hdcSrc, 0, 0,
		SRCCOPY);
	BitBlt(hdcDst, 0, offsetY, offsetX, height - offsetY,
		hdcSrc, width - offsetX, 0,
		SRCCOPY);

	// delete compatible dcs
	SelectObject(hdcDst, hbmDstOrignal);
	DeleteDC(hdcDst);

	SelectObject(hdcSrc, hbmSrcOrignal);
	DeleteDC(hdcSrc);
	DeleteObject(hbmSrc);

	return hbmDst;
}

HBITMAP Drawer::CreateHBITMAP(Bitmap* pBitmap)
{
	Color color;
	HBITMAP hbm;
	pBitmap->GetHBITMAP(color, &hbm);
	return hbm;
}

Brush* Drawer::GetRandomTetrisBrush()
{
	return vecTetrisBrushes[Utility::Random(0, (int)vecTetrisBrushes.size() - 1)];
}

bool Drawer::IsValid()
{
	return initialized && hdc && hdcCmp;
}

void Drawer::BackgroundFrameChangedProcStatic(Bitmap* pBitmap, SHORT sLoopedCount, UINT uCurrentFrame)
{
	singleton.BackgroundFrameChangedProc(pBitmap, sLoopedCount, uCurrentFrame);
}

void Drawer::BackgroundFrameChangedProc(Bitmap* pBitmap, SHORT sLoopedCount, UINT uCurrentFrame)
{
	InvalidateRect(hWnd, NULL, FALSE);
}
