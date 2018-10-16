#pragma once

#include <string>
#include <memory>
#include <vector>

#include "UtilEnums.h"

class Exit;


class Room
{
public:
	Room(std::string _name) : name(_name), description(""), exits() {}
	Room(std::string _name, std::string desc) : name(_name), description(desc), exits() {}

	std::string GetName();
	std::string GetDescription() { return description; }
	std::shared_ptr<Room> CheckExit(ExitDirections dir);
	void AddExit(std::shared_ptr<Exit> newExit);

private:
	std::string name;
	std::string description;
	std::vector<std::shared_ptr<Exit>> exits;
};