#pragma once
#include <string>
#include <fstream>
#include "Utility.h"
#include "Archive.h"

class ISerializable
{
public:
	virtual bool Save(const wchar_t* szSection, wchar_t** pszString) = 0;
	virtual bool Load(const wchar_t* szSection, wchar_t* szString) = 0;
};