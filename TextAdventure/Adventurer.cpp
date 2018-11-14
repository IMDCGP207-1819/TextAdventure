#include "Adventurer.h"

#include <algorithm>

/// HasItem takes a string and checks it against the items in the adventurers inventory
bool Adventurer::HasItem(std::string itemName) 
{
	// find_if searches a container (in this case, the inventory vector) and matches against either a single value or, as here, with a predicate
	// below, the predicate is a lambda function; it "captures" a reference to itemName and uses it to compare against each item in the inventory.
	return std::find_if(inventory.begin(), inventory.end(), [&itemName](std::unique_ptr<Item> &item) {return item->GetName() == itemName; }) == inventory.end();
}

void Adventurer::AddItem(std::unique_ptr<Item> item)
{
	inventory.push_back(std::move(item));
}

/// DropItem takes a string and returns either nullptr or the item from the adventurers inventory, in which case, it removes the item from the vector
/*std::unique_ptr<Item> Adventurer::DropItem(std::string itemName)
{
	// use the same style of find_if call to get the item from the inventory, if it exists.
	auto item = std::find_if(inventory.begin(), inventory.end(), [&itemName](std::unique_ptr<Item> &item) {return item->GetName() == itemName; });

	// if we got to the end of the vector, item will equal inventory.end() and we can return a nullptr - that item does not exist in our pockets
	if (item == inventory.end()) return nullptr;

	auto droppedItem = &*item;

	// and remove it from the inventory vector
	inventory.erase(item);

	// and return it.
	return std::move(droppedItem);
}*/
