#include "Configuration.h"
#include <Windows.h>
#include <tchar.h>
#include <fstream> 
#include "TetrisShape.h"
using namespace std;

#define PATH_LENGTH 512
#define BUFFER_CHARS 32
#define ClaimStringBuffer TCHAR buffer[BUFFER_CHARS] = _T("");
#define GetStringBuffer (buffer)
#define GetConfigurationString(keySection, keyKey) GetPrivateProfileString((keySection).c_str(),\
													 (keyKey).c_str(), _T(""),\
													 buffer, BUFFER_CHARS, pathConfiguration.c_str())
#define GetConfigurationStringEx(keySection, keyKey, buffer) GetPrivateProfileString((keySection).c_str(),\
															  (keyKey).c_str(), _T(""),\
															  (buffer), BUFFER_CHARS, pathConfiguration.c_str())
#define GetConfigurationStr(keySection, keyKey, str) GetPrivateProfileString((keySection).c_str(),\
														(keyKey).c_str(), _T(""),\
														buffer, BUFFER_CHARS, pathConfiguration.c_str()),\
														(str) = GetStringBuffer
#define GetConfigurationInt(keySection, keyKey) GetPrivateProfileInt((keySection).c_str(),\
													 (keyKey).c_str(), 0, pathConfiguration.c_str())
#define GetConfigurationBool(keySection, keyKey) (GetConfigurationInt(keySection, keyKey)==1)
#define GetConfigurationDouble(keySection, keyKey) (GetConfigurationString(keySection, keyKey),\
													stold(GetStringBuffer))

Configuration Configuration::singleton;

bool Configuration::Initialize()
{
	try
	{
		if(!InitializeIniPaths())
			return false;
		if(!LoadParameters())
			return false;
		if(!LoadShapes())
			return false;
		if(!LoadColors())
			return false;
	}
	catch(exception e)
	{
		return false;
	}
	return true;
}

bool Configuration::InitializeIniPaths()
{
	TCHAR path[PATH_LENGTH];
	GetModuleFileName(nullptr, path, PATH_LENGTH);
	*(_tcsrchr(path, _T('\\')) + 1) = _T('\0');

	pathInis = path + INIS_PATH;
	pathBitmaps = path + BITMAPS_PATH;
	pathShapes = path + SHAPES_PATH;

	pathConfiguration = path + CONFIGURATION_PATH;
	pathColorFile = path + COLOR_FILE_PATH;
	pathDroppedColorFile = path + DROPPED_COLOR_FILE_PATH;
	pathFrameColorFile = path + FRAME_COLOR_FILE_PATH;
	pathSeparatorColorFile = path + BORDER_COLOR_FILE_PATH;
	pathClassicShapes = path + CLASSIC_SHAPES_PATH;

	return true;
}

bool Configuration::LoadParameters()
{
	ClaimStringBuffer;
	
	// window
	GetConfigurationString(keyWindow, keyWindowSize);
	SplitStringToInts(GetStringBuffer, _T('x'), &windowWidth, &windowHeight);
	GetConfigurationString(keyWindow, keyWindowPostion);
	SplitStringToInts(GetStringBuffer, _T(','), &windowLeft, &windowTop);
	windowCenter = GetConfigurationBool(keyWindow, keyWindowCenter);

	// setting
	// draw
	GetConfigurationString(keySetting, keyFramePostion);
	SplitStringToInts(GetStringBuffer, _T(','), &frameLeft, &frameTop);
	GetConfigurationString(keySetting, keyFrameSize);
	SplitStringToInts(GetStringBuffer, _T(','), &frameSizeX, &frameSizeY);
	GetConfigurationString(keySetting, keyPromptFramePostion);
	SplitStringToInts(GetStringBuffer, _T(','), &promptFrameLeft, &promptFrameTop);
	GetConfigurationString(keySetting, keyPromptFrameSize);
	SplitStringToInts(GetStringBuffer, _T(','), &promptFrameSizeX, &promptFrameSizeY);
	GetConfigurationString(keySetting, keyInfoFramePosition);
	SplitStringToInts(GetStringBuffer, _T(','), &infoFrameLeft, &infoFrameTop);
	GetConfigurationString(keySetting, keyInfoFrameSize);
	SplitStringToInts(GetStringBuffer, _T(','), &infoFrameSizeX, &infoFrameSizeY);
	borderThickness = GetConfigurationInt(keySetting, keyBorderThickness);
	unitWidth = GetConfigurationInt(keySetting, keyUnitWidth);
	separatorThickness = GetConfigurationInt(keySetting, keySeparatorThickness);
	// timespan
	dropTimespan = GetConfigurationInt(keySetting, keyDropTimespan);
	dropDelay = GetConfigurationInt(keySetting, keyDropDelay);
	droppedDelay = GetConfigurationInt(keySetting, keyDroppedDelay);
	removeDelay = GetConfigurationInt(keySetting, keyRemoveDelay);
	removeBlinkTimespan = GetConfigurationInt(keySetting, keyRemoveBlinkTimespan);

	// game
	level = GetConfigurationInt(keyGame, keyLevel);
	startLine = GetConfigurationInt(keyGame, keyStartLine);
	startLineBlankRate = GetConfigurationDouble(keyGame, keyStartLineBlankRate);
	GetConfigurationString(keyGame, keyRemoveScores);
	SplitStringToInts(GetStringBuffer, _T(','), &removeScores[0], &removeScores[1], &removeScores[2], &removeScores[3]);
	droppedScore = GetConfigurationInt(keyGame, keyDroppedScore);

	// music
	GetConfigurationStr(keyMusic, keyMusicRotate, musicRotate);
	GetConfigurationStr(keyMusic, keyMusicDrop, musicDrop);
	GetConfigurationStr(keyMusic, keyMusicDropped, musicDropped);
	GetConfigurationStr(keyMusic, keyMusicBgm, musicBgm);

	// bitmap
	useColor = GetConfigurationBool(keyBitmap, keyUseColor);
	useColorRandom = GetConfigurationBool(keyBitmap, keyUseColorRandom);
	GetConfigurationStr(keyBitmap, keyUnitBitmap, unitBitmap);
	useDroppedColor = GetConfigurationBool(keyBitmap, keyUseDroppedColor);

	return true;
}

bool Configuration::LoadShapes()
{
	wfstream fs;
	try {
		fs.open(pathClassicShapes, wfstream::in);

		ClaimStringBuffer;
		
		TCHAR group[BUFFER_CHARS] = _T("");
		_tsplitpath(pathClassicShapes.c_str(), NULL, NULL, group, NULL);
		TCHAR name[BUFFER_CHARS] = _T("");
		bool penetrable = false;
		int row = 0, col = 0;
		int color = 0;
		vector<char> vecData;
			
		while (true)
		{

			fs.getline(GetStringBuffer, BUFFER_CHARS);
			if (_tcschr(GetStringBuffer, _T(':'))) // shape declare
			{
				if (_T("") != name && row != 0 && col != 0)
				{
					TetrisType::Create(group, name, penetrable, row, col, vecData.data(), vecData.size(), color);
					color++;
				}
				SplitStringToStrInt(GetStringBuffer, _T(':'), name, penetrable);
				row = col = 0;
				vecData.clear();
			}
			else if (fs.rdstate() & wfstream::eofbit) // end of file
				break;
			else if (*GetStringBuffer == _T('\0')) // blank line
				continue;
			else // shape data
			{
				col = (int)_tcslen(GetStringBuffer);
				row++;
				for (int i = 0; i < col; i++)
					vecData.push_back((char)GetStringBuffer[i]);
			}
		}
		fs.close();
	}
	catch(...)
	{
	}

	return true;
}

bool Configuration::LoadColors()
{
	return false;
}

bool Configuration::SaveWindowPostion(int w, int h, int l, int t, bool c)
{
	return false;
}

bool Configuration::SplitStringToInts(TCHAR* szStr, TCHAR ch, int* v1, int* v2)
{
	try {
		wstring strBuffer(szStr);
		size_t pos = strBuffer.find(ch);
		if (string::npos == pos)
		return false;
		wstring strVal = strBuffer.substr(0, pos);
		*v1 = stoi(strVal);
		strVal = strBuffer.substr(pos + 1);
		*v2 = stoi(strVal);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Configuration::SplitStringToInts(TCHAR* szStr, TCHAR ch, int* v1, int* v2, int* v3, int* v4)
{
	try {
		wstring strBuffer(szStr);
		size_t pos;
		wstring strVal;

		if(string::npos == (pos = strBuffer.find(ch)))
			return false;
		strVal = strBuffer.substr(0, pos);
		*v1 = stoi(strVal);
		strBuffer.erase(0, pos + 1);

		if (string::npos == (pos = strBuffer.find(ch)))
			return false;
		strVal = strBuffer.substr(0, pos);
		*v2 = stoi(strVal);
		strBuffer.erase(0, pos + 1);

		if (string::npos == (pos = strBuffer.find(ch)))
			return false;
		strVal = strBuffer.substr(0, pos);
		*v3 = stoi(strVal);
		strBuffer.erase(0, pos + 1);

		if (string::npos == (pos = strBuffer.find(ch)))
			return false;
		strVal = strBuffer.substr(0, pos);
		*v4 = stoi(strVal);

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Configuration::SplitStringToStrInt(TCHAR* szStr, TCHAR ch, TCHAR* szv, int iv)
{
	try {
		wstring strBuffer(szStr);
		size_t pos = strBuffer.find(ch);
		if (string::npos == pos)
			return false;
		wstring strVal = strBuffer.substr(0, pos);
		copy(strVal.begin(), strVal.end(), szv);
		strVal = strBuffer.substr(pos + 1);
		iv = stoi(strVal);
		return true;
	}
	catch (...)
	{
		return false;
	}
}
