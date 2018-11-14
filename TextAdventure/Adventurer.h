#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Item.h"

class Adventurer {
public:
	Adventurer() {}

	const std::vector<std::unique_ptr<Item>>& GetInventory() const { return inventory; }
	bool HasItem(std::string itemName);
	void AddItem(std::unique_ptr<Item> item);
	std::unique_ptr<Item> DropItem(std::string itemName);

private:
	std::vector<std::unique_ptr<Item>> inventory;
};