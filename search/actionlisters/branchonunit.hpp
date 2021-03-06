#pragma once
#include "search/actionlisters/actionlister.hpp"
#include "search/units/unit.hpp"

namespace Bot { namespace Search
{
	class BranchOnUnit : public ActionLister
	{
	public:
		virtual std::list<Action*> actions(const GameState* gamestate) const
		{
			const std::vector<const Unit*>& units = gamestate->getUnits();
			for each (const Unit* unit in units)
			{
				if (unit->isAlive())
				{
					std::list<Action*> actions = unit->possibleActions(gamestate);

					/*if (actions.size() > 2)
					{
						std::list<Action*>::iterator element2 = actions.begin();
						std::advance(element2, 2);
						actions.erase(element2, actions.end());
					}*/

					if (!actions.empty())
						return actions;
				}
			}

			return std::list<Action*>();
		}
	};
}}
