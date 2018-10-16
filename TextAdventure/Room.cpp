#include "Exit.h"

std::string Room::GetName()
{
	return name;
}

std::shared_ptr<Room> Room::CheckExit(ExitDirections dir)
{
	for (size_t i = 0; i < exits.size(); i++)
	{
		if (exits.at(i)->HasDirection(dir))
			return exits.at(i)->GetRoom();
	}

	return nullptr;
}

void Room::AddExit(std::shared_ptr<Exit> newExit)
{
	exits.push_back(newExit);
}
