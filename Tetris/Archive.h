#pragma once
#include <tchar.h>
#include <string>
#include "tcharstd.h"
using namespace std;

class Controller;

class Archive
{
public:
	static bool Save(TCHAR* szArchive, Controller* pController);
	static bool Load(TCHAR* szArchive, Controller* pController);

public:
	static const tstring labelFrame;
	static const tstring labelNext;
	static const tstring labelCurrent;
	static const tstring labelMass;
	static const tstring labelScore;
	static const tstring labelMark;
};

