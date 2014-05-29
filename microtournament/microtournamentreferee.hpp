#pragma once
#include "microtournament/triggerunit.hpp"
#include "microtournament/playerunits.hpp"
#include <BWAPI.h>
#include <vector>
#include <array>

namespace microtournament
{
	class MicroTournamentReferee : public BWAPI::AIModule
	{
	public:
		static const std::string mapNamePrefix;
		static const std::vector<BWAPI::UnitType> spawnableTypes;

	public:
	  virtual void onStart();
	  virtual void onEnd(bool isWinner);
	  virtual void onFrame();
	  virtual void onSendText(std::string text);
	  virtual void onReceiveText(BWAPI::Player player, std::string text);
	  virtual void onPlayerLeft(BWAPI::Player player);
	  virtual void onNukeDetect(BWAPI::Position target);
	  virtual void onUnitDiscover(BWAPI::Unit unit);
	  virtual void onUnitEvade(BWAPI::Unit unit);
	  virtual void onUnitShow(BWAPI::Unit unit);
	  virtual void onUnitHide(BWAPI::Unit unit);
	  virtual void onUnitCreate(BWAPI::Unit unit);
	  virtual void onUnitDestroy(BWAPI::Unit unit);
	  virtual void onUnitMorph(BWAPI::Unit unit);
	  virtual void onUnitRenegade(BWAPI::Unit unit);
	  virtual void onSaveGame(std::string gameName);

	  void parseCommand(std::string command);

	private:
		bool enabled;
		int fieldMinX;
		int fieldMaxX;
		int fieldMinY;
		int fieldMaxY;
		std::map<BWAPI::UnitType, std::vector<TriggerUnit*>> unitTriggers;
		PlayerUnits players[2];
		TriggerUnit* giveTrigger;
		TriggerUnit* resetTrigger;
	};
}
