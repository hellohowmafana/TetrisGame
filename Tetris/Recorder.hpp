#pragma once

#include <string>
#include "Configuration.hpp"
#include "Controller.hpp"
using namespace std;

class RecordParameters : public IBinarySerializable
{
	Configuration* pConfiguration;

	virtual bool Save(char* pData, size_t& size);
	virtual bool Load(char* pData);

	void Set(Configuration* pConfiguration);
};

enum class RecordState : unsigned char
{
	Start = 1, Halfway = 2, Remove = 3, End = 4
};

enum class RecordAction : unsigned char
{
	StepDown = 16, StepLeft = 17, StepRight = 18, Rotate = 19, Drop = 20
};

class Recorder
{
private:
	unsigned short version;
	char head[7] = "tetris";
	char tail[2] = "\\";

	bool SaveParameters(Configuration* pConfiguration, char* pData);

public:
	bool Save(wstring file, Configuration* pConfiguration, Controller* pController);
	bool Load(wstring file, Configuration* pConfiguration, Controller* pController);
};

