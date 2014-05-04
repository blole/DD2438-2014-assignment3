#include <windows.h>
#include <algorithm>
#include <regex>
#include "microtournament/microtournament.hpp"

using namespace BWAPI;

template<class TContainer>
bool begins_with(const TContainer& input, const TContainer& match)
{
	return input.size() >= match.size()
		&& equal(match.begin(), match.end(), input.begin());
}
bool begins_with(const std::string& input, const char* match)
{
	return begins_with(input, std::string(match));
}

std::vector<std::string> explode(const std::string& input) {
	std::stringstream ss(input);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	return std::vector<std::string>(begin, end);
}

std::string mapNamePrefix = "microtournament";
std::string errorText = "Micro Tournament module loaded, but not enabled, because map name didn't begin with '" + mapNamePrefix + "'";

static std::vector<UnitType> types;

void MicroTournamentAI::onStart()
{
	ready = false;
	types.push_back(UnitTypes::Terran_Marine);

	if (!begins_with(Broodwar->mapFileName(), mapNamePrefix))
	{
		Broodwar << "Error!" << std::endl;
		Broodwar << errorText << std::endl;
		enabled = false;
	}
	else
	{
		Broodwar << "Welcome to Micro Tournament!" << std::endl;
		Broodwar << "   write !help for help" << std::endl;
		enabled = true;

		for each (Unit u in Broodwar->self()->getUnits())
		{
			if (u->getType() == UnitTypes::Special_Terran_Flag_Beacon)
			{
				fieldMinX = min(fieldMinX, u->getPosition().x);
				fieldMaxX = std::max(fieldMaxX, u->getPosition().x);
				fieldMinY = min(fieldMinY, u->getPosition().y);
				fieldMaxY = std::max(fieldMaxY, u->getPosition().y);
			}
			if (u->getType() == UnitTypes::Terran_Civilian)
				unitTriggers[UnitTypes::Terran_Marine].push_back(new TriggerUnit(u));
		}

		//sort ascendingly by pos.x
		std::sort(
			unitTriggers[UnitTypes::Terran_Marine].begin(),
			unitTriggers[UnitTypes::Terran_Marine].end(),
			[](TriggerUnit* a, TriggerUnit* b){ return a->unit->getPosition().x < b->unit->getPosition().x; });

		resetTrigger = unitTriggers[UnitTypes::Terran_Marine].back();
		unitTriggers[UnitTypes::Terran_Marine].pop_back();
		giveTrigger = unitTriggers[UnitTypes::Terran_Marine].back();
		unitTriggers[UnitTypes::Terran_Marine].pop_back();
	}
}

void MicroTournamentAI::onEnd(bool isWinner)
{
}

void MicroTournamentAI::onFrame()
{
	//fighters
	if (!ready)
	{
		for each (std::pair<BWAPI::UnitType, std::vector<BWAPI::Unit>> unitGroup in player1Units)
		{
			Unitset unitset;
			for each (Unit unit in unitGroup.second)
				unitset.push_back(unit);

			unitset.move(Position(fieldMinX + (fieldMaxX - fieldMinX) / 2, fieldMinY + (fieldMaxY - fieldMinY) / 4));
		}
		for each (std::pair<BWAPI::UnitType, std::vector<BWAPI::Unit>> unitGroup in player2Units)
		{
			Unitset unitset;
			for each (Unit unit in unitGroup.second)
				unitset.push_back(unit);

			unitset.move(Position(fieldMinX + (fieldMaxX - fieldMinX) / 2, fieldMinY + (fieldMaxY - fieldMinY) *3 / 4));
		}
	}

	//trigger units
	for each (std::pair<UnitType, std::vector<TriggerUnit*>> triggers in unitTriggers)
	{
		for each (TriggerUnit* u in triggers.second)
			u->onFrame();
	}
}

void MicroTournamentAI::onSendText(std::string text)
{
	onReceiveText(Broodwar->self(), text);
}

void MicroTournamentAI::onReceiveText(Player player, std::string text)
{
	if (enabled && Broodwar->self()->getID() == 0)
	{
		if (begins_with(text, "!help"))
		{
			Broodwar << "help text here" << std::endl;
		}
		else if (begins_with(text, "!reset"))
		{
			ready = false;
			resetTrigger->enable();
			player1UnitCount.clear();
			player2UnitCount.clear();
			player1Units.clear();
			player2Units.clear();
		}
		else if (begins_with(text, "!fight"))
		{
			ready = false;
		}
		else if (begins_with(text, "!spawn"))
		{
			if (!player1UnitCount.empty() || !player2UnitCount.empty())
				Broodwar << "error: can only parse one !spawn command between !reset:s" << std::endl;
			else
			{
				try
				{
					std::vector<std::string> commands = explode(text);
					for (unsigned int i = 1; i < commands.size(); i++)
					{
						if (i + 3 > commands.size())
							break;

						int player = std::stoi(commands[i++]);
						int amount = std::stoi(commands[i++]);
						std::string typeString = commands[i++];
						UnitType type;

						if (begins_with(typeString, "mar"))
							type = UnitTypes::Terran_Marine;

						if (player == 0)
							player1UnitCount[type] = amount;
						else if (player == 1)
							player2UnitCount[type] = amount;
						else
						{
							Broodwar << "error " << player << " must be either 1 or 0" << std::endl;
							return;
						}

						Broodwar << "spawning " << amount << " " << type << (amount > 1 ? "s" : "") << std::endl;
					}
					
					for each (UnitType type in types)
					{
						int amount = player1UnitCount[type] + player2UnitCount[type];
						std::vector<TriggerUnit*> triggers = unitTriggers[type];

						for (unsigned int i = 0; i < triggers.size(); i++)
						{
							if ((amount & (1 << i)) != 0)
								triggers[i]->enable();
						}
					}
				}
				catch (std::invalid_argument ex)
				{
					Broodwar << "error parsing !spawn command, players and unit count need to be given as integers." << std::endl;
					return;
				}
			}
		}
	}
	else
	{
		if (begins_with(text, "!"))
		{
			Broodwar << errorText << std::endl;
		}
	}
}

void MicroTournamentAI::onPlayerLeft(BWAPI::Player player)
{
}

void MicroTournamentAI::onNukeDetect(BWAPI::Position target)
{
}

void MicroTournamentAI::onUnitDiscover(BWAPI::Unit unit)
{
}

void MicroTournamentAI::onUnitEvade(BWAPI::Unit unit)
{
}

void MicroTournamentAI::onUnitShow(BWAPI::Unit unit)
{
}

void MicroTournamentAI::onUnitHide(BWAPI::Unit unit)
{
}

void MicroTournamentAI::onUnitCreate(BWAPI::Unit unit)
{
	if (enabled && Broodwar->self()->getID() == 0)
	{
		UnitType type = unit->getType();

		if (std::find(types.begin(), types.end(), type) == types.end())
			return;

		if (player2Units[type].size() < player2UnitCount[type])
			player2Units[type].push_back(unit);
		else
			player1Units[type].push_back(unit);
	}
}

void MicroTournamentAI::onUnitDestroy(BWAPI::Unit unit)
{
}

void MicroTournamentAI::onUnitMorph(BWAPI::Unit unit)
{
}

void MicroTournamentAI::onUnitRenegade(BWAPI::Unit unit)
{
}

void MicroTournamentAI::onSaveGame(std::string gameName)
{}

bool MicroTournamentModule::onAction(BWAPI::Tournament::ActionID actionType, void *parameter)
{
  return true;
}

void MicroTournamentModule::onFirstAdvertisement()
{
}
