#include "Recorder.hpp"

bool Recorder::SaveParameters(Configuration* pConfiguration, char* pData)
{
	return false;
}

bool Recorder::Save(wstring file, Configuration* pConfiguration, Controller* pController)
{
	ofstream ofs(file.c_str(), ios::binary | ios::trunc, ios::_Default_open_prot);
	ofs.write(head, strlen(head));
	ofs.write((char*)&version, sizeof(version));
	ofs.write(tail, strlen(tail));
	return false;
}

bool Recorder::Load(wstring file, Configuration* pConfiguration, Controller* pController)
{
	return false;
}

bool RecordParameters::Save(char* pData, size_t& size)
{
	if (!pConfiguration)
		return false;

	// just get size while pData is 0
	size = 0;

	// window
	PutUshort(pData, pConfiguration->windowWidth, size);
	PutUshort(pData, pConfiguration->windowHeight, size);
	PutUshort(pData, pConfiguration->windowLeft, size);
	PutUshort(pData, pConfiguration->windowTop, size);
	PutBool(pData, pConfiguration->windowCenter, size);

	// display
	PutUshort(pData, pConfiguration->frameLeft, size);
	PutUshort(pData, pConfiguration->frameTop, size);
	PutUchar(pData, pConfiguration->frameSizeX, size);
	PutUchar(pData, pConfiguration->frameSizeY, size);
	PutUshort(pData, pConfiguration->promptFrameLeft, size);
	PutUshort(pData, pConfiguration->promptFrameTop, size);
	PutUchar(pData, pConfiguration->promptFrameSizeX, size);
	PutUchar(pData, pConfiguration->promptFrameSizeY, size);
	PutUshort(pData, pConfiguration->infoFrameLeft, size);
	PutUshort(pData, pConfiguration->infoFrameTop, size);
	PutUshort(pData, pConfiguration->infoFrameSizeX, size);
	PutUshort(pData, pConfiguration->infoFrameSizeY, size);
	PutUchar(pData, pConfiguration->borderThickness, size);
	PutUchar(pData, pConfiguration->separatorThickness, size);
	PutUshort(pData, pConfiguration->unitWidth, size);
	PutWstring(pData, pConfiguration->infoFontFace, size);
	PutUshort(pData, pConfiguration->infoFontHeight, size);
	PutUshort(pData, pConfiguration->infoFontWidth, size);
	PutUshort(pData, pConfiguration->infoFontWeight, size);
	PutFloat(pData, pConfiguration->iconScaleRatio, size);
	PutFloat(pData, pConfiguration->maskTransparency, size);

	// game
	PutUshort(pData, pConfiguration->startLevel, size);
	PutUshort(pData, pConfiguration->startLine, size);
	PutFloat(pData, pConfiguration->startLineBlankRate, size);
	PutUshortArray(pData, pConfiguration->vecRemoveScores, size);
	PutUshort(pData, pConfiguration->droppedScore, size);
	PutUchar(pData, pConfiguration->maxLevel, size);
	PutFloatArray(pData, pConfiguration->vecScoreGainRate, size);
	PutUshortArray(pData, pConfiguration->vecLevelScore, size);
	PutUshortArray(pData, pConfiguration->vecStepDownTimespan, size);
	PutUshort(pData, pConfiguration->dropTimespan, size);
	PutBool(pData, pConfiguration->dropImmediate, size);
	PutUshort(pData, pConfiguration->removeBlinkTimespan, size);
	PutUchar(pData, pConfiguration->removeBlinkCount, size);
	PutUshort(pData, pConfiguration->rollTimespan, size);
	PutUshort(pData, pConfiguration->resumeDelayTimespan, size);

	return true;
}

bool RecordParameters::Load(char* pData)
{
	if (!pConfiguration)
		return false;

	// window
	TakeUshort(pData, pConfiguration->windowWidth);
	TakeUshort(pData, pConfiguration->windowHeight);
	TakeUshort(pData, pConfiguration->windowLeft);
	TakeUshort(pData, pConfiguration->windowTop);
	TakeBool(pData, pConfiguration->windowCenter);

	// display
	TakeUshort(pData, pConfiguration->frameLeft);
	TakeUshort(pData, pConfiguration->frameTop);
	TakeUchar(pData, pConfiguration->frameSizeX);
	TakeUchar(pData, pConfiguration->frameSizeY);
	TakeUshort(pData, pConfiguration->promptFrameLeft);
	TakeUshort(pData, pConfiguration->promptFrameTop);
	TakeUchar(pData, pConfiguration->promptFrameSizeX);
	TakeUchar(pData, pConfiguration->promptFrameSizeY);
	TakeUshort(pData, pConfiguration->infoFrameLeft);
	TakeUshort(pData, pConfiguration->infoFrameTop);
	TakeUshort(pData, pConfiguration->infoFrameSizeX);
	TakeUshort(pData, pConfiguration->infoFrameSizeY);
	TakeUchar(pData, pConfiguration->borderThickness);
	TakeUchar(pData, pConfiguration->separatorThickness);
	TakeUshort(pData, pConfiguration->unitWidth);
	TakeWstring(pData, pConfiguration->infoFontFace);
	TakeUshort(pData, pConfiguration->infoFontHeight);
	TakeUshort(pData, pConfiguration->infoFontWidth);
	TakeUshort(pData, pConfiguration->infoFontWeight);
	TakeFloat(pData, pConfiguration->iconScaleRatio);
	TakeFloat(pData, pConfiguration->maskTransparency);

	// game
	TakeUshort(pData, pConfiguration->startLevel);
	TakeUshort(pData, pConfiguration->startLine);
	TakeFloat(pData, pConfiguration->startLineBlankRate);
	TakeUshortArray(pData, pConfiguration->vecRemoveScores);
	TakeUshort(pData, pConfiguration->droppedScore);
	TakeUchar(pData, pConfiguration->maxLevel);
	TakeFloatArray(pData, pConfiguration->vecScoreGainRate);
	TakeUshortArray(pData, pConfiguration->vecLevelScore);
	TakeUshortArray(pData, pConfiguration->vecStepDownTimespan);
	TakeUshort(pData, pConfiguration->dropTimespan);
	TakeBool(pData, pConfiguration->dropImmediate);
	TakeUshort(pData, pConfiguration->removeBlinkTimespan);
	TakeUchar(pData, pConfiguration->removeBlinkCount);
	TakeUshort(pData, pConfiguration->rollTimespan);
	TakeUshort(pData, pConfiguration->resumeDelayTimespan);

	return true;
}

void RecordParameters::Set(Configuration* pConfiguration)
{
	this->pConfiguration = pConfiguration;
}
