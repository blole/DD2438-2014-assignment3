#pragma once
#include <BWAPI.h>

class GuidedUnit
{
private:
	BWAPI::Position goalPosition;
	double goalDirection;
public:
	/*const*/ BWAPI::Unit unit;
public:
	GuidedUnit(BWAPI::Unit unit, BWAPI::Position goalPosition, double goalDirection);

	void onFrame();
};
