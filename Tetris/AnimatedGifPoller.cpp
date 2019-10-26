#include "AnimatedGifPoller.h"
#include <tchar.h>

AnimatedGifPoller::AnimatedGifPoller(Bitmap* pBitmap, FrameChangedProc pFrameChangedProc) :
	m_pBitmap(nullptr),
	m_uFrameCur(0),
	m_bLoopInfinate(false)
{
	m_pBitmap = pBitmap;
	if(!Initialize())
		throw std::exception("This image is not an animated gif.");
	m_pFrameChangedProc = pFrameChangedProc;
}

AnimatedGifPoller::~AnimatedGifPoller()
{
	DeInitialize();
}

bool AnimatedGifPoller::Initialize()
{
	if (!IsGifFormat())
	{
		return false;
	}

	if (!ParseFrameCount(&m_uFrameCount))
	{
		return false;
	}
	if (m_uFrameCount <= 1)
	{
		return false;
	}

	m_plFrameDelays = new LONG[m_uFrameCount];
	if (!ParseFrameDelays(m_plFrameDelays))
	{
		delete[] m_plFrameDelays;
		return false;
	}

	if(!ParseLoopCount(&m_sLoopCount))
		m_sLoopCount = 1;

	m_uFrameCur = 0;
	m_sLoopedCount = 0;

	m_bInitialized = true;
	return true;
}

bool AnimatedGifPoller::DeInitialize()
{
	if (m_bInitialized)
	{
		delete[] m_plFrameDelays;
		m_bInitialized = false;
		return true;
	}
	return false;
}

void AnimatedGifPoller::Start()
{
	m_pBitmap->SelectActiveFrame(&FrameDimensionTime, m_uFrameCur);

	UINT_PTR uIDEvent = SetTimer(NULL, 0, GetFrameDelay(m_uFrameCur) * 10, PollTimerProcStatic);
	mapTimers[uIDEvent] = this;
	
	m_pFrameChangedProc(m_pBitmap, m_sLoopedCount, m_uFrameCur);
}

void AnimatedGifPoller::Stop()
{
}

void AnimatedGifPoller::Pause()
{
}

void AnimatedGifPoller::Resume()
{
}

Bitmap* AnimatedGifPoller::GetBitmap()
{
	return m_pBitmap;
}

LONG AnimatedGifPoller::GetFrameDelay(UINT uFrame)
{
	return m_plFrameDelays[uFrame];
}

SHORT AnimatedGifPoller::GetLoopCount()
{
	return m_sLoopCount;
}

void AnimatedGifPoller::SetLoopInfinate(bool loopInfinate)
{
	m_bLoopInfinate = loopInfinate;
}

bool AnimatedGifPoller::IsAnimatedGif(Bitmap* pBitmap)
{
	try
	{
		AnimatedGifPoller animatedGifPlayer(pBitmap, nullptr);
	}
	catch (const std::exception&)
	{
		return false;
	}
	return true;
}

bool AnimatedGifPoller::IsGifFormat()
{
	GUID rawFormat;
	m_pBitmap->GetRawFormat(&rawFormat);
	return 0 == memcmp(&ImageFormatGIF, &rawFormat, sizeof(GUID));
}

bool AnimatedGifPoller::ParseFrameCount(UINT* puFrameCount)
{
	*puFrameCount = m_pBitmap->GetFrameCount(&FrameDimensionTime);
	return true;
}

bool AnimatedGifPoller::ParseFrameDelays(LONG* const plFrameDelays)
{
	PropertyItem* pPropertyItem;
	if (!(pPropertyItem = CreatePropertyItem(PropertyTagFrameDelay)))
	{
		return false;
	}
	memcpy(plFrameDelays, pPropertyItem->value, pPropertyItem->length);
	DeletePropertyItem(pPropertyItem);
	return true;
}

bool AnimatedGifPoller::ParseLoopCount(SHORT* psLoopCount)
{
	PropertyItem* pPropertyItem;
	if (!(pPropertyItem = CreatePropertyItem(PropertyTagLoopCount)))
		return false;
	*psLoopCount = *((SHORT*)pPropertyItem->value);
	DeletePropertyItem(pPropertyItem);
	return true;
}

PropertyItem* AnimatedGifPoller::CreatePropertyItem(PROPID propId)
{
	UINT uSize = m_pBitmap->GetPropertyItemSize(propId);
	PropertyItem* pPropertyItem = (PropertyItem*)new char[uSize];
	Status status = m_pBitmap->GetPropertyItem(propId, uSize, pPropertyItem);
	if (Status::Ok != status)
	{
		delete[] pPropertyItem;
		return nullptr;
	}
	return pPropertyItem;
}

void AnimatedGifPoller::DeletePropertyItem(PropertyItem* pPropertyItem)
{
	delete[] pPropertyItem;
}

void AnimatedGifPoller::PollTimerProcStatic(HWND hWnd, UINT msg, UINT_PTR uIDEvent, DWORD millisecond)
{
	mapTimers[uIDEvent]->PollTimerProc(hWnd, msg, uIDEvent, millisecond);
}

void AnimatedGifPoller::PollTimerProc(HWND hWnd, UINT msg, UINT_PTR uIDEvent, DWORD millisecond)
{
	// kill timer
	KillTimer(NULL, uIDEvent);
	mapTimers.erase(uIDEvent);

	// increase loop count, stop if over
	if (m_uFrameCur == m_uFrameCount - 1)
		m_sLoopedCount++;
	if (!m_bLoopInfinate && 0 != m_sLoopCount && m_sLoopCount + 1 == m_sLoopedCount)
		return;

	// set timer for next frame
	uIDEvent = SetTimer(NULL, 0, GetFrameDelay(m_uFrameCur) * 10, PollTimerProcStatic);
	mapTimers[uIDEvent] = this;

	// set next frame as active
	m_uFrameCur = (m_uFrameCur + 1) % m_uFrameCount;
	m_pBitmap->SelectActiveFrame(&FrameDimensionTime, m_uFrameCur);

	m_pFrameChangedProc(m_pBitmap, m_sLoopedCount, m_uFrameCur);
}

map<UINT_PTR, AnimatedGifPoller*> AnimatedGifPoller::mapTimers;
