#include "Configuration.hpp"
#include <fstream> 
#include "TetrisShape.hpp"
#include "Level.hpp"
#include <gdiplus.h>
#include "Background.hpp"
#include "Musician.hpp"
using namespace Gdiplus;

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
	pathFrameBackgroundColorFile = path + FRAME_BACKGROUND_COLOR_FILE_PATH;
	pathBorderColorFile = path + BORDER_COLOR_FILE_PATH;
	pathSeparatorColorFile = path + SEPARATOR_COLOR_FILE_PATH;
	pathInformationColor = path + INFORMATION_COLOR_FILE_PATH;
	pathBackgroundColor = path + BACKGROUND_COLOR_FILE_PATH;
	pathBackground = path + BACKGROUND_FILE_PATH;
	FindFile(pathBackground);
	pathBeginSplash = path + BEGIN_SPLASH_FILE_PATH;
	FindFile(pathBeginSplash);
	pathGameOverSplash = path + GAMEOVER_SPLASH_FILE_PATH;
	FindFile(pathGameOverSplash);

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
	// window
	GetConfigurationIntPair(keyWindow, keyWindowSize, windowWidth, windowHeight);
	GetConfigurationIntPair(keyWindow, keyWindowPostion, windowLeft, windowTop);
	windowCenter = GetConfigurationBool(keyWindow, keyWindowCenter);

	// display
	GetConfigurationIntPair(keyDisplay, keyFramePostion, frameLeft, frameTop);	
	GetConfigurationIntPair(keyDisplay, keyFrameSize, frameSizeX, frameSizeY);
	GetConfigurationIntPair(keyDisplay, keyPromptFramePostion, promptFrameLeft, promptFrameTop);
	GetConfigurationIntPair(keyDisplay, keyPromptFrameSize, promptFrameSizeX, promptFrameSizeY);
	GetConfigurationIntPair(keyDisplay, keyInfoFramePosition, infoFrameLeft, infoFrameTop);
	GetConfigurationIntPair(keyDisplay, keyInfoFrameSize, infoFrameSizeX, infoFrameSizeY);
	borderThickness = GetConfigurationInt(keyDisplay, keyBorderThickness);
	separatorThickness = GetConfigurationInt(keyDisplay, keySeparatorThickness);
	unitWidth = GetConfigurationInt(keyDisplay, keyUnitWidth);
	infoFontFace = GetConfigurationString(keyDisplay, keyInfoFontFace);
	infoFontHeight = GetConfigurationInt(keyDisplay, keyInfoFontHeight);
	infoFontWidth = GetConfigurationInt(keyDisplay, keyInfoFontWidth);
	infoFontWeight = GetConfigurationInt(keyDisplay, keyInfoFontWeight);
	iconScaleRatio = GetConfigurationDouble(keyDisplay, keyIconScaleRatio);
	maskTransparency = GetConfigurationDouble(keyDisplay, keyMaskTransparency);

	// game
	startLevel = GetConfigurationInt(keyGame, keyStartLevel);
	startLine = GetConfigurationInt(keyGame, keyStartLine);
	startLineBlankRate = GetConfigurationDouble(keyGame, keyStartLineBlankRate);
	GetConfigurationIntArray(keyGame, keyRemoveScores, vecRemoveScores);
	droppedScore = GetConfigurationInt(keyGame, keyDroppedScore);
	maxLevel = GetConfigurationInt(keyGame, keyMaxLevel);
	GetConfigurationDoubleArray(keyGame, keyScoreGainRate, vecScoreGainRate);
	GetConfigurationIntArray(keyGame, keyLevelScore, vecLevelScore);
	GetConfigurationIntArray(keyGame, keyStepDownTimespan, vecStepDownTimespan);
	dropTimespan = GetConfigurationInt(keyGame, keyDropTimespan);
	dropImmediate = GetConfigurationBool(keyGame, keyDropImmediate);
	stepHorizontalTimespan = GetConfigurationInt(keyGame, keyStepHorizontalTimespan);
	rotateTimespan = GetConfigurationInt(keyGame, keyRotateTimespan);
	removeBlinkTimespan = GetConfigurationInt(keyGame, keyRemoveBlinkTimespan);
	removeBlinkCount = GetConfigurationInt(keyGame, keyRemoveBlinkCount);
	rollTimespan = GetConfigurationInt(keyGame, keyRollTimespan);
	resumeDelayTimespan = GetConfigurationInt(keyGame, keyResumeDelayTimespan);

	// input
	actionLeft = GetConfigurationInt(keyInput, keyActionLeft);
	actionRight = GetConfigurationInt(keyInput, keyActionRight);
	actionDown = GetConfigurationInt(keyInput, keyActionDown);
	actionRotate = GetConfigurationInt(keyInput, keyActionRotate);
	actionDrop = GetConfigurationInt(keyInput, keyActionDrop);
	operationPause = GetConfigurationInt(keyInput, keyOperationPause);
	operationRestart = GetConfigurationInt(keyInput, keyOperationRestart);

	// music
	soundOn = GetConfigurationBool(keyMusic, keySoundOn);
	bgmOn = GetConfigurationBool(keyMusic, keyBgmOn);
	randomBgm = GetConfigurationBool(keyMusic, keyRandomBgm);

	// bitmap
	useColor = GetConfigurationBool(keyBitmap, keyUseColor);
	useColorRandom = GetConfigurationBool(keyBitmap, keyUseColorRandom);
	unitBitmap = GetConfigurationString(keyBitmap, keyUnitBitmap);
	useMassColor = GetConfigurationBool(keyBitmap, keyUseMassColor);
	backgroundMode = (RenderMode)GetConfigurationInt(keyBitmap, keyBackgroundMode);
	int iBackgroundAlignmentHorizontal, iBackgroundAlignmentVertical;
	GetConfigurationIntPair(keyBitmap, keyBackgroundAlignment,
		iBackgroundAlignmentHorizontal, iBackgroundAlignmentVertical);
	backgroundAlignmentHorizontal =	(RenderAlignmentHorizontal)iBackgroundAlignmentHorizontal;
	backgroundAlignmentVertical = (RenderAlignmentVertical)iBackgroundAlignmentVertical;

	return true;
}

bool Configuration::LoadLevels()
{
	vector<Level>* pvecLevels = &Level::vecLevels;
	pvecLevels->resize(maxLevel);
	for (UINT i = 0; i < maxLevel; i++)
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
		
		wstring group = TetrisType::classic;
		wstring name;
		bool penetrable = false;
		bool twoRotation;
		bool clockwiseRotation;
		int horizontalCenterOffset = 0;
		int row = 0, col = 0;
		int color = 0;
		vector<char> vecData;

		while (true)
		{
			fs.getline(buffer, bufferSize);
			wchar_t* psz = buffer + wcsspn(buffer, L" \t");
			if (wcschr(psz, L':')) // type declare
			{
				if (L"" != name && row != 0 && col != 0)
				{
					TetrisType::Create(group, name, penetrable, twoRotation, clockwiseRotation,
						horizontalCenterOffset,
						row, col, vecData.data(), vecData.size(), color);
					color++;
				}
				ParseTetrisTypeDeclaration(psz, name, penetrable, twoRotation, clockwiseRotation, horizontalCenterOffset);
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
		GetColorFromFile(pathFrameBackgroundColorFile, colorFrameBackground);
		GetColorFromFile(pathBorderColorFile, colorBorder);
		GetColorFromFile(pathSeparatorColorFile, colorSeparator);
		GetColorsFromFile(pathTetrisColorFile, vecTetrisColors);
		GetColorFromFile(pathMassColorFile, colorMass);
		GetColorFromFile(pathInformationColor, colorInfo);
		GetColorFromFile(pathBackgroundColor, colorBackground);
	}
	catch (...)
	{
		return false;
	}
	return true;
}

bool Configuration::GetColorFromFile(wstring file, Color& color)
{
	try {
		Bitmap bmp(file.c_str());
		bmp.GetPixel(0, 0, &color);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Configuration::GetColorsFromFile(wstring file, vector<Color>& vecColors)
{
	try {
		vecColors.clear();
		Color color;
		Bitmap bmp(file.c_str());
		for (UINT i = 0; i < bmp.GetWidth(); i++)
		{
			for (UINT j = 0; j < bmp.GetHeight(); j++)
			{
				bmp.GetPixel(i, j, &color);
				bool found = false;
				for (vector<Color>::iterator it = vecColors.begin(); it != vecColors.end(); it++)
				{
					if (color.GetValue() == it->GetValue())
					{
						found = true;
						break;
					}
				}
				if (!found)
					vecColors.push_back(color);
			}
		}
		return true;
	}
	catch (...)
	{
		vecColors.clear();
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

wchar_t* Configuration::GetConfigurationString(wstring section, wstring key)
{
	GetPrivateProfileString(section.c_str(), key.c_str(), L"", buffer, bufferSize, pathConfiguration.c_str());
	return buffer;
}

int Configuration::GetConfigurationInt(wstring section, wstring key)
{
	return GetPrivateProfileInt(section.c_str(), key.c_str(), 0, pathConfiguration.c_str());
}

bool Configuration::GetConfigurationBool(wstring section, wstring key)
{
	return GetConfigurationInt(section,key);
}

double Configuration::GetConfigurationDouble(wstring section, wstring key)
{
	try
	{
		const wchar_t* pszBuffer = GetConfigurationString(section, key);
		return stod(pszBuffer);
	}
	catch (const std::exception&)
	{
		return 0.0;
	}
}

bool Configuration::GetConfigurationIntPair(wstring section, wstring key, int& val1, int& val2)
{
	wchar_t* pszBuffer = GetConfigurationString(section, key);
	return SplitStringToInts(pszBuffer, L',', val1, val2);
}

bool Configuration::GetConfigurationIntArray(wstring section, wstring key, vector<int>& vec)
{
	wchar_t* pszBuffer = GetConfigurationString(section, key);
	return SplitStringToInts(pszBuffer, L',', vec);
}

bool Configuration::GetConfigurationDoubleArray(wstring section, wstring key, vector<double>& vec)
{
	wchar_t* pszBuffer = GetConfigurationString(section, key);
	return SplitStringToDoubles(pszBuffer, L',', vec);
}

bool Configuration::SplitStringToInts(wstring str, wchar_t ch, int& v1, int& v2)
{
	try {
		size_t pos = str.find(ch);
		if (string::npos == pos)
		return false;
		wstring strVal = str.substr(0, pos);
		v1 = stoi(strVal);
		strVal = str.substr(pos + 1);
		v2 = stoi(strVal);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Configuration::SplitStringToInts(wstring str, wchar_t ch, vector<int>& vecInts)
{
	try {
		vector<wchar_t*> tokens; // token pointers
		tokens.resize(Utility::SplitString((wchar_t*)str.c_str(), L',', nullptr, 0));
		Utility::SplitString((wchar_t*)str.c_str(), L',', tokens.data(), 0);
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

bool Configuration::SplitStringToDoubles(wstring str, wchar_t ch, vector<double>& vecDoubles)
{
	try {
		vector<wchar_t*> tokens; // token pointers
		tokens.resize(Utility::SplitString((wchar_t*)str.c_str(), L',', nullptr, 0));
		Utility::SplitString((wchar_t*)str.c_str(), L',', tokens.data(), 0);
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

bool Configuration::ParseTetrisTypeDeclaration(wstring str, wstring& name, bool& penetrable,
	bool& twoRotation, bool& clockwiseRotation, int& horizontalCenterOffset)
{
	try {
		size_t pos = str.find(L':');
		if (string::npos == pos)
			return false;
		wstring strVal = str.substr(0, pos);
		name = strVal;
		strVal = str.substr(pos + 1);
		pos = strVal.find(L',');
		penetrable = L"1" == strVal.substr(0, pos);
		strVal = strVal.substr(pos + 1);
		pos = strVal.find(L',');
		twoRotation = L"1" == strVal.substr(0, pos);
		strVal = strVal.substr(pos + 1);
		pos = strVal.find(L',');
		clockwiseRotation = L"1" == strVal.substr(0, pos);
		strVal = strVal.substr(pos + 1);
		horizontalCenterOffset = stoi(strVal);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

void Configuration::LoadPreconfiguration(int rowCount, int colCount)
{
	borderThickness = 5, separatorThickness = 2;
	unitWidth = 20;
	frameLeft = 60, frameTop = 60;
	frameSizeX = colCount, frameSizeY = rowCount;
	int frameWidth = borderThickness * 2 + separatorThickness * (frameSizeX + 1) + unitWidth * frameSizeX;
	int frameHeight = borderThickness * 2 + separatorThickness * (frameSizeY + 1) + unitWidth * frameSizeY;
	promptFrameLeft = 120 + frameWidth, promptFrameTop = 70;
	promptFrameSizeX = 5, promptFrameSizeY = 6;
	int promptFrameWidth = borderThickness * 2 + separatorThickness * (promptFrameSizeX + 1) + unitWidth * promptFrameSizeX;
	int promptFrameHeight = borderThickness * 2 + separatorThickness * (promptFrameSizeY + 1) + unitWidth * promptFrameSizeY;
	infoFrameLeft = 60 + frameWidth + frameLeft, infoFrameTop = 40 + promptFrameHeight + promptFrameTop;
	infoFrameSizeX = 120, infoFrameSizeY = 200;
	windowWidth = frameWidth + promptFrameWidth + 180, windowHeight = frameHeight + 180;
}

bool Configuration::SavePositions()
{
	bool success = true;
	success &= SaveConfigurationInt(keyDisplay, keyBorderThickness, borderThickness);
	success &= SaveConfigurationInt(keyDisplay, keySeparatorThickness, separatorThickness);
	success &= SaveConfigurationInt(keyDisplay, keyUnitWidth, unitWidth);
	success &= SaveConfigurationIntPair(keyDisplay, keyFramePostion, frameLeft, frameTop);
	success &= SaveConfigurationIntPair(keyDisplay, keyFrameSize, frameSizeX, frameSizeY);
	success &= SaveConfigurationIntPair(keyDisplay, keyPromptFramePostion, promptFrameLeft, promptFrameTop);
	success &= SaveConfigurationIntPair(keyDisplay, keyPromptFrameSize, promptFrameSizeX, promptFrameSizeY);
	success &= SaveConfigurationIntPair(keyDisplay, keyInfoFramePosition, infoFrameLeft, infoFrameTop);
	success &= SaveConfigurationIntPair(keyDisplay, keyInfoFrameSize, infoFrameSizeX, infoFrameSizeY);
	success &= SaveConfigurationIntPair(keyWindow, keyWindowSize, windowWidth, windowHeight);
	return success;
}

void Configuration::LoadInput(unsigned char left, unsigned char right, unsigned char down, unsigned char drop, unsigned char rotate, unsigned char pause, unsigned char restart)
{
	actionLeft = left;
	actionRight = right;
	actionDown = down;
	actionDrop = drop;
	actionRotate = rotate;
	operationPause = pause;
	operationRestart = restart;
}

bool Configuration::SaveInput()
{
	bool success = true;
	success &= SaveConfigurationInt(keyInput, keyActionLeft, actionLeft);
	success &= SaveConfigurationInt(keyInput, keyActionRight, actionRight);
	success &= SaveConfigurationInt(keyInput, keyActionDown, actionDown);
	success &= SaveConfigurationInt(keyInput, keyActionRotate, actionRotate);
	success &= SaveConfigurationInt(keyInput, keyActionDrop, actionDrop);
	success &= SaveConfigurationInt(keyInput, keyOperationPause, operationPause);
	success &= SaveConfigurationInt(keyInput, keyOperationRestart, operationRestart);
	return success;
}

bool Configuration::SaveBgmOn(bool bgm)
{
	return SaveConfigurationBool(keyMusic, keyBgmOn, bgm);
}

bool Configuration::SaveSoundOn(bool sound)
{
	return SaveConfigurationBool(keyMusic, keySoundOn, sound);
}

bool Configuration::SaveConfigurationInt(wstring section, wstring key, int val)
{
	return 0 != WritePrivateProfileString(section.c_str(), key.c_str(), to_wstring(val).c_str(), pathConfiguration.c_str());
}

bool Configuration::SaveConfigurationIntPair(wstring section, wstring key, int val1, int val2)
{
	return 0 != WritePrivateProfileString(section.c_str(), key.c_str(), (to_wstring(val1) + L',' + to_wstring(val2)).c_str(),
		pathConfiguration.c_str());
}

bool Configuration::SaveConfigurationBool(wstring section, wstring key, bool val)
{
	return 0 != WritePrivateProfileString(section.c_str(), key.c_str(), to_wstring(val ? 1 : 0).c_str(), pathConfiguration.c_str());
}
