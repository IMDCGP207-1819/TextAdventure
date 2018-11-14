#define _WIN32_WINNT 0x0500

#include <windows.h> 
#include <iostream>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

#include <nlohmann\json.hpp>

#include "Game.h"
#include "Room.h"
#include "Exit.h"

using json = nlohmann::json;

void clear() {
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}

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
		
		clear();

		// check the verb for the move command
		if (verb == "move") 
		{
			// check if we have a single noun provided
			if (results.size() != 2)
			{
				std::cout << "invalid command structure\n";
				continue;
			}
			// pass the noun off to the movement handler
			HandleMovement(results[1]);
			continue;
		}

		if (verb == "take") 
		{
			if (results.size() != 2) {
				std::cout << "invalid command structure\n";
				continue;
			}

			HandleItemTake(results[1]);
			continue;
		}

		if (verb == "drop") 
		{
			if (results.size() != 2) {
				std::cout << "invalid command structure\n";
				continue;
			}

			HandleDropItem(results[1]);
		}

		if (verb == "look")
		{
			if (results.size() == 2) {
				std::string item = results[1];
				std::transform(item.begin(), item.end(), item.begin(), ::tolower);
				DoItemLook(item);
			}
			else {
				DoRoomLook();
			}
			continue;
		}

		if (verb == "help") {
			PrintHelp();
		}

		if (verb == "quit")
			break;
	}
}

void Game::HandleItemTake(std::string item_name) 
{
	auto item = std::move(CurrentRoom->DropItem(item_name));

	if (item != nullptr) {
		std::cout << "You have picked up: " << item->GetName() << "\n";
		Player->AddItem(std::move(item));		
		return;
	}
	
	std::cout << "You cannot pickup that item here\n";
}

void Game::HandleDropItem(std::string item_name)
{
	auto item = Player->DropItem(item_name);

	if (item != nullptr)
	{
		std::cout << "You have dropped the " << item->GetName() << "\n";
		CurrentRoom->AddItem(std::move(item));
		return;
	}

	std::cout << "You are not carrying that item\n";
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
	LoadItemData();
}

void Game::LoadItemData()
{
	std::ifstream itemFile("items.json");
	json itemList;
	itemFile >> itemList;	

	std::vector<std::unique_ptr<Item>> items;

	for (auto it = itemList.begin(); it != itemList.end(); ++it) {
		json item = *it;
		items.push_back(std::make_unique<Item>(item["name"], item["description"], item["room_id"]));
	}

	for (auto &item : items) {
		if (item->GetTargetRoom() == 0) continue;

		int id = item->GetTargetRoom();

		auto room_find = std::find_if(rooms.begin(), rooms.end(), [id](const std::shared_ptr<Room> room) { return room->GetId() == id; });

		if (room_find != rooms.end()) {
			auto room = *room_find;
			room->AddItem(std::move(item));
		}
	}
}

void Game::LoadRoomData()
{
	std::ifstream roomFile("rooms.json");
	json roomData;
	roomFile >> roomData;

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
	DoRoomLook();

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
	else if (direction[0] == 'd')
		dir = ExitDirections::Down;
	else if (direction[0] == 'u')
		dir = ExitDirections::Up;

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

	if (CurrentRoom->HasItems()) {
		std::cout << "The room contains the following items:\n";
		for (auto &item : CurrentRoom->GetInventory()) {
			std::cout << item->GetName() << "\n";
		}
	}
}

bool Game::SearchInventory(std::string item_name, const std::vector<std::unique_ptr<Item>> &inventory) 
{
	// check if the item exists in the provided inventory.
	auto item_find = std::find_if(inventory.begin(), inventory.end(), [item_name](const std::unique_ptr<Item> &item) {
		std::string itemName = item->GetName();
		std::transform(itemName.begin(), itemName.end(), itemName.begin(), ::tolower);
		return itemName == item_name;
	});

	// the item exists in the players inventory, so we can print the description and bail.
	if (item_find != inventory.end()) {
		auto item_ptr = &*item_find;
		std::cout << item_ptr->get()->GetDescription() << "\n";
		return true;
	}

	return false;
}

void Game::DoItemLook(std::string item_name)
{
	auto &player_items = Player->GetInventory();
	bool found = SearchInventory(item_name, player_items);	

	if (found) return;
	
	auto &room_items = CurrentRoom->GetInventory();
	SearchInventory(item_name, room_items);
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
