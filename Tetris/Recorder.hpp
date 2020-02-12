#pragma once

#include <string>
#include "Configuration.hpp"
using namespace std;

class Controller;

enum class TrackState : unsigned char
{
	// status
	Start = 1,
	Halfway = 2,
	Remove = 3,
	End = 4,

	// action
	StepDown = 16,
	StepDownSequence = 17,
	
	StepLeft = 18,
	StepLeftSequence = 19,
	
	StepRight = 20,
	StepRightSequence = 21,
	
	Rotate = 22,
	RotateSequence = 23,
	
	Drop = 24
};

class Recorder
{
private:
	ios* pstream;
	bool output;

	unsigned short version;
	char head[7] = "tetris";
	char tail[2] = "\\";

	streampos posTrackSize;
	TrackState lastTrackedState;
	int times;

	ofstream& getofstream();
	ifstream& getifstream();

public:
	static Recorder singleton;

	bool StartRecord(wstring file, Configuration* pConfiguration, Controller* pController);
	bool EndRecord();

	bool Load(wstring file, Configuration* pConfiguration, Controller* pController);
	bool Unload();

	bool Track(TrackState state);
	bool EndTrack();
};

