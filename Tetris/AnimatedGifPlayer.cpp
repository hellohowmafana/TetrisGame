#include "AnimatedGifPlayer.h"
#include <tchar.h>

AnimatedGifPlayer::AnimatedGifPlayer() :
	m_pBitmap(nullptr),
	m_uFrameCur(0),
	m_bLoopInfinate(false)
{
}

AnimatedGifPlayer::AnimatedGifPlayer(const TCHAR* szFilePath) :
	m_pBitmap(nullptr),
	m_uFrameCur(0),
	m_bLoopInfinate(false)
{
	Initialize(szFilePath);
}

AnimatedGifPlayer::~AnimatedGifPlayer()
{
	DeInitialize();
}

bool AnimatedGifPlayer::Initialize(const TCHAR* szFilePath)
{
	_tcscpy(m_szFilePath, szFilePath);
	m_pBitmap = new Bitmap(szFilePath);
	m_uFrameCur = 0;

	// detect raw format
	GUID rawFormat;
	m_pBitmap->GetRawFormat(&rawFormat);
	if(0 != memcmp(&ImageFormatGIF, &rawFormat, sizeof(GUID)))
		return false;

	// get frame count
	m_uFrameCount = m_pBitmap->GetFrameCount(&FrameDimensionTime);
	if (m_uFrameCount == 0)
		return false;

	// get frame delay property
	if (!(m_pPropFrameDelay = CreatePropertyItem(PropertyTagFrameDelay)))
		return false;

	// get loop count
	PropertyItem* pPropLoopCount;
	if (!(pPropLoopCount = CreatePropertyItem(PropertyTagLoopCount)))
		return false;
	m_sLoopCount = *((SHORT*)pPropLoopCount->value);

	return true;
}

bool AnimatedGifPlayer::DeInitialize()
{
	delete m_pBitmap;
	m_pBitmap = nullptr;
	DeletePropertyItem(m_pPropFrameDelay);
	m_pPropFrameDelay = nullptr;
	return true;
}

void AnimatedGifPlayer::Play()
{
	// set first frame as active
	m_pBitmap->SelectActiveFrame(&FrameDimensionTime, m_uFrameCur);

	// set timer for next frame
	UINT_PTR uIDEvent = SetTimer(NULL, 0, GetFrameDelay(m_uFrameCur) * 10, TimerProcStatic);
	mapTimers[uIDEvent] = this;
	
	InvalidateRgn(m_hWndRedraw, m_hRgnRedraw, FALSE);
}

void AnimatedGifPlayer::SetRedrawRegion(HWND hWnd, HRGN hRgn)
{
	this->m_hWndRedraw = hWnd;
	this->m_hRgnRedraw = hRgn;
}

Image* AnimatedGifPlayer::GetImage()
{
	return m_pBitmap;
}

LONG AnimatedGifPlayer::GetFrameDelay(UINT uFrame)
{
	return ((LONG*)m_pPropFrameDelay->value)[uFrame];
}

SHORT AnimatedGifPlayer::GetLoopCount()
{
	return m_sLoopCount;
}

void AnimatedGifPlayer::SetLoopInfinate(bool loopInfinate)
{
	m_bLoopInfinate = loopInfinate;
}

PropertyItem* AnimatedGifPlayer::CreatePropertyItem(PROPID propId)
{
	UINT uSize = m_pBitmap->GetPropertyItemSize(propId);
	PropertyItem* pPropertyItem = (PropertyItem*)new char[uSize];
	Status status = m_pBitmap->GetPropertyItem(propId, uSize, pPropertyItem);
	if (Status::Ok != status)
		return nullptr;
	return pPropertyItem;
}

void AnimatedGifPlayer::DeletePropertyItem(PropertyItem* pPropertyItem)
{
	delete[] pPropertyItem;
}

void AnimatedGifPlayer::TimerProcStatic(HWND hWnd, UINT msg, UINT_PTR uIDEvent, DWORD millisecond)
{
	mapTimers[uIDEvent]->TimerProc(hWnd, msg, uIDEvent, millisecond);
}

void AnimatedGifPlayer::TimerProc(HWND hWnd, UINT msg, UINT_PTR uIDEvent, DWORD millisecond)
{
	// kill timer
	KillTimer(NULL, uIDEvent);
	mapTimers.erase(uIDEvent);

	// increase loop count, stop if over
	if (m_uFrameCur == m_uFrameCount - 1)
		m_sLoopedCount++;
	if (!m_bLoopInfinate && 0 != m_sLoopCount && m_sLoopCount == m_sLoopedCount)
		return;

	// set timer for next frame
	uIDEvent = SetTimer(NULL, 0, GetFrameDelay(m_uFrameCur) * 10, TimerProcStatic);
	mapTimers[uIDEvent] = this;

	// set next frame as active
	m_uFrameCur = (m_uFrameCur + 1) % m_uFrameCount;
	m_pBitmap->SelectActiveFrame(&FrameDimensionTime, m_uFrameCur);

	InvalidateRgn(m_hWndRedraw, m_hRgnRedraw, FALSE);
}

map<UINT_PTR, AnimatedGifPlayer*> AnimatedGifPlayer::mapTimers;
