#include "Exit.h"

#include <algorithm>

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

void Room::AddItem(std::unique_ptr<Item> newItem)
{
	inventory.push_back(std::move(newItem));
}

std::unique_ptr<Item> Room::DropItem(std::string item)
{
	std::transform(item.begin(), item.end(), item.begin(), ::tolower);

	auto room_find = std::find_if(inventory.begin(), inventory.end(), [item](const std::unique_ptr<Item> &inv_item) { 
		std::string itemName = inv_item->GetName();
		std::transform(itemName.begin(), itemName.end(), itemName.begin(), ::tolower);
		return itemName == item;
	});

	if ( room_find == inventory.end())
		return nullptr;

	return std::move(*room_find);
}
