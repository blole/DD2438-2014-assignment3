#pragma once
#include <lib/libbehavior/BehaviorTree.h>
#include <BWAPI.h>
#include "botSquad/SquadManager.hpp"

using namespace BehaviorTree;

class MoveSquadToPosition : public BehaviorTreeNode
{
public:
	MoveSquadToPosition();
	
	void init(void* agent);
	BEHAVIOR_STATUS execute(void* agent);
	
private:
	bool first;
	float radiusFormation;
};
