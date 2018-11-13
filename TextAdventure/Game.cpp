#include "Game.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

#include "Room.h"
#include "Exit.h"

struct TempExit {
	std::string dir;
	int toId;
	int fromId;
};

void Game::Run()
{
	// run all setup
	Init();

	// loop until we get a suitable exit command from the user
	while (true)
	{
		std::string input; 
		std::getline(std::cin, input);
		std::string verb = "invalid";
		std::vector<std::string> results;
		
		if (input.length() > 0) {

			// split the input string at every space ' ' found, storing the resulting strings into a vector
			// this is making use of the stringstream defaults.
			std::istringstream iss(input);
			results = std::vector<std::string>((std::istream_iterator<std::string>(iss)),
				std::istream_iterator<std::string>());

			// TODO: refine input handling
			verb = StandardiseCommandInput(results[0]);
		}

		if (verb == "invalid") {
			std::cout << "invalid command, type 'help' for a list of valid commands\n";
			continue;
		}

		// check the verb for the move command
		if (verb == "move") {

			// check if we have a single noun provided
			if (results.size() != 2)
			{
				std::cout << "invalid command structure" << "\n";
				continue;
			}
			// pass the noun off to the movement handler
			HandleMovement(results[1]);
		}

		if (verb == "look")
		{
			// TODO: break look up into looking at the room 'look' and looking at thing 'look <item>'
			if (results.size() == 2) {

			}
			else {
				DoRoomLook();
			}
			
		}

		if (verb == "help") {
			PrintHelp();
		}

		if (verb == "quit")
			break;
	}
}

void Game::PrintHelp() {
	// we loop over the command map entries
	// and then over each of the vector of synonyms to see if we have a match
	std::cout << "\nThe following commands are available:\n";
	for (auto it = commandMap.begin(); it != commandMap.end(); ++it) {
		std::cout << it->first << ": " << it->second->description << "\n";
	}
}

void Game::Init()
{
	LoadConfig();

	// build the player object, only Game cares about it, so it can be unique.
	Player = std::make_unique<Adventurer>();

	LoadRoomData();
}

void Game::LoadRoomData()
{
	std::ifstream roomFile("rooms.json");
	json roomData;
	roomFile >> roomData;

	std::vector<std::shared_ptr<Room>> rooms;
	std::vector<TempExit> exits;

	for (auto it = roomData.begin(); it != roomData.end(); ++it) {
		json room = *it;
		auto newRoom = std::make_shared<Room>(room["id"], room["name"], room["description"]);
		rooms.push_back(newRoom);
		
		json exitJson = room["exits"];

		for (auto _exit : exitJson) {
			for (auto _it = _exit.begin(); _it != _exit.end(); ++_it) {
				TempExit exit;
				json exitInfo = *_it;
				exit.dir = _it.key();
				exit.toId = exitInfo.get<int>();
				exit.fromId = room["id"];
				exits.push_back(exit);
			}		
		}

		if (room["starting"])
			CurrentRoom = newRoom;
	}

	for (auto exit : exits) {
		std::shared_ptr<Room> startRoom;
		std::shared_ptr<Room> targetRoom;

		for (auto room : rooms) {
			if (room->GetRoomId() == exit.fromId)
				startRoom = room;
			if (room->GetRoomId() == exit.toId)
				targetRoom = room;
		}

		if (startRoom && targetRoom) {
			startRoom->AddExit(std::make_shared<Exit>(GetDirectionFromString(exit.dir), targetRoom));
		}
	}
}

void Game::LoadConfig()
{
	// load the config json file and parse the file into a json object
	std::ifstream configFile("config.json");
	json config;
	configFile >> config;

	// iterate over the json object and extract the list of commands and synonyms.
	for (auto it = config.begin(); it != config.end(); ++it) {
		json command = *it;
		auto commandSt = std::unique_ptr<CommandInfo>(new CommandInfo());
		commandSt->description = command["description"].get<std::string>();

		// each command has an array of synonyms, which need to be iterated over to extract the values
		for (auto com = command["synonyms"].begin(); com != command["synonyms"].end(); ++com) {
			commandSt->synonyms.push_back(com.value().get<std::string>());
		}

		commandMap[it.key()] = std::move(commandSt);
	}
}

ExitDirections Game::GetDirectionFromString(std::string dir)
{
	if (dir == "north")
		return ExitDirections::North;
	if (dir == "south")
		return ExitDirections::South;
	if (dir == "east")
		return ExitDirections::East;
	if (dir == "west")
		return ExitDirections::West;
	if (dir == "up")
		return ExitDirections::Up;
	if (dir == "down")
		return ExitDirections::Down;

	return ExitDirections::North;
}

bool Game::ChangeRoom(std::shared_ptr<Room> newRoom)
{
	if (newRoom == nullptr)
		return false;

	// move room if we have a new room to move to
	CurrentRoom = newRoom;
	std::cout << CurrentRoom->GetName() << "\n";

	return true;
}

void Game::HandleMovement(std::string direction)
{
	// generate an enum version of the direction
	ExitDirections dir = ExitDirections::South;
	if (direction[0] == 'n')
		dir = ExitDirections::North;
	else if (direction[0] == 'e')
		dir = ExitDirections::East;
	else if (direction[0] == 'w')
		dir = ExitDirections::West;

	// see if the current room has that direction as a valid exit
	if (!ChangeRoom(CurrentRoom->CheckExit(dir)))
	{
		std::cout << "You cannot move in that direction\n";
	}
}

void Game::DoRoomLook()
{
	std::cout << CurrentRoom->GetName() << "\n";
	std::cout << CurrentRoom->GetDescription() << "\n";
}

std::string Game::StandardiseCommandInput(std::string command)
{
	controlMap::iterator it;
	
	// we loop over the command map entries
	// and then over each of the vector of synonyms to see if we have a match
	for (it = commandMap.begin(); it != commandMap.end(); ++it) {
		std::vector<std::string> words = it->second->synonyms;
		for (auto word : words) {
			if (command == word) {
				return it->first; // if we do, we return the main command word for that synomym
			}
		}
	}

	// otherwise, return invalid so we can easily dump out.
	return "invalid";
}
