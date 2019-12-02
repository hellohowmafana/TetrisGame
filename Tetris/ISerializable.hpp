#pragma once
#include <string>
#include <fstream>
#include "Utility.hpp"
#include "Archive.hpp"

class ISerializable
{
public:
	virtual bool Save(const wstring label, wstring& value) = 0;
	virtual bool Load(const wstring label, wstring value) = 0;
};