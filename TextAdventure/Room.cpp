#include "Exit.h"

std::string Room::GetName()
{
	return name;
}

std::shared_ptr<Room> Room::CheckExit(ExitDirections dir)
{
	// use a range-based for loop to work through all values in the exits vector
	for (auto exit : exits)
	{
		if (exit->HasDirection(dir))
			return exit->GetRoom();
	}

	return nullptr;
}

void Room::AddExit(std::shared_ptr<Exit> newExit)
{
	exits.push_back(newExit);
}
