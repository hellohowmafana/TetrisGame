#pragma once
#include <Windows.h>
#include <string>
using namespace std;

class Controller;

class Archive
{
public:
	static bool Exist(wstring archive);
	static bool Save(wstring archive, Controller* pController);
	static bool Load(wstring archive, Controller* pController);

public:
	static const wstring labelFrame;
	static const wstring labelNext;
	static const wstring labelCurrent;
	static const wstring labelMass;
	static const wstring labelScore;
	static const wstring labelStartLevel;
	static const wstring labelStartLine;
	static const wstring labelMark;
};

