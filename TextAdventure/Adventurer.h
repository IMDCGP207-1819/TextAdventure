#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Item.h"

class Adventurer {
public:
	Adventurer() {}

	bool HasItem(std::string itemName);
	std::shared_ptr<Item> DropItem(std::string itemName);

private:
	std::vector<std::shared_ptr<Item>> inventory;
};