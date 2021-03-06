#include "behaviors/AttackClosestLethal.hpp"
#include "common/GameUnit.hpp"
#include "common/PUnit.hpp"

using namespace BWAPI;

AttackClosestLethal::AttackClosestLethal()
{
}

void AttackClosestLethal::init(void* agent)
{
	first = true;
	PUnit* pUnit = (PUnit*)agent;
	pUnit->attackClosestEnemyNonWorried();
}

BEHAVIOR_STATUS AttackClosestLethal::execute(void* agent)
{
	PUnit* pUnit = (PUnit*)agent;
	BWAPI::Unit unit = pUnit->unit;

	static bool hasStartAttack = false;

	if(hasStartAttack)
	{
		if(pUnit->hasTarget)
			Broodwar->drawLineMap(pUnit->getPosition(),pUnit->target.getUnit()->getPosition(),Color(255,0,0));
		
		if(!pUnit->target.getUnit()->exists()) //|| !pUnit->isAttacking())
			return BT_SUCCESS;
		
		return BT_RUNNING;
	}
	else
	{
		if(pUnit->isAttacking())
		{
			hasStartAttack = true;
		}

		if(!hasStartAttack)
		{
			pUnit->attackClosestEnemyNonWorried();
		}
		
		if(pUnit->hasTarget)
			Broodwar->drawLineMap(pUnit->getPosition(),pUnit->target.getUnit()->getPosition(),Color(255,0,0));
		
		return BT_RUNNING;
	}
}
