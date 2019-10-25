#pragma once
#include <string>
#include <Windows.h>
#include <vector>
#include <tchar.h>
#include "tcharstd.h"
using namespace std;

class Background;

enum class BackgroundMode;

class Configuration
{
private:
	// path
	const tstring INIS_PATH =  _T("inis");
	const tstring BITMAPS_PATH = INIS_PATH + _T("\\bitmaps");
	const tstring SHAPES_PATH = INIS_PATH + _T("\\shapes");

	const tstring CONFIGURATION_PATH = INIS_PATH + _T("\\configuration.txt");
	const tstring TETRIS_COLOR_FILE_PATH = BITMAPS_PATH + _T("\\tetris color.bmp");
	const tstring MASS_COLOR_FILE_PATH = BITMAPS_PATH + _T("\\mass color.bmp");
	const tstring BORDER_COLOR_FILE_PATH = BITMAPS_PATH + _T("\\border color.bmp");
	const tstring SEPARATOR_COLOR_FILE_PATH = BITMAPS_PATH + _T("\\separator color.bmp");
	const tstring INFORMATION_COLOR_FILE_PATH = BITMAPS_PATH + _T("\\information color.bmp");
	const tstring BACKGROUND_COLOR_FILE_PATH = BITMAPS_PATH + _T("\\background color.bmp");
	const tstring BACKGROUND_FILE_PATH = BITMAPS_PATH + _T("\\background.*");
	const tstring CLASSIC_SHAPES_PATH = SHAPES_PATH + _T("\\classic.txt");

	const tstring ARCHIVES_PATH = _T("archives");

public:
	// path for use
	tstring pathInis;
	tstring pathBitmaps;
	tstring pathShapes;
	tstring pathArchives;

	tstring pathConfiguration;
	tstring pathTetrisColorFile;
	tstring pathMassColorFile;
	tstring	pathBorderColorFile;
	tstring pathSeparatorColorFile;
	tstring pathInformationColor;
	tstring pathBackgroundColor;
	tstring pathBackground;
	tstring pathClassicShapes;

private:
	// keys name
	const tstring keyWindow = _T("Window");
	const tstring keyWindowSize = _T("WindowSize");
	const tstring keyWindowPostion = _T("WindowPostion");
	const tstring keyWindowCenter = _T("WindowCenter");
	
	const tstring keyDisplay = _T("Display");
	const tstring keyFramePostion = _T("FramePostion");
	const tstring keyFrameSize = _T("FrameSize");
	const tstring keyPromptFramePostion = _T("PromptFramePostion");
	const tstring keyPromptFrameSize = _T("PromptFrameSize");
	const tstring keyInfoFramePosition = _T("InfoFramePosition");
	const tstring keyInfoFrameSize = _T("InfoFrameSize");
	const tstring keyBorderThickness = _T("BorderThickness");
	const tstring keySeparatorThickness = _T("SeparatorThickness");
	const tstring keyUnitWidth = _T("UnitWidth");
	const tstring keyInfoFontFace = _T("InfoFontFace");
	const tstring keyInfoFontHeight = _T("InfoFontHeight");
	const tstring keyInfoFontWidth = _T("InfoFontWidth"); 
	const tstring keyInfoFontWeight = _T("InfoFontWeight");
	
	const tstring keyGame = _T("Game");
	const tstring keyStartLevel = _T("StartLevel");
	const tstring keyStartLine = _T("StartLine");
	const tstring keyStartLineBlankRate = _T("StartLineBlankRate");
	const tstring keyRemoveScores = _T("RemoveScores");
	const tstring keyDroppedScore = _T("DroppedScore");
	const tstring keyMaxLevel = _T("MaxLevel");
	const tstring keyScoreGainRate = _T("ScoreGainRate");
	const tstring keyLevelScore = _T("LevelScore");
	const tstring keyStepDownTimespan = _T("StepDownTimespan");
	const tstring keyDropTimespan = _T("DropTimespan");
	const tstring keyDropImmediate = _T("DropImmediate");
	const tstring keyDropDelay = _T("DropDelay");
	const tstring keyRemoveBlinkTimespan = _T("RemoveBlinkTimespan");
	const tstring keyRemoveBlinkTimes = _T("RemoveBlinkTimes");

	const tstring keyMusic = _T("Music");
	const tstring keyMusicRotate = _T("MusicRotate");
	const tstring keyMusicDrop = _T("MusicDrop");
	const tstring keyMusicDropped = _T("MusicDropped");
	const tstring keyMusicBgm = _T("MusicBgm");

	const tstring keyBitmap = _T("Bitmap");
	const tstring keyUseColor = _T("UseColor");
	const tstring keyUseColorRandom = _T("UseColorRandom");
	const tstring keyUnitBitmap = _T("UnitBitmap");
	const tstring keyUseMassColor = _T("UseMassColor");
	const tstring keyBackgroundMode = _T("BackgroundMode");

public:
	// window
	int windowWidth, windowHeight, windowLeft, windowTop;
	bool windowCenter;
	
	// display
	int frameLeft, frameTop;
	int frameSizeX, frameSizeY;
	int promptFrameLeft, promptFrameTop;
	int promptFrameSizeX, promptFrameSizeY;
	int infoFrameLeft, infoFrameTop;
	int infoFrameSizeX, infoFrameSizeY;
	int borderThickness;
	int separatorThickness;
	int unitWidth;
	tstring infoFontFace;
	int infoFontHeight;
	int infoFontWidth;
	int infoFontWeight;

	// game
	int startLevel;
	int startLine;
	double startLineBlankRate;
	vector<int> vecRemoveScores;
	int droppedScore;
	int maxLevel;
	vector<double> vecScoreGainRate;
	vector<int> vecLevelScore;
	vector<int> vecStepDownTimespan;
	int dropTimespan;
	bool dropImmediate;
	int dropDelay;
	int removeBlinkTimespan;
	int removeBlinkTimes;

	// music
	tstring musicRotate;
	tstring musicDrop;
	tstring musicDropped;
	tstring musicBgm;

	// bitmaps
	bool useColor;
	bool useColorRandom;
	tstring unitBitmap;
	bool useMassColor;
	BackgroundMode backgroundMode;

public:
	COLORREF colorBorder;
	COLORREF colorSeparator;
	vector<COLORREF> vecTetrisColors;
	COLORREF colorMass;
	COLORREF colorInfo;
	COLORREF colorBackground;

public:
	static Configuration singleton;

public:
	bool Initialize();

private:
	Configuration() {};
	bool InitializeIniPaths();
	bool LoadParameters();
	bool LoadLevels();
	bool LoadShapes();
	bool LoadColors();
	bool GetColorFromFile(const TCHAR* file, COLORREF* pColor);
	bool GetColorsFromFile(const TCHAR* file, vector<COLORREF>* pvecColors);
	tstring& FindFile(tstring& path);
	void FindFiles(tstring& path, vector<tstring>* pvecFiles);

	bool SaveWindowPostion(int w, int h, int l, int t, bool c);

private:
	bool SplitStringToInts(TCHAR* szStr, TCHAR ch, int* v1, int* v2);
	bool SplitStringToInts(TCHAR* szStr, TCHAR ch, vector<int>& vecInts);
	bool SplitStringToDoubles(TCHAR* szStr, TCHAR ch, vector<double>& vecDoubles);
	bool ParseTetrisTypeDeclaration(TCHAR* szStr, TCHAR* name,
		bool* pPenetrable, bool* pClockwiseRotation, bool* pTwoRotation, int* pHorizontalCenterOffset);
};

