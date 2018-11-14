#pragma once

#include <string>
#include <memory>
#include <vector>

#include "UtilEnums.h"
#include "Item.h"

/* forward declaration of Exit class - allows us to not include Exit.h in this file
   the issue here is that Exit needs to know about Room and Room needs to know about Exit.
   This is something called a "circular dependency" and is generally frowned upon*/
class Exit;

/// Room has a name, description, vector of exits and vector of items
class Room
{
public:
	/// Room constructor with default description and empty exits and item vectors
	Room(int _id, std::string _name) : id(_id), name(_name), description(""), exits() {}
	/// Room constructor with default exit and item vectors
	Room(int _id, std::string _name, std::string desc) : id(_id), name(_name), description(desc), exits() {}

	/// GetName - returns the room name
	std::string GetName();

	/// GetDescription - returns the room description
	std::string GetDescription() { return description; }

	/// GetRoomId - returns the room id number
	int GetRoomId() { return id; }

	/// CheckExit - searches the exit vector for an exit associated with the given direction
	std::shared_ptr<Room> CheckExit(ExitDirections dir);

	/// AddExit - adds an exit to the exit vector
	void AddExit(std::shared_ptr<Exit> newExit);

	void AddItem(std::unique_ptr<Item> newItem);

	std::unique_ptr<Item> DropItem(std::string item);

	bool HasItems() { return inventory.size() > 0; }
	const std::vector<std::unique_ptr<Item>> &GetInventory() const { return inventory; }
	const int GetId() const { return id; }

private:
	int id;
	std::string name;
	std::string description;
	std::vector<std::shared_ptr<Exit>> exits;
	std::vector<std::unique_ptr<Item>> inventory;
};