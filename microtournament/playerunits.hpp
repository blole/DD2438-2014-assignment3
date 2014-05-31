#pragma once
#include <BWAPI.h>
#include "microtournament/guidedunit.hpp"
#include "common/rect.hpp"

namespace microtournament
{
	class PlayerUnits
	{
	public:
		std::vector<GuidedUnit> units;
		std::map<BWAPI::UnitType, std::vector<BWAPI::Unit>> player1Units;
		std::map<BWAPI::UnitType, std::vector<BWAPI::Unit>> player2Units;
		std::map<BWAPI::UnitType, unsigned int> goalUnitCount;
		std::map<BWAPI::UnitType, unsigned int> currentUnitCount;

		Rect startingRectangle;

		std::vector<BWAPI::Position> goalPositions;

		int goalUnitCountTotal()
		{
			int total = 0;
			for each (std::pair<BWAPI::UnitType, unsigned int> pair in goalUnitCount)
				total += pair.second;

			return total;
		}
	};
}
