#pragma once
#include <string>
#include <fstream>
#include "Utility.h"
#include "Archive.h"

class ISerializable
{
public:
	virtual bool Save(const wstring label, wstring& value) = 0;
	virtual bool Load(const wstring label, wstring value) = 0;
};