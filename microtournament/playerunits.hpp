#pragma once
#include <BWAPI.h>
#include "microtournament/guidedunit.hpp"

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
	};
}
