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
	virtual bool Save(TCHAR* szString) = 0;
	virtual bool Load(TCHAR* szString) = 0;
};