#include <string>
#include <Windows.h>
#include <vector>
using namespace std;

#pragma once

class Configuration
{
private:
	// path
	const wstring INIS_PATH =  L"inis";
	const wstring BITMAPS_PATH = INIS_PATH + L"\\bitmaps";
	const wstring SHAPES_PATH = INIS_PATH + L"\\shapes";

	const wstring CONFIGURATION_PATH = INIS_PATH + L"\\configuration.txt";
	const wstring TETRIS_COLOR_FILE_PATH = BITMAPS_PATH + L"\\tetris color.bmp";
	const wstring MASS_COLOR_FILE_PATH = BITMAPS_PATH + L"\\mass color.bmp";
	const wstring BORDER_COLOR_FILE_PATH = BITMAPS_PATH + L"\\border color.bmp";
	const wstring SEPARATOR_COLOR_FILE_PATH = BITMAPS_PATH + L"\\separator color.bmp";
	const wstring CLASSIC_SHAPES_PATH = SHAPES_PATH + L"\\classic.txt";

public:
	// path for use
	wstring pathInis;
	wstring pathBitmaps;
	wstring pathShapes;

	wstring pathConfiguration;
	wstring pathTetrisColorFile;
	wstring pathMassColorFile;
	wstring	pathBorderColorFile;
	wstring pathSeparatorColorFile;
	wstring pathClassicShapes;

private:
	// keys name
	const wstring keyWindow = L"Window";
	const wstring keyWindowSize = L"WindowSize";
	const wstring keyWindowPostion = L"WindowPostion";
	const wstring keyWindowCenter = L"WindowCenter";
	
	const wstring keySetting = L"Setting";
	const wstring keyFramePostion = L"FramePostion";
	const wstring keyFrameSize = L"FrameSize";
	const wstring keyPromptFramePostion = L"PromptFramePostion";
	const wstring keyPromptFrameSize = L"PromptFrameSize";
	const wstring keyInfoFramePosition = L"InfoFramePosition";
	const wstring keyInfoFrameSize = L"InfoFrameSize";
	const wstring keyBorderThickness = L"BorderThickness";
	const wstring keyUnitWidth = L"UnitWidth";
	const wstring keySeparatorThickness = L"SeparatorThickness";
	const wstring keyDropTimespan = L"DropTimespan";
	const wstring keyDropDelay = L"DropDelay";
	const wstring keyDroppedDelay = L"DroppedDelay";
	const wstring keyRemoveDelay = L"RemoveDelay";
	const wstring keyRemoveBlinkTimespan = L"RemoveBlinkTimespan";
	
	const wstring keyGame = L"Game";
	const wstring keyLevel = L"Level";
	const wstring keyStartLine = L"StartLine";
	const wstring keyStartLineBlankRate = L"StartLineBlankRate";
	const wstring keyRemoveScores = L"RemoveScores";
	const wstring keyDroppedScore = L"DroppedScore";

	const wstring keyMusic = L"Music";
	const wstring keyMusicRotate = L"MusicRotate";
	const wstring keyMusicDrop = L"MusicDrop";
	const wstring keyMusicDropped = L"MusicDropped";
	const wstring keyMusicBgm = L"MusicBgm";

	const wstring keyBitmap = L"Bitmap";
	const wstring keyUseColor = L"UseColor";
	const wstring keyUseColorRandom = L"UseColorRandom";
	const wstring keyUnitBitmap = L"UnitBitmap";
	const wstring keyUseDroppedColor = L"UseDroppedColor";

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
	int dropTimespan;
	int dropDelay;
	int droppedDelay;
	int removeDelay;
	int removeBlinkTimespan;

	// game setting
	int level;
	int startLine;
	double startLineBlankRate;
	int removeScores[4];
	int droppedScore;

	// music
	wstring musicRotate;
	wstring musicDrop;
	wstring musicDropped;
	wstring musicBgm;

	// bitmaps
	bool useColor;
	bool useColorRandom;
	wstring unitBitmap;
	bool useDroppedColor;

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
	bool SplitStringToStrInt(TCHAR* szStr, TCHAR ch, TCHAR* szv, int iv);
};

