#include "Music.h"

Music::Music()
{
}

Music::Music(MusicType musicType, wstring musicPath, wstring alias)
{
	SetMusic(musicType, alias, musicPath);
}

Music::~Music()
{
	Close();
}

void Music::SetMusic(MusicType musicType, wstring alias, wstring musicPath)
{
	if (MusicStatus::Close != GetStatus())
		return;

	this->musicType = musicType;
	if(!alias.empty())
		this->alias = alias;
	if(!musicPath.empty())
		this->musicPath = musicPath;
}

void Music::SetNotifyWindow(HWND hWnd)
{
	this->hWndMsg = hWnd;
}

bool Music::Open()
{
	if (MusicStatus::Close != GetStatus())
		return false;

	wstring mciString;
	mciString.append(L"open \"").append(musicPath).append(L"\" alias \"")
		.append(alias).append(L"\"");
	if (0 == mciSendString(mciString.c_str(), NULL, 0, NULL))
	{
		deviceId = mciGetDeviceID(alias.c_str());
		return true;
	}
	else
	{
		return false;
	}
}

bool Music::Close()
{
	if (MusicStatus::Close == GetStatus())
		return false;
	wstring mciString;
	mciString.append(L"close \"").append(alias).append(L"\"");
	if (0 == mciSendString(mciString.c_str(), NULL, 0, NULL))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Music::Play(bool notify)
{
	if (MusicStatus::Play != GetStatus() &&
		MusicStatus::Stop != GetStatus())
		return false;
	wstring mciString;
	mciString.append(L"play \"").append(alias).append(L"\" from 0");
	if(notify && hWndMsg)
		mciString.append(L" notify");
	if (0 == mciSendString(mciString.c_str(), NULL, 0, hWndMsg))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Music::Stop()
{
	if (MusicStatus::Play != GetStatus() &&
		MusicStatus::Pause != GetStatus())
		return false;
	wstring mciString;
	mciString.append(L"stop \"").append(alias).append(L"\"");
	if (0 == mciSendString(mciString.c_str(), NULL, 0, NULL))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Music::Pause()
{
	if (MusicStatus::Play != GetStatus())
		return false;
	wstring mciString;
	mciString.append(L"pause \"").append(alias).append(L"\"");
	if (0 == mciSendString(mciString.c_str(), NULL, 0, NULL))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Music::Resume()
{
	if (MusicStatus::Pause != GetStatus())
		return false;
	wstring mciString;
	mciString.append(L"resume \"").append(alias).append(L"\"");
	if (0 == mciSendString(mciString.c_str(), NULL, 0, NULL))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Music::Shift(wstring musicPath, bool forceClose, bool open)
{
	if (forceClose)
	{
		if (MusicStatus::Close != GetStatus())
			if (!Close())
				return false;
	}
	else
	{
		if (MusicStatus::Close != GetStatus())
			return false;
	}
	this->musicPath = musicPath;
	if (open)
		if(!Open())
			return false;
	return true;
}

MusicType Music::GetType()
{
	return musicType;
}

wstring Music::GetAlias()
{
	return alias;
}

MusicStatus Music::GetStatus()
{
	wstring mciString;
	mciString.append(L"status \"").append(alias).append(L"\" mode");
	wchar_t szStatus[8];
	if (0 == mciSendString(mciString.c_str(), szStatus, 8, NULL))
	{
		const wchar_t* pszStatusPlay = L"playing";
		const wchar_t* pszStatusPause = L"paused";
		const wchar_t* pszStatusStop = L"stopped";

		if(wcscmp(pszStatusPlay, szStatus) == 0)
		{
			return MusicStatus::Play;
		}
		else if (wcscmp(pszStatusPause, szStatus) == 0)
		{
			return MusicStatus::Pause;
		}
		else if (wcscmp(pszStatusStop, szStatus) == 0)
		{
			return MusicStatus::Stop;
		}
	}
	return MusicStatus::Close;
}

MCIDEVICEID Music::GetDeviceId()
{
	return deviceId;
}

bool Music::IsSound()
{
	return MusicType::Bgm != musicType;
}

bool Music::IsBgm()
{
	return MusicType::Bgm == musicType;
}
