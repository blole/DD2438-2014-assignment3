#include <windows.h>
#include <algorithm>
#include <regex>
#include "microtournament/microtournamentreferee.hpp"
#include "common/rect.hpp"

using namespace BWAPI;
using namespace microtournament;

const std::vector<BWAPI::UnitType> createSpawnableTypes()
{
	std::vector<BWAPI::UnitType> spawnableTypes;
	spawnableTypes.push_back(BWAPI::UnitTypes::Terran_Marine);
	return spawnableTypes;
}

const std::vector<BWAPI::UnitType> MicroTournamentReferee::spawnableTypes(createSpawnableTypes());
const std::string MicroTournamentReferee::mapNamePrefix("microtournament");

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

std::string errorText;
int minUnitDistance = 20;

void MicroTournamentReferee::onStart()
{
	Broodwar->enableFlag(Flag::UserInput);

	if (!begins_with(Broodwar->mapFileName(), mapNamePrefix))
	{
		Broodwar << "Error!" << std::endl;
		errorText = "Micro Tournament referee module loaded, but not enabled, because map name didn't begin with '"+mapNamePrefix+"'";
		Broodwar << errorText <<std::endl;
		enabled = false;
	}
	else if (Broodwar->self()->getID() != 0)
	{
		Broodwar << "Error!" << std::endl;
		errorText = "Micro Tournament referee module loaded, but not enabled, because it was not loaded as first player";
		Broodwar << errorText << std::endl;
		enabled = false;
	}
	else
	{
		Broodwar << "Welcome to Micro Tournament!" << std::endl;
		Broodwar << "   write !help for help" << std::endl;
		enabled = true;

		Rect playingField;
		for each (Unit u in Broodwar->self()->getUnits())
		{
			if (u->getType() == UnitTypes::Powerup_Flag)
				playingField.enclose(u->getPosition());
			else if (u->getType() == UnitTypes::Terran_Civilian)
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

		players[0].startingRectangle = playingField;
		players[0].startingRectangle.width /= 2;

		players[1].startingRectangle = playingField;
		players[1].startingRectangle.width /= 2;
		players[1].startingRectangle.x += playingField.width/2;
	}
}

std::vector<Position> generatePositions(Rect bounds, unsigned int count, int minDistance)
{
	Rect shrunkBounds = bounds;
	shrunkBounds.shrink(minDistance);
	std::vector<Position> positions;
	
	while (positions.size() < count)
	{
		Position newPos(std::rand()%shrunkBounds.width, std::rand()%shrunkBounds.height);
		bool collides = false;

		for each (Position pos in positions)
		{
			if (newPos.getDistance(pos) < minDistance)
			{
				collides = true;
				break;
			}
		}

		if (!collides)
			positions.push_back(newPos);
	}

	for (unsigned int i=0; i<count; i++)
		positions[i] = positions[i] + Position(minDistance, minDistance);

	return positions;
}

void MicroTournamentReferee::onFrame()
{
	if (enabled)
	{
		//fighting units
		for each (PlayerUnits player in players)
		{
			for each (GuidedUnit unit in player.units)
				unit.onFrame();
		}

		//trigger units
		for each (std::pair<UnitType, std::vector<TriggerUnit*>> triggers in unitTriggers)
		{
			for each (TriggerUnit* unit in triggers.second)
				unit->onFrame();
		}
	}
}

void MicroTournamentReferee::parseCommand(std::string command)
{
	if (!enabled)
	{
		if (begins_with(command, "!"))
			Broodwar << errorText << std::endl;
	}
	else
	{
		if (begins_with(command, "!help"))
		{
			Broodwar << "TODO: help text here" << std::endl;
		}
		else if (begins_with(command, "!reset"))
		{
			for each (PlayerUnits player in players)
			{
				player.units.clear();
				player.currentUnitCount.clear();
				player.goalUnitCount.clear();
			}

			resetTrigger->enable();
		}
		else if (begins_with(command, "!fight"))
		{
			giveTrigger->enable();
		}
		else if (begins_with(command, "!spawn"))
		{
			if (!(players[0].units.empty() && players[1].units.empty()))
				Broodwar << "error: can only parse one !spawn command between !reset:s" << std::endl;
			else
			{
				std::vector<std::string> commands = explode(command);
				for (unsigned int i = 1; i < commands.size(); i++)
				{
					if (i + 3 > commands.size())
						break;

					int player, amount;
					try
					{
						player = std::stoi(commands[i++]);
						amount = std::stoi(commands[i++]);
					}
					catch (std::invalid_argument ex)
					{
						Broodwar << "spawn: error: players or unit count not integer" << std::endl;
						continue;
					}

					std::string typeString = commands[i++];
					UnitType type;

					if (begins_with(typeString, "mar"))
						type = UnitTypes::Terran_Marine;
					else
					{
						Broodwar << "spawn: error: unit type '" << typeString << "' not recognized" << std::endl;
						continue;
					}

					if (player == 0 || player == 1)
						players[player].goalUnitCount[type] += amount;
					else
					{
						Broodwar << "spawn: error: player '" << player << "' must be either 1 or 0" << std::endl;
						continue;
					}

					Broodwar << "spawn: creating " << amount << " " << type << (amount > 1 ? "s" : "") << " for player " << player << std::endl;
				}

				bool symmetric = true;
				for each (UnitType type in spawnableTypes)
				{
					if (players[0].goalUnitCount[type] != players[1].goalUnitCount[type])
						symmetric = false;

					int amount = players[0].goalUnitCount[type] + players[1].goalUnitCount[type];
					std::vector<TriggerUnit*> triggers = unitTriggers[type];

					for (unsigned int i = 0; i < triggers.size(); i++)
					{
						if ((amount & (1 << i)) != 0)
							triggers[i]->enable();
					}
				}

				if (symmetric)
				{
					std::vector<Position> positions = generatePositions(players[0].startingRectangle, players[0].goalUnitCountTotal(), minUnitDistance);
					players[0].goalPositions = positions;
					players[1].goalPositions = positions;
				}
				else
				{
					players[0].goalPositions = generatePositions(players[0].startingRectangle, players[0].goalUnitCountTotal(), minUnitDistance);
					players[1].goalPositions = generatePositions(players[1].startingRectangle, players[1].goalUnitCountTotal(), minUnitDistance);
				}
			}
		}
		else if (begins_with(command, "!"))
			Broodwar << "unknown command " << command << std::endl;
	}
}

void MicroTournamentReferee::onSendText(std::string text)
{
	Broodwar->sendText("%s", text.c_str());
	parseCommand(text);
}

void MicroTournamentReferee::onReceiveText(Player player, std::string text)
{
	parseCommand(text);
}

void MicroTournamentReferee::onEnd(bool isWinner)
{
}

void MicroTournamentReferee::onPlayerLeft(BWAPI::Player player)
{
}

void MicroTournamentReferee::onNukeDetect(BWAPI::Position target)
{
}

void MicroTournamentReferee::onUnitDiscover(BWAPI::Unit unit)
{
}

void MicroTournamentReferee::onUnitEvade(BWAPI::Unit unit)
{
}

void MicroTournamentReferee::onUnitShow(BWAPI::Unit unit)
{
}

void MicroTournamentReferee::onUnitHide(BWAPI::Unit unit)
{
}

void MicroTournamentReferee::onUnitCreate(BWAPI::Unit unit)
{
	if (enabled)
	{
		UnitType type = unit->getType();
		
		if (std::find(spawnableTypes.begin(), spawnableTypes.end(), type) == spawnableTypes.end())
			return;
		
		for each (PlayerUnits player in players)
		{
			Position topLeft = Position(player.startingRectangle.x, player.startingRectangle.y);
			if (player.currentUnitCount[type] < player.goalUnitCount[type])
			{
				//TODO: units of different types may not end up in symmetric positions!
				Position pos = player.goalPositions[player.units.size()] + topLeft;

				player.units.push_back(GuidedUnit(unit, pos, 0));
				return;
			}
		}
	}
}

void MicroTournamentReferee::onUnitDestroy(BWAPI::Unit unit)
{
}

void MicroTournamentReferee::onUnitMorph(BWAPI::Unit unit)
{
}

void MicroTournamentReferee::onUnitRenegade(BWAPI::Unit unit)
{
}

void MicroTournamentReferee::onSaveGame(std::string gameName)
{}
