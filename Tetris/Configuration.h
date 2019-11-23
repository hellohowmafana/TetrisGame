#pragma once
#include <string>
#include <Windows.h>
#include <vector>
using namespace std;

class Background;

enum class BackgroundMode;

class Configuration
{
private:
	// path
	const wstring INIS_PATH =  L"inis";
	const wstring BITMAPS_PATH = INIS_PATH + L"\\bitmaps";
	const wstring ICONS_PATH = INIS_PATH + L"\\icons";
	const wstring SHAPES_PATH = INIS_PATH + L"\\shapes";
	const wstring SOUND_PATH = INIS_PATH + L"\\sound";
	const wstring BGM_PATH = INIS_PATH + L"\\bgm";
	const wstring ARCHIVES_PATH = L"archives";

	const wstring CONFIGURATION_PATH = INIS_PATH + L"\\configuration.txt";

	const wstring UNIT_BITMAP_FILE_PATH = BITMAPS_PATH + L"\\unit.bmp";
	const wstring TETRIS_COLOR_FILE_PATH = BITMAPS_PATH + L"\\tetris color.bmp";
	const wstring MASS_COLOR_FILE_PATH = BITMAPS_PATH + L"\\mass color.bmp";
	const wstring BORDER_COLOR_FILE_PATH = BITMAPS_PATH + L"\\border color.bmp";
	const wstring SEPARATOR_COLOR_FILE_PATH = BITMAPS_PATH + L"\\separator color.bmp";
	const wstring INFORMATION_COLOR_FILE_PATH = BITMAPS_PATH + L"\\information color.bmp";
	const wstring BACKGROUND_COLOR_FILE_PATH = BITMAPS_PATH + L"\\background color.bmp";
	const wstring BACKGROUND_FILE_PATH = BITMAPS_PATH + L"\\background.*";
	const wstring GAMEOVER_FILE_PATH = BITMAPS_PATH + L"\\game over.*";

	const wstring PAUSE_ICON_FILE_PATH = ICONS_PATH + L"\\pause.*";
	const wstring RESUME_ICON_FILE_PATH = ICONS_PATH + L"\\resume.*";

	const wstring CLASSIC_SHAPES_PATH = SHAPES_PATH + L"\\classic.txt";

	const wstring STEPDOWN_SOUND_FILE_PATH = SOUND_PATH + L"\\step down.mp3";
	const wstring STEPHORIZONTAL_SOUND_FILE_PATH = SOUND_PATH + L"\\step horizontal.mp3";
	const wstring ROTATE_SOUND_FILE_PATH = SOUND_PATH + L"\\rotate.mp3";
	const wstring DROPPED_SOUND_FILE_PATH = SOUND_PATH + L"\\dropped.mp3";
	const wstring REMOVE_SOUND_FILE_PATH = SOUND_PATH + L"\\remove.mp3";
	const wstring GAMEOVER_SOUND_FILE_PATH = SOUND_PATH + L"\\game over.mp3";

public:
	// path for use
	wstring pathInis;
	wstring pathBitmaps;
	wstring pathIcons;
	wstring pathShapes;
	wstring pathSound;
	wstring pathBgm;
	wstring pathArchives;

	wstring pathConfiguration;
	wstring pathUnitBitmapFile;
	wstring pathTetrisColorFile;
	wstring pathMassColorFile;
	wstring	pathBorderColorFile;
	wstring pathSeparatorColorFile;
	wstring pathInformationColor;
	wstring pathBackgroundColor;
	wstring pathBackground;
	wstring pathGameOver;

	wstring pathPauseIcon;
	wstring pathResumeIcon;

	wstring pathClassicShapes;

	wstring pathStepDownSound;
	wstring pathStepHorizontalSound;
	wstring pathRotateSound;
	wstring pathDroppedSound;
	wstring pathRemoveSound;
	wstring pathGameOverSound;

private:
	// keys name
	const wstring keyWindow = L"Window";
	const wstring keyWindowSize = L"WindowSize";
	const wstring keyWindowPostion = L"WindowPostion";
	const wstring keyWindowCenter = L"WindowCenter";
	
	const wstring keyDisplay = L"Display";
	const wstring keyFramePostion = L"FramePostion";
	const wstring keyFrameSize = L"FrameSize";
	const wstring keyPromptFramePostion = L"PromptFramePostion";
	const wstring keyPromptFrameSize = L"PromptFrameSize";
	const wstring keyInfoFramePosition = L"InfoFramePosition";
	const wstring keyInfoFrameSize = L"InfoFrameSize";
	const wstring keyBorderThickness = L"BorderThickness";
	const wstring keySeparatorThickness = L"SeparatorThickness";
	const wstring keyUnitWidth = L"UnitWidth";
	const wstring keyInfoFontFace = L"InfoFontFace";
	const wstring keyInfoFontHeight = L"InfoFontHeight";
	const wstring keyInfoFontWidth = L"InfoFontWidth"; 
	const wstring keyInfoFontWeight = L"InfoFontWeight";
	const wstring keyIconScaleRatio = L"IconScaleRatio";
	const wstring keyMaskTransparency = L"MaskTransparency";

	const wstring keyGame = L"Game";
	const wstring keyStartLevel = L"StartLevel";
	const wstring keyStartLine = L"StartLine";
	const wstring keyStartLineBlankRate = L"StartLineBlankRate";
	const wstring keyRemoveScores = L"RemoveScores";
	const wstring keyDroppedScore = L"DroppedScore";
	const wstring keyMaxLevel = L"MaxLevel";
	const wstring keyScoreGainRate = L"ScoreGainRate";
	const wstring keyLevelScore = L"LevelScore";
	const wstring keyStepDownTimespan = L"StepDownTimespan";
	const wstring keyDropTimespan = L"DropTimespan";
	const wstring keyDropImmediate = L"DropImmediate";
	const wstring keyRemoveBlinkTimespan = L"RemoveBlinkTimespan";
	const wstring keyRemoveBlinkCount = L"RemoveBlinkCount";
	const wstring keyRollTimespan = L"RollTimespan";
	const wstring keyResumeDelayTimespan = L"ResumeDelayTimespan";

	const wstring keyMusic = L"Music";
	const wstring keySoundOn = L"SoundOn";
	const wstring keyBgmOn = L"BgmOn";
	const wstring keyRandomBgm = L"RandomBgm";

	const wstring keyBitmap = L"Bitmap";
	const wstring keyUseColor = L"UseColor";
	const wstring keyUseColorRandom = L"UseColorRandom";
	const wstring keyUnitBitmap = L"UnitBitmap";
	const wstring keyUseMassColor = L"UseMassColor";
	const wstring keyBackgroundMode = L"BackgroundMode";

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
	wstring infoFontFace;
	int infoFontHeight;
	int infoFontWidth;
	int infoFontWeight;
	double iconScaleRatio;
	double maskTransparency;

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
	int removeBlinkTimespan;
	int removeBlinkCount;
	int rollTimespan;
	int resumeDelayTimespan;

	// music
	bool soundOn;
	bool bgmOn;
	vector<wstring> vecBgms;
	bool randomBgm;

	// bitmaps
	bool useColor;
	bool useColorRandom;
	wstring unitBitmap;
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
	bool GetColorFromFile(const wchar_t* file, COLORREF* pColor);
	bool GetColorsFromFile(const wchar_t* file, vector<COLORREF>* pvecColors);
	wstring& FindFile(wstring& path);
	void FindFiles(wstring path, vector<wstring>* pvecFiles);

	bool SaveWindowPostion(int w, int h, int l, int t, bool c);

private:
	bool SplitStringToInts(wchar_t* szStr, wchar_t ch, int* v1, int* v2);
	bool SplitStringToInts(wchar_t* szStr, wchar_t ch, vector<int>& vecInts);
	bool SplitStringToDoubles(wchar_t* szStr, wchar_t ch, vector<double>& vecDoubles);
	bool ParseTetrisTypeDeclaration(wchar_t* szStr, wchar_t* name,
		bool* pPenetrable, bool* pClockwiseRotation, bool* pTwoRotation, int* pHorizontalCenterOffset);
};

