#include "BWAPI.h"
#include "bot/ABCDsquad.hpp"
#include "behaviors/moverelative.hpp"
#include "common/PUnit.hpp"
#include "search/gamestate.hpp"
#include "search/actions/action.hpp"
#include "search/searchers/searcherabcd.hpp"
#include "search/actionlisters/branchonplayer.hpp"
#include <exception>
#include <vector>
#include <set>

using namespace BWAPI;
using namespace Bot::Search;

void ABCDsquad::onFrame()
{
	static Searcher* searchAlgorithm = new SearcherABCD();
	static BranchOnPlayer* possibleActions = new BranchOnPlayer();

	units.remove_if([](PUnit* unit){ return !unit->exists(); });
	
	std::vector<BWAPI::Unit> playerUnits;
	std::vector<BWAPI::Unit> enemyUnits;

	for each(PUnit* unit in units)
	{
		playerUnits.push_back(unit->unit);
		for each (BWAPI::Unit enemyUnit in unit->unit->getUnitsInRadius(radius, BWAPI::Filter::IsEnemy))
		{
			if (std::find(enemyUnits.begin(), enemyUnits.end(), enemyUnit) == enemyUnits.end())
				enemyUnits.push_back(enemyUnit);
		}
	}

	GameState state(playerUnits, enemyUnits);


	if (!state.isTerminal())
	{
		try {
			std::list<Action*> actions = searchAlgorithm->search(&state, possibleActions);

			for each (Action* action in actions)
			{
				if (action->isPlayerAction(&state))
					action->executeOrder(&state);
			}
		} catch(const std::runtime_error& e) {
			throw;
		} catch(const std::exception& e) {
			throw;
		} catch(...) {
			abort();
		}
	}
}
