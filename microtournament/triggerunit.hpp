#pragma once
#include <BWAPI.h>

class TriggerUnit
{
public:
	static const int triggerEnableFrames = 80;

private:
	int disableOnFrame;
	BWAPI::Position enablePosition;
	BWAPI::Position disablePosition;
public:
	const BWAPI::Unit unit;
	const BWAPI::TilePosition origin;
public:
	TriggerUnit(BWAPI::Unit unit);

	void enable(int frames = triggerEnableFrames);
	void onFrame();
};
