#include "microtournament/guidedunit.hpp"

using namespace BWAPI;

GuidedUnit::GuidedUnit(BWAPI::Unit unit, Position goalPosition, double direction)
	: unit(unit)
	, goalPosition(goalPosition)
	, goalDirection(goalDirection)
{}

void GuidedUnit::onFrame()
{
	if (unit->getPosition() == goalPosition && unit->getAngle() == goalDirection)
		unit->move(goalPosition);
	else
	{
		Position preGoalPosition = goalPosition + Position((int)(std::cos(-goalDirection)*1.9), (int)(std::sin(-goalDirection)*1.9));
		if (unit->getPosition() != preGoalPosition)
			unit->move(preGoalPosition);
		else
			unit->move(goalPosition);
	}
}
