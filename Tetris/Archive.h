#pragma once
#include <Windows.h>
#include <string>
using namespace std;

class Controller;

class Archive
{
public:
	static bool Save(wchar_t* szArchive, Controller* pController);
	static bool Load(wchar_t* szArchive, Controller* pController);

public:
	static const wstring labelFrame;
	static const wstring labelNext;
	static const wstring labelCurrent;
	static const wstring labelMass;
	static const wstring labelScore;
	static const wstring labelMark;
};

