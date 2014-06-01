#pragma once
#include <BWAPI.h>
#include <vector>
#include <list>
#include <map>

typedef int id_t;

namespace Bot { namespace Search
{
	class GameState;
	class Action;

	class Unit
	{
	public:
		const id_t id;
		BWAPI::Position pos;
		int hp;
		bool isMoving;
		bool isAttackFrame;
		bool groundWeaponCooldown;

	public:
		bool isAlive() const { return hp > 0; }

		virtual std::list<Action*> possibleActions(const GameState* state) const = 0;
		virtual Unit* clone() const = 0;

	protected:
		Unit(GameState* state, BWAPI::Unit bwapiUnit, id_t id);

	public:
		static Unit* create(GameState* state, BWAPI::Unit bwapiUnit, id_t id);
	};
}}
