#include <string>
#include <Windows.h>
#include <vector>
#include "tcharstd.h"
using namespace std;

#pragma once

class Configuration
{
private:
	// path
	const tstring INIS_PATH =  L"inis";
	const tstring BITMAPS_PATH = INIS_PATH + L"\\bitmaps";
	const tstring SHAPES_PATH = INIS_PATH + L"\\shapes";

	const tstring CONFIGURATION_PATH = INIS_PATH + L"\\configuration.txt";
	const tstring TETRIS_COLOR_FILE_PATH = BITMAPS_PATH + L"\\tetris color.bmp";
	const tstring MASS_COLOR_FILE_PATH = BITMAPS_PATH + L"\\mass color.bmp";
	const tstring BORDER_COLOR_FILE_PATH = BITMAPS_PATH + L"\\border color.bmp";
	const tstring SEPARATOR_COLOR_FILE_PATH = BITMAPS_PATH + L"\\separator color.bmp";
	const tstring CLASSIC_SHAPES_PATH = SHAPES_PATH + L"\\classic.txt";

	const tstring ARCHIVES_PATH = L"archives";

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
	tstring pathClassicShapes;

private:
	// keys name
	const tstring keyWindow = L"Window";
	const tstring keyWindowSize = L"WindowSize";
	const tstring keyWindowPostion = L"WindowPostion";
	const tstring keyWindowCenter = L"WindowCenter";
	
	const tstring keySetting = L"Setting";
	const tstring keyFramePostion = L"FramePostion";
	const tstring keyFrameSize = L"FrameSize";
	const tstring keyPromptFramePostion = L"PromptFramePostion";
	const tstring keyPromptFrameSize = L"PromptFrameSize";
	const tstring keyInfoFramePosition = L"InfoFramePosition";
	const tstring keyInfoFrameSize = L"InfoFrameSize";
	const tstring keyBorderThickness = L"BorderThickness";
	const tstring keyUnitWidth = L"UnitWidth";
	const tstring keySeparatorThickness = L"SeparatorThickness";
	const tstring keyStepDownTimespan = L"StepDownTimespan";
	const tstring keyDropTimespan = L"DropTimespan";
	const tstring keyDropDelay = L"DropDelay";
	const tstring keyRemoveDelay = L"RemoveDelay";
	const tstring keyRemoveBlinkTimespan = L"RemoveBlinkTimespan";
	
	const tstring keyGame = L"Game";
	const tstring keyLevel = L"Level";
	const tstring keyStartLine = L"StartLine";
	const tstring keyStartLineBlankRate = L"StartLineBlankRate";
	const tstring keyRemoveScores = L"RemoveScores";
	const tstring keyDroppedScore = L"DroppedScore";

	const tstring keyMusic = L"Music";
	const tstring keyMusicRotate = L"MusicRotate";
	const tstring keyMusicDrop = L"MusicDrop";
	const tstring keyMusicDropped = L"MusicDropped";
	const tstring keyMusicBgm = L"MusicBgm";

	const tstring keyBitmap = L"Bitmap";
	const tstring keyUseColor = L"UseColor";
	const tstring keyUseColorRandom = L"UseColorRandom";
	const tstring keyUnitBitmap = L"UnitBitmap";
	const tstring keyUseMassColor = L"UseMassColor";

public:
	// window
	int windowWidth, windowHeight, windowLeft, windowTop;
	bool windowCenter;
	
	// draw
	int frameLeft, frameTop;
	int frameSizeX, frameSizeY;
	int promptFrameLeft, promptFrameTop;
	int promptFrameSizeX, promptFrameSizeY;
	int infoFrameLeft, infoFrameTop;
	int infoFrameSizeX, infoFrameSizeY;
	int borderThickness;
	int separatorThickness;
	int unitWidth;

	// timespan
	int stepDownTimespan;
	int dropTimespan;
	int dropDelay;
	int removeDelay;
	int removeBlinkTimespan;

	// game setting
	int level;
	int startLine;
	double startLineBlankRate;
	int removeScores[4];
	int droppedScore;

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

public:
	COLORREF colorBorder;
	COLORREF colorSeparator;
	vector<COLORREF> vecTetrisColors;
	COLORREF colorMass;

public:
	static Configuration singleton;

public:
	bool Initialize();

private:
	Configuration() {};
	bool InitializeIniPaths();
	bool LoadParameters();
	bool LoadShapes();
	bool LoadColors();
	bool GetColorFromFile(const TCHAR* file, COLORREF* pColor);
	bool GetColorsFromFile(const TCHAR* file, vector<COLORREF>* pvecColors);

	bool SaveWindowPostion(int w, int h, int l, int t, bool c);

private:
	bool SplitStringToInts(TCHAR* szStr, TCHAR ch, int* v1, int* v2);
	bool SplitStringToInts(TCHAR* szStr, TCHAR ch, int* v1, int* v2, int* v3, int*v4);
	bool ParseTetrisTypeDeclaration(TCHAR* szStr, TCHAR* name,
		bool* pPenetrable, bool* pClockwiseRotation, bool* pTwoRotation, int* pHorizontalCenterOffset);
};

