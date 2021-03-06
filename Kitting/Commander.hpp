#pragma once
#include "common/InfluenceMap.hpp"
#include "common/GameUnit.hpp"
#include "common/PUnit.hpp"
#include "common/OUnit.hpp"
#include <BWAPI.h>
#include <lib/libbehavior/BehaviorTree.h>
#include <set>


using namespace BehaviorTree;

class Commander
{
public:
	// Attributes
	std::set<PUnit*> pAllUnits;
	std::set<OUnit*> oAllUnits;
	InfluenceMap influenceMap;

	// Constructor
	Commander();
	// Methods
	void update();
	void destroyUnit(BWAPI::Unit unit);
	void init();
};
