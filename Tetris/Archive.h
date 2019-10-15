#pragma once
#include <tchar.h>
#include <string>
#include "tcharstd.h"
using namespace std;

class Controller;

class Archive
{
public:
	bool Save(TCHAR* szFile, Controller* pController);
	bool Load(TCHAR* szFile, Controller* pController);

public:
	static const tstring labelFrame;
	static const tstring labelNext;
	static const tstring labelCurrent;
	static const tstring labelMass;
	static const tstring labelScore;
};

