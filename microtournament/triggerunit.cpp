#include "microtournament/triggerunit.hpp"

using namespace BWAPI;

TriggerUnit::TriggerUnit(BWAPI::Unit unit)
	: unit(unit)
	, enablePosition(Position(TilePosition(unit->getPosition())) + Position(TilePosition(1,1))/2)
	, disablePosition(Position(TilePosition(unit->getPosition())) + Position(TilePosition(1, -1)) / 2)
	, disableOnFrame(0)
{}

void TriggerUnit::onFrame()
{
	bool enable = Broodwar->getFrameCount() > disableOnFrame;

	if (enable)
	{
		if (unit->getPosition() != enablePosition)
			unit->move(enablePosition);
	}
	else
	{
		if (unit->getPosition() != disablePosition)
			unit->move(disablePosition);
	}
}

void TriggerUnit::enable(int frames)
{
	disableOnFrame = Broodwar->getFrameCount() + frames;
}
