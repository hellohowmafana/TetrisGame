#pragma once
#include <tchar.h>
#include "tcharstd.h"
#include <string>
#include <fstream>
#include "Utility.h"
#include "Archive.h"

class ISerializable
{
public:
	virtual bool Save(const TCHAR* szSection, TCHAR** pszString) = 0;
	virtual bool Load(const TCHAR* szSection, TCHAR* szString) = 0;
};