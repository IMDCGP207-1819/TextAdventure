#pragma once

#include <string>
#include <memory>
#include <vector>

#include "UtilEnums.h"

/* forward declaration of Exit class - allows us to not include Exit.h in this file
   this is something known as a "circular dependency". */
class Exit;

/// Room has a name, description, vector of exits and vector of items
class Room
{
public:
	/// Room constructor with default description and empty exits and item vectors
	Room(std::string _name) : name(_name), description(""), exits() {}
	/// Room constructor with default exit and item vectors
	Room(std::string _name, std::string desc) : name(_name), description(desc), exits() {}

	/// GetName - returns the room name
	std::string GetName();

	/// GetDescription - returns the room description
	std::string GetDescription() { return description; }

	/// CheckExit - searches the exit vector for an exit associated with the given direction
	std::shared_ptr<Room> CheckExit(ExitDirections dir);

	/// AddExit - adds an exit to the exit vector
	void AddExit(std::shared_ptr<Exit> newExit);

private:
	std::string name;
	std::string description;
	std::vector<std::shared_ptr<Exit>> exits;
};