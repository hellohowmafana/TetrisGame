#include "Configuration.h"
#include <fstream> 
#include "TetrisShape.h"
#include "Level.h"
#include <gdiplus.h>
#include "Background.h"
#include "Musician.h"
using namespace Gdiplus;

#define BUFFER_CHARS 256
#define ClaimStringBuffer wchar_t buffer[BUFFER_CHARS] = L"";
#define GetStringBuffer (buffer)
#define GetConfigurationString(keySection, keyKey) GetPrivateProfileString((keySection).c_str(),\
													 (keyKey).c_str(), L"",\
													 buffer, BUFFER_CHARS, pathConfiguration.c_str())
#define GetConfigurationStringEx(keySection, keyKey, buffer) GetPrivateProfileString((keySection).c_str(),\
															  (keyKey).c_str(), L"",\
															  (buffer), BUFFER_CHARS, pathConfiguration.c_str())
#define GetConfigurationStr(keySection, keyKey, str) GetPrivateProfileString((keySection).c_str(),\
														(keyKey).c_str(), L"",\
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
		if (!LoadLevels())
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
	wchar_t path[MAX_PATH];
	GetModuleFileName(nullptr, path, MAX_PATH);
	*(wcsrchr(path, L'\\') + 1) = L'\0';

	pathInis = path + INIS_PATH;
	pathBitmaps = path + BITMAPS_PATH;
	pathIcons = path + ICONS_PATH;
	pathShapes = path + SHAPES_PATH;
	pathSound = path + SOUND_PATH;
	pathBgm = path + BGM_PATH;
	pathArchives = path + ARCHIVES_PATH;

	pathConfiguration = path + CONFIGURATION_PATH;
	pathUnitBitmapFile = path + UNIT_BITMAP_FILE_PATH;
	pathTetrisColorFile = path + TETRIS_COLOR_FILE_PATH;
	pathMassColorFile = path + MASS_COLOR_FILE_PATH;
	pathBorderColorFile = path + BORDER_COLOR_FILE_PATH;
	pathSeparatorColorFile = path + SEPARATOR_COLOR_FILE_PATH;
	pathInformationColor = path + INFORMATION_COLOR_FILE_PATH;
	pathBackgroundColor = path + BACKGROUND_COLOR_FILE_PATH;
	pathBackground = path + BACKGROUND_FILE_PATH;
	FindFile(pathBackground);
	pathGameOver = path + GAMEOVER_FILE_PATH;
	FindFile(pathGameOver);

	pathPauseIcon = path + PAUSE_ICON_FILE_PATH;
	FindFile(pathPauseIcon);
	pathResumeIcon = path + RESUME_ICON_FILE_PATH;
	FindFile(pathResumeIcon);

	pathClassicShapes = path + CLASSIC_SHAPES_PATH;

	pathStepDownSound = path + STEPDOWN_SOUND_FILE_PATH;
	pathStepHorizontalSound = path + STEPHORIZONTAL_SOUND_FILE_PATH;
	pathRotateSound = path + ROTATE_SOUND_FILE_PATH;
	pathDroppedSound = path + DROPPED_SOUND_FILE_PATH;
	pathRemoveSound = path + REMOVE_SOUND_FILE_PATH;
	pathGameOverSound = path + GAMEOVER_SOUND_FILE_PATH;

	FindFiles(pathBgm + L"\\*", &vecBgms);

	return true;
}

bool Configuration::LoadParameters()
{
	ClaimStringBuffer;
	
	// window
	GetConfigurationString(keyWindow, keyWindowSize);
	SplitStringToInts(GetStringBuffer, L'x', &windowWidth, &windowHeight);
	GetConfigurationString(keyWindow, keyWindowPostion);
	SplitStringToInts(GetStringBuffer, L',', &windowLeft, &windowTop);
	windowCenter = GetConfigurationBool(keyWindow, keyWindowCenter);

	// display
	GetConfigurationString(keyDisplay, keyFramePostion);
	SplitStringToInts(GetStringBuffer, L',', &frameLeft, &frameTop);
	GetConfigurationString(keyDisplay, keyFrameSize);
	SplitStringToInts(GetStringBuffer, L',', &frameSizeX, &frameSizeY);
	GetConfigurationString(keyDisplay, keyPromptFramePostion);
	SplitStringToInts(GetStringBuffer, L',', &promptFrameLeft, &promptFrameTop);
	GetConfigurationString(keyDisplay, keyPromptFrameSize);
	SplitStringToInts(GetStringBuffer, L',', &promptFrameSizeX, &promptFrameSizeY);
	GetConfigurationString(keyDisplay, keyInfoFramePosition);
	SplitStringToInts(GetStringBuffer, L',', &infoFrameLeft, &infoFrameTop);
	GetConfigurationString(keyDisplay, keyInfoFrameSize);
	SplitStringToInts(GetStringBuffer, L',', &infoFrameSizeX, &infoFrameSizeY);
	borderThickness = GetConfigurationInt(keyDisplay, keyBorderThickness);
	separatorThickness = GetConfigurationInt(keyDisplay, keySeparatorThickness);
	unitWidth = GetConfigurationInt(keyDisplay, keyUnitWidth);
	GetConfigurationStr(keyDisplay, keyInfoFontFace, infoFontFace);
	infoFontHeight = GetConfigurationInt(keyDisplay, keyInfoFontHeight);
	infoFontWidth = GetConfigurationInt(keyDisplay, keyInfoFontWidth);
	infoFontWeight = GetConfigurationInt(keyDisplay, keyInfoFontWeight);
	iconScaleRatio = GetConfigurationDouble(keyDisplay, keyIconScaleRatio);
	maskTransparency = GetConfigurationDouble(keyDisplay, keyMaskTransparency);

	// game
	startLevel = GetConfigurationInt(keyGame, keyStartLevel);
	startLine = GetConfigurationInt(keyGame, keyStartLine);
	startLineBlankRate = GetConfigurationDouble(keyGame, keyStartLineBlankRate);
	GetConfigurationString(keyGame, keyRemoveScores);
	SplitStringToInts(GetStringBuffer, L',', vecRemoveScores);
	droppedScore = GetConfigurationInt(keyGame, keyDroppedScore);
	maxLevel = GetConfigurationInt(keyGame, keyMaxLevel);
	GetConfigurationString(keyGame, keyScoreGainRate);
	SplitStringToDoubles(GetStringBuffer, L',', vecScoreGainRate);
	GetConfigurationString(keyGame, keyLevelScore);
	SplitStringToInts(GetStringBuffer, L',', vecLevelScore);
	GetConfigurationString(keyGame, keyStepDownTimespan);
	SplitStringToInts(GetStringBuffer, L',', vecStepDownTimespan);
	dropTimespan = GetConfigurationInt(keyGame, keyDropTimespan);
	dropImmediate = GetConfigurationBool(keyGame, keyDropImmediate);
	removeBlinkTimespan = GetConfigurationInt(keyGame, keyRemoveBlinkTimespan);
	removeBlinkCount = GetConfigurationInt(keyGame, keyRemoveBlinkCount);
	rollTimespan = GetConfigurationInt(keyGame, keyRollTimespan);
	resumeDelayTimespan = GetConfigurationInt(keyGame, keyResumeDelayTimespan);

	// music
	soundOn = GetConfigurationBool(keyMusic, keySoundOn);
	bgmOn = GetConfigurationBool(keyMusic, keyBgmOn);
	randomBgm = GetConfigurationBool(keyMusic, keyRandomBgm);

	// bitmap
	useColor = GetConfigurationBool(keyBitmap, keyUseColor);
	useColorRandom = GetConfigurationBool(keyBitmap, keyUseColorRandom);
	GetConfigurationStr(keyBitmap, keyUnitBitmap, unitBitmap);
	useMassColor = GetConfigurationBool(keyBitmap, keyUseMassColor);
	backgroundMode = (BackgroundMode)GetConfigurationInt(keyBitmap, keyBackgroundMode);

	return true;
}

bool Configuration::LoadLevels()
{
	vector<Level>* pvecLevels = &Level::vecLevels;
	pvecLevels->resize(maxLevel);
	for (int i = 0; i < maxLevel; i++)
	{
		pvecLevels->at(i).level = i + 1;
		pvecLevels->at(i).minScore =
			i < vecLevelScore.size() ? vecLevelScore[i] : -1;
		pvecLevels->at(i).maxScore =
			i < vecLevelScore.size() - 1 ? vecLevelScore[i + 1] : -1;
		pvecLevels->at(i).stepDownTimeSpan =
			i < vecStepDownTimespan.size() ? vecStepDownTimespan[i] : *(vecStepDownTimespan.end() - 1);
		double scoreGainRate =
			i < vecScoreGainRate.size() ? vecScoreGainRate[i] : *(vecScoreGainRate.end() - 1);
		pvecLevels->at(i).scoreRate = 
			(i == 0 ? 1 : pvecLevels->at(i - 1).scoreRate) + scoreGainRate;
	}
	return true;
}

bool Configuration::LoadShapes()
{
	wfstream fs;
	try {
		fs.open(pathClassicShapes, wfstream::in);

		ClaimStringBuffer;
		
		wchar_t group[BUFFER_CHARS] = L"";
		_wsplitpath(pathClassicShapes.c_str(), NULL, NULL, group, NULL);
		wchar_t name[BUFFER_CHARS] = L"";
		bool penetrable = false;
		bool twoRotation;
		bool clockwiseRotation;
		int horizontalCenterOffset = 0;
		int row = 0, col = 0;
		int color = 0;
		vector<char> vecData;

		while (true)
		{
			fs.getline(GetStringBuffer, BUFFER_CHARS);
			wchar_t* psz = GetStringBuffer + wcsspn(GetStringBuffer, L" \t");
			if (wcschr(psz, L':')) // type declare
			{
				if (L"" != name && row != 0 && col != 0)
				{
					TetrisType::Create(group, name, penetrable, twoRotation, clockwiseRotation,
						horizontalCenterOffset,
						row, col, vecData.data(), vecData.size(), color);
					color++;
				}
				ParseTetrisTypeDeclaration(psz, name, &penetrable, &twoRotation, &clockwiseRotation, &horizontalCenterOffset);
				row = col = 0;
				vecData.clear();
			}
			else if (fs.rdstate() & wfstream::eofbit) // end of file
			{
				if (*psz == L'\0') // blank line
				{
					break;
				}
				else
				{
					// type data
					col = (int)wcslen(psz);
					row++;
					for (int i = 0; i < col; i++)
						vecData.push_back((char)psz[i]);
					// last type
					if (L"" != name && row != 0 && col != 0)
					{
						TetrisType::Create(group, name, penetrable, twoRotation, clockwiseRotation,
							horizontalCenterOffset,
							row, col, vecData.data(), vecData.size(), color);
					}
					break;
				}
			}
			else if (L'\0' == *psz) // blank line
			{
				continue;
			}
			else // type data
			{
				col = (int)wcslen(psz);
				row++;
				for (int i = 0; i < col; i++)
					vecData.push_back((char)psz[i]);
			}
		}
	}
	catch(...)
	{
	}

	if (fs.is_open())
		fs.close();

	return true;
}

bool Configuration::LoadColors()
{
	try {
		GetColorFromFile(pathBorderColorFile.c_str(), &colorBorder);
		GetColorFromFile(pathSeparatorColorFile.c_str(), &colorSeparator);
		GetColorsFromFile(pathTetrisColorFile.c_str(), &vecTetrisColors);
		GetColorFromFile(pathMassColorFile.c_str(), &colorMass);
		GetColorFromFile(pathInformationColor.c_str(), &colorInfo);
		GetColorFromFile(pathBackgroundColor.c_str(), &colorBackground);
	}
	catch (...)
	{
		return false;
	}
	return true;
}

bool Configuration::GetColorFromFile(const wchar_t* file, COLORREF* pColor)
{
	try {
		Bitmap* pbmp;
		Color color;
		pbmp = Bitmap::FromFile(file);
		pbmp->GetPixel(0, 0, &color);
		*pColor = color.ToCOLORREF();
		return true;
	}
	catch (...)
	{
		return (COLORREF)0;
	}
}

bool Configuration::GetColorsFromFile(const wchar_t* file, vector<COLORREF>* pvecColors)
{
	try {
		pvecColors->clear();
		Bitmap* pbmp;
		Color color;
		pbmp = Bitmap::FromFile(file);
		for (UINT i = 0; i < pbmp->GetWidth(); i++)
		{
			for (UINT j = 0; j < pbmp->GetHeight(); j++)
			{
				pbmp->GetPixel(i, j, &color);
				if (pvecColors->end() ==
					find(pvecColors->begin(), pvecColors->end(), color.ToCOLORREF()))
				{
					pvecColors->push_back(color.ToCOLORREF());
				}
			}
		}
		return true;
	}
	catch (...)
	{
		pvecColors->clear();
		return false;
	}
}

wstring& Configuration::FindFile(wstring& path)
{
	wstring dir = path.substr(0, path.find_last_of(L'\\') + 1);
	WIN32_FIND_DATA findData;
	HANDLE hFindFile = FindFirstFile(path.c_str(), &findData);
	if (hFindFile)
	{
		if (!(FILE_ATTRIBUTE_DIRECTORY & findData.dwFileAttributes))
			path = dir + findData.cFileName;
		FindClose(hFindFile);
	}
	return path;
}

void Configuration::FindFiles(wstring path, vector<wstring>* pvecFiles)
{
	wstring dir = path.substr(0, path.find_last_of(L'\\') + 1);
	WIN32_FIND_DATA findData;
	HANDLE hFindFile = FindFirstFile(path.c_str(), &findData);
	if (hFindFile)
	{
		do
		{
			if (!(FILE_ATTRIBUTE_DIRECTORY & findData.dwFileAttributes))
				pvecFiles->push_back(dir + findData.cFileName);
		} while (FindNextFile(hFindFile, &findData));
		FindClose(hFindFile);
	}
}

bool Configuration::SaveWindowPostion(int w, int h, int l, int t, bool c)
{
	return false;
}

bool Configuration::SplitStringToInts(wchar_t* szStr, wchar_t ch, int* v1, int* v2)
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

bool Configuration::SplitStringToInts(wchar_t* szStr, wchar_t ch, vector<int>& vecInts)
{
	try {
		vector<wchar_t*> tokens; // token pointers
		tokens.resize(Utility::Spliwstring(szStr, L',', nullptr, 0));
		Utility::Spliwstring(szStr, L',', tokens.data(), 0);
		vecInts.clear();
		for (size_t i = 0; i < tokens.size(); i++)
		{
			vecInts.push_back(stoi(tokens[i]));
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Configuration::SplitStringToDoubles(wchar_t* szStr, wchar_t ch, vector<double>& vecDoubles)
{
	try {
		vector<wchar_t*> tokens; // token pointers
		tokens.resize(Utility::Spliwstring(szStr, L',', nullptr, 0));
		Utility::Spliwstring(szStr, L',', tokens.data(), 0);
		vecDoubles.clear();
		for (size_t i = 0; i < tokens.size(); i++)
		{
			vecDoubles.push_back(stod(tokens[i]));
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Configuration::ParseTetrisTypeDeclaration(wchar_t* szStr, wchar_t* name, bool* pPenetrable,
	bool* pTwoRotation, bool* pClockwiseRotation, int* pHorizontalCenterOffset)
{
	try {
		wstring strBuffer(szStr);
		size_t pos = strBuffer.find(L':');
		if (string::npos == pos)
			return false;
		wstring strVal = strBuffer.substr(0, pos);
		copy(strVal.begin(), strVal.end(), name);
		strVal = strBuffer.substr(pos + 1);
		pos = strVal.find(L',');
		*pPenetrable = L"1" == strVal.substr(0, pos);
		strVal = strVal.substr(pos + 1);
		pos = strVal.find(L',');
		*pTwoRotation = L"1" == strVal.substr(0, pos);
		strVal = strVal.substr(pos + 1);
		pos = strVal.find(L',');
		*pClockwiseRotation = L"1" == strVal.substr(0, pos);
		strVal = strVal.substr(pos + 1);
		*pHorizontalCenterOffset = stoi(strVal);
		return true;
	}
	catch (...)
	{
		return false;
	}
}
