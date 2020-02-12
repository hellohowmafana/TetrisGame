#include "Configuration.hpp"
#include <fstream> 
#include "TetrisShape.hpp"
#include "Level.hpp"
#include <gdiplus.h>
#include "Background.hpp"
#include "Musician.hpp"
#include "BinarySerializer.h"
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
	if(!CreateDirectoryAbsent(pathArchives))
		return false;
	pathRecords = path + RECORDS_PATH;
	if (!CreateDirectoryAbsent(pathRecords))
		return false;

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
	iconScaleRatio = GetConfigurationFloat(keyDisplay, keyIconScaleRatio);
	maskTransparency = GetConfigurationFloat(keyDisplay, keyMaskTransparency);

	// game
	GetConfigurationStringArray(keyGame, keyShape, vecShapes);
	GetConfigurationInt(keyGame, keyStartLevel);
	startLevel = GetConfigurationInt(keyGame, keyStartLevel);
	startLine = GetConfigurationInt(keyGame, keyStartLine);
	startLineBlankRate = GetConfigurationFloat(keyGame, keyStartLineBlankRate);
	GetConfigurationIntArray(keyGame, keyRemoveScores, vecRemoveScores);
	droppedScore = GetConfigurationInt(keyGame, keyDroppedScore);
	maxLevel = GetConfigurationInt(keyGame, keyMaxLevel);
	GetConfigurationFloatArray(keyGame, keyScoreGainRate, vecScoreGainRate);
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
	shapeBlinkTimespan = GetConfigurationInt(keyGame, keyShapeBlinkTimespan);

	//other
	record = GetConfigurationBool(keyOther, keyRecord);

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
	for (int i = 0; i < maxLevel; i++)
	{
		pvecLevels->at(i).level = i + 1;
		pvecLevels->at(i).minScore =
			i < vecLevelScore.size() ? vecLevelScore[i] : -1;
		pvecLevels->at(i).maxScore =
			i < vecLevelScore.size() - 1 ? vecLevelScore[i + 1] : -1;
		pvecLevels->at(i).stepDownTimeSpan =
			i < vecStepDownTimespan.size() ? vecStepDownTimespan[i] : *(vecStepDownTimespan.end() - 1);
		float scoreGainRate =
			i < vecScoreGainRate.size() ? vecScoreGainRate[i] : *(vecScoreGainRate.end() - 1);
		pvecLevels->at(i).scoreRate = 
			(i == 0 ? 1 : pvecLevels->at(i - 1).scoreRate) + scoreGainRate;
	}
	return true;
}

bool Configuration::LoadShapes()
{
	wfstream fs;
	for (vector<wstring>::iterator it = vecShapes.begin(); it != vecShapes.end(); it++)
	{
		wstring pathShapeFile = pathShapes + L"\\" + *it + L".txt";
		try
		{
			fs.open(pathShapeFile, wfstream::in);

			wstring group = *it;
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
		catch (...)
		{
		}

		if (fs.is_open())
			fs.close();
	}

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

bool Configuration::CreateDirectoryAbsent(wstring path)
{
	DWORD attr = GetFileAttributes(path.c_str());

	if (INVALID_FILE_ATTRIBUTES == attr)
	{
		// not exist
		if(ERROR_FILE_NOT_FOUND == GetLastError())
			return CreateDirectory(path.c_str(), NULL);
		else
			return false;
	}

	// not directory
	if(!(FILE_ATTRIBUTE_DIRECTORY & attr))
		return CreateDirectory(path.c_str(), NULL);
	else
		// directory exist
		return true;
}

bool Configuration::SaveWindowPostion(int w, int h, int l, int t, bool c)
{
	return false;
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

float Configuration::GetConfigurationFloat(wstring section, wstring key)
{
	try
	{
		const wchar_t* pszBuffer = GetConfigurationString(section, key);
		return stof(pszBuffer);
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

bool Configuration::GetConfigurationFloatArray(wstring section, wstring key, vector<float>& vec)
{
	wchar_t* pszBuffer = GetConfigurationString(section, key);
	return SplitStringToFloats(pszBuffer, L',', vec);
}

bool Configuration::GetConfigurationStringArray(wstring section, wstring key, vector<wstring>& vec)
{
	wchar_t* pszBuffer = GetConfigurationString(section, key);
	return SplitStringToStrings(pszBuffer, L',', vec);
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

bool Configuration::SplitStringToFloats(wstring str, wchar_t ch, vector<float>& vecFloats)
{
	try {
		vector<wchar_t*> tokens; // token pointers
		tokens.resize(Utility::SplitString((wchar_t*)str.c_str(), L',', nullptr, 0));
		Utility::SplitString((wchar_t*)str.c_str(), L',', tokens.data(), 0);
		vecFloats.clear();
		for (size_t i = 0; i < tokens.size(); i++)
		{
			vecFloats.push_back(stof(tokens[i]));
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Configuration::SplitStringToStrings(wstring str, wchar_t ch, vector<wstring>& vecStrings)
{
	try {
		vector<wchar_t*> tokens; // token pointers
		tokens.resize(Utility::SplitString((wchar_t*)str.c_str(), L',', nullptr, 0));
		Utility::SplitString((wchar_t*)str.c_str(), L',', tokens.data(), 0);
		vecStrings.clear();
		for (size_t i = 0; i < tokens.size(); i++)
		{
			vecStrings.push_back(tokens[i]);
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

bool Configuration::Save(char* pData, unsigned int& size, char argument)
{
	BinarySerializer serializer;

	// window
	serializer.PutUshort(pData, windowWidth, size);
	serializer.PutUshort(pData, windowHeight, size);
	serializer.PutUshort(pData, windowLeft, size);
	serializer.PutUshort(pData, windowTop, size);
	serializer.PutBool(pData, windowCenter, size);

	// display
	serializer.PutUshort(pData, frameLeft, size);
	serializer.PutUshort(pData, frameTop, size);
	serializer.PutUchar(pData, frameSizeX, size);
	serializer.PutUchar(pData, frameSizeY, size);
	serializer.PutUshort(pData, promptFrameLeft, size);
	serializer.PutUshort(pData, promptFrameTop, size);
	serializer.PutUchar(pData, promptFrameSizeX, size);
	serializer.PutUchar(pData, promptFrameSizeY, size);
	serializer.PutUshort(pData, infoFrameLeft, size);
	serializer.PutUshort(pData, infoFrameTop, size);
	serializer.PutUshort(pData, infoFrameSizeX, size);
	serializer.PutUshort(pData, infoFrameSizeY, size);
	serializer.PutUchar(pData, borderThickness, size);
	serializer.PutUchar(pData, separatorThickness, size);
	serializer.PutUshort(pData, unitWidth, size);
	serializer.PutWstring(pData, infoFontFace, size);
	serializer.PutUshort(pData, infoFontHeight, size);
	serializer.PutUshort(pData, infoFontWidth, size);
	serializer.PutUshort(pData, infoFontWeight, size);
	serializer.PutFloat(pData, iconScaleRatio, size);
	serializer.PutFloat(pData, maskTransparency, size);

	// game
	serializer.PutUshort(pData, startLevel, size);
	serializer.PutUshort(pData, startLine, size);
	serializer.PutFloat(pData, startLineBlankRate, size);
	serializer.PutUshortArray(pData, vecRemoveScores, size);
	serializer.PutUshort(pData, droppedScore, size);
	serializer.PutUchar(pData, maxLevel, size);
	serializer.PutFloatArray(pData, vecScoreGainRate, size);
	serializer.PutUshortArray(pData, vecLevelScore, size);
	serializer.PutUshortArray(pData, vecStepDownTimespan, size);
	serializer.PutUshort(pData, dropTimespan, size);
	serializer.PutBool(pData, dropImmediate, size);
	serializer.PutUshort(pData, removeBlinkTimespan, size);
	serializer.PutUchar(pData, removeBlinkCount, size);
	serializer.PutUshort(pData, rollTimespan, size);
	serializer.PutUshort(pData, resumeDelayTimespan, size);

	return true;
}

bool Configuration::Load(char* pData)
{
	BinarySerializer serializer;

	// window
	serializer.TakeUshort(pData, windowWidth);
	serializer.TakeUshort(pData, windowHeight);
	serializer.TakeUshort(pData, windowLeft);
	serializer.TakeUshort(pData, windowTop);
	serializer.TakeBool(pData, windowCenter);

	// display
	serializer.TakeUshort(pData, frameLeft);
	serializer.TakeUshort(pData, frameTop);
	serializer.TakeUchar(pData, frameSizeX);
	serializer.TakeUchar(pData, frameSizeY);
	serializer.TakeUshort(pData, promptFrameLeft);
	serializer.TakeUshort(pData, promptFrameTop);
	serializer.TakeUchar(pData, promptFrameSizeX);
	serializer.TakeUchar(pData, promptFrameSizeY);
	serializer.TakeUshort(pData, infoFrameLeft);
	serializer.TakeUshort(pData, infoFrameTop);
	serializer.TakeUshort(pData, infoFrameSizeX);
	serializer.TakeUshort(pData, infoFrameSizeY);
	serializer.TakeUchar(pData, borderThickness);
	serializer.TakeUchar(pData, separatorThickness);
	serializer.TakeUshort(pData, unitWidth);
	serializer.TakeWstring(pData, infoFontFace);
	serializer.TakeUshort(pData, infoFontHeight);
	serializer.TakeUshort(pData, infoFontWidth);
	serializer.TakeUshort(pData, infoFontWeight);
	serializer.TakeFloat(pData, iconScaleRatio);
	serializer.TakeFloat(pData, maskTransparency);

	// game
	serializer.TakeUshort(pData, startLevel);
	serializer.TakeUshort(pData, startLine);
	serializer.TakeFloat(pData, startLineBlankRate);
	serializer.TakeUshortArray(pData, vecRemoveScores);
	serializer.TakeUshort(pData, droppedScore);
	serializer.TakeUchar(pData, maxLevel);
	serializer.TakeFloatArray(pData, vecScoreGainRate);
	serializer.TakeUshortArray(pData, vecLevelScore);
	serializer.TakeUshortArray(pData, vecStepDownTimespan);
	serializer.TakeUshort(pData, dropTimespan);
	serializer.TakeBool(pData, dropImmediate);
	serializer.TakeUshort(pData, removeBlinkTimespan);
	serializer.TakeUchar(pData, removeBlinkCount);
	serializer.TakeUshort(pData, rollTimespan);
	serializer.TakeUshort(pData, resumeDelayTimespan);

	return true;
}

