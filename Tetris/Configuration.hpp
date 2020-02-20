#pragma once
#include <string>
#include <Windows.h>
#include <vector>
#include <array>
#include <gdiplus.h>
#include "RenderMode.hpp"
#include "IBinarySerializable.hpp"
using namespace std;
using Color = Gdiplus::Color;

class Background;

class Configuration : public IBinarySerializable
{
private:
	// paths
	static const wstring INIS_PATH;
	static const wstring BITMAPS_PATH;
	static const wstring ICONS_PATH;
	static const wstring SHAPES_PATH;
	static const wstring SOUND_PATH;
	static const wstring BGM_PATH;
	static const wstring ARCHIVES_PATH;
	static const wstring RECORDS_PATH;

	static const wstring CONFIGURATION_PATH;

	static const wstring UNIT_BITMAP_FILE_PATH;
	static const wstring TETRIS_COLOR_FILE_PATH;
	static const wstring MASS_COLOR_FILE_PATH;
	static const wstring FRAME_BACKGROUND_COLOR_FILE_PATH;
	static const wstring BORDER_COLOR_FILE_PATH;
	static const wstring SEPARATOR_COLOR_FILE_PATH;
	static const wstring INFORMATION_COLOR_FILE_PATH;
	static const wstring BACKGROUND_COLOR_FILE_PATH;
	static const wstring BACKGROUND_FILE_PATH;
	static const wstring BEGIN_SPLASH_FILE_PATH;
	static const wstring GAMEOVER_SPLASH_FILE_PATH;

	static const wstring PAUSE_ICON_FILE_PATH;
	static const wstring RESUME_ICON_FILE_PATH;

	static const wstring STEPDOWN_SOUND_FILE_PATH;
	static const wstring STEPHORIZONTAL_SOUND_FILE_PATH;
	static const wstring ROTATE_SOUND_FILE_PATH;
	static const wstring DROPPED_SOUND_FILE_PATH;
	static const wstring REMOVE_SOUND_FILE_PATH;
	static const wstring GAMEOVER_SOUND_FILE_PATH;

public:
	// paths for use
	wstring pathInis;
	wstring pathBitmaps;
	wstring pathIcons;
	wstring pathShapes;
	wstring pathSound;
	wstring pathBgm;
	wstring pathArchives;
	wstring pathRecords;

	wstring pathConfiguration;
	wstring pathUnitBitmapFile;
	wstring pathTetrisColorFile;
	wstring pathMassColorFile;
	wstring	pathFrameBackgroundColorFile;
	wstring	pathBorderColorFile;
	wstring pathSeparatorColorFile;
	wstring pathInformationColor;
	wstring pathBackgroundColor;
	wstring pathBackground;
	wstring pathBeginSplash;
	wstring pathGameOverSplash;

	wstring pathPauseIcon;
	wstring pathResumeIcon;

	wstring pathStepDownSound;
	wstring pathStepHorizontalSound;
	wstring pathRotateSound;
	wstring pathDroppedSound;
	wstring pathRemoveSound;
	wstring pathGameOverSound;

private:
	// key names
	static const wstring keyWindow;
	static const wstring keyWindowSize;
	static const wstring keyWindowPostion;
	static const wstring keyWindowCenter;
	
	static const wstring keyDisplay;
	static const wstring keyFramePostion;
	static const wstring keyFrameSize;
	static const wstring keyPromptFramePostion;
	static const wstring keyPromptFrameSize;
	static const wstring keyInfoFramePosition;
	static const wstring keyInfoFrameSize;
	static const wstring keyBorderThickness;
	static const wstring keySeparatorThickness;
	static const wstring keyUnitWidth;
	static const wstring keyInfoFontFace;
	static const wstring keyInfoFontHeight;
	static const wstring keyInfoFontWidth; 
	static const wstring keyInfoFontWeight;
	static const wstring keyIconScaleRatio;
	static const wstring keyMaskTransparency;

	static const wstring keyGame;
	static const wstring keyShape;
	static const wstring keyStartLevel;
	static const wstring keyStartLine;
	static const wstring keyStartLineBlankRate;
	static const wstring keyRemoveScores;
	static const wstring keyDroppedScore;
	static const wstring keyMaxLevel;
	static const wstring keyScoreGainRate;
	static const wstring keyLevelScore;
	static const wstring keyStepDownTimespan;
	static const wstring keyDropTimespan;
	static const wstring keyDropImmediate;
	static const wstring keyStepHorizontalTimespan;
	static const wstring keyRotateTimespan;
	static const wstring keyRemoveBlinkTimespan;
	static const wstring keyRemoveBlinkCount;
	static const wstring keyRollTimespan;
	static const wstring keyResumeDelayTimespan;
	static const wstring keyShapeBlinkTimespan;

	static const wstring keyOther;
	static const wstring keyRecord;

	static const wstring keyMusic;
	static const wstring keySoundOn;
	static const wstring keyBgmOn;
	static const wstring keyRandomBgm;

	static const wstring keyBitmap;
	static const wstring keyUseColor;
	static const wstring keyUseColorRandom;
	static const wstring keyUnitBitmap;
	static const wstring keyUseMassColor;
	static const wstring keyBackgroundMode;
	static const wstring keyBackgroundAlignment;

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
	float iconScaleRatio;
	float maskTransparency;

	// game
	vector<wstring> vecShapes;
	int startLevel;
	int startLine;
	float startLineBlankRate;
	vector<int> vecRemoveScores;
	int droppedScore;
	int maxLevel;
	vector<float> vecScoreGainRate;
	vector<int> vecLevelScore;
	vector<int> vecStepDownTimespan;
	int dropTimespan;
	bool dropImmediate;
	int stepHorizontalTimespan;
	int rotateTimespan;
	int removeBlinkTimespan;
	int removeBlinkCount;
	int rollTimespan;
	int resumeDelayTimespan;
	int shapeBlinkTimespan;

	//other
	bool record;

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
	RenderMode backgroundMode;
	RenderAlignmentHorizontal backgroundAlignmentHorizontal;
	RenderAlignmentVertical backgroundAlignmentVertical;

public:
	Color colorFrameBackground;
	Color colorBorder;
	Color colorSeparator;
	vector<Color> vecTetrisColors;
	Color colorMass;
	Color colorInfo;
	Color colorBackground;

public:
	static Configuration singleton;

private:
	static Configuration* pBackup;

public:
	bool Initialize();
	bool Backup();
	bool Restore();

private:
	Configuration() {};
	bool InitializeIniPaths();
	bool LoadParameters();
	bool LoadLevels();
	bool LoadShapes();
	bool LoadColors();
	bool GetColorFromFile(wstring file, Color& color);
	bool GetColorsFromFile(wstring file, vector<Color>& vecColors);
	wstring& FindFile(wstring& path);
	void FindFiles(wstring path, vector<wstring>* pvecFiles);
	bool CreateDirectoryAbsent(wstring path);

	bool SaveWindowPostion(int w, int h, int l, int t, bool c);

private:
#define MAX_BUFFER_LENGTH 256
	wchar_t buffer[MAX_BUFFER_LENGTH];
	int bufferSize = MAX_BUFFER_LENGTH;
#undef MAX_BUFFER_LENGTH
	wchar_t* GetConfigurationString(wstring section, wstring key);
	int GetConfigurationInt(wstring section, wstring key);
	bool GetConfigurationBool(wstring section, wstring key);
	float GetConfigurationFloat(wstring section, wstring key);
	bool GetConfigurationIntPair(wstring section, wstring key, int& val1, int& val2);
	bool  GetConfigurationIntArray(wstring section, wstring key, vector<int>& vec);
	bool  GetConfigurationFloatArray(wstring section, wstring key, vector<float>& vec);
	bool  GetConfigurationStringArray(wstring section, wstring key, vector<wstring>& vec);

	bool SplitStringToInts(wstring str, wchar_t ch, int& v1, int& v2);
	bool SplitStringToInts(wstring str, wchar_t ch, vector<int>& vecInts);
	bool SplitStringToFloats(wstring str, wchar_t ch, vector<float>& vecFloats);
	bool SplitStringToStrings(wstring str, wchar_t ch, vector<wstring>& vecStrings);
	bool ParseTetrisTypeDeclaration(wstring str, wstring& name,
		bool& penetrable, bool& clockwiseRotation, bool& twoRotation, int& horizontalCenterOffset);

public:
	virtual bool Save(char*& pData, unsigned int& size, char argument = 0);
	virtual bool Load(char*& pData, char argument = 0);
};

