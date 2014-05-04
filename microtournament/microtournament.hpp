#pragma once
#include "microtournament/triggerunit.hpp"
#include <BWAPI.h>
#include <vector>

class MicroTournamentModule : public BWAPI::TournamentModule
{
  virtual bool onAction(BWAPI::Tournament::ActionID actionType, void *parameter = nullptr);
  virtual void onFirstAdvertisement();
};

class MicroTournamentAI : public BWAPI::AIModule
{
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

private:
	bool enabled;
	bool ready;
	int fieldMinX;
	int fieldMaxX;
	int fieldMinY;
	int fieldMaxY;
	std::map<BWAPI::UnitType, std::vector<TriggerUnit*>> unitTriggers;
	std::map<BWAPI::UnitType, unsigned int> player1UnitCount;
	std::map<BWAPI::UnitType, unsigned int> player2UnitCount;
	std::map<BWAPI::UnitType, std::vector<BWAPI::Unit>> player1Units;
	std::map<BWAPI::UnitType, std::vector<BWAPI::Unit>> player2Units;
	TriggerUnit* giveTrigger;
	TriggerUnit* resetTrigger;
};
