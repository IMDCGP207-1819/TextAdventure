#pragma once

#include <string>

class Item
{
public:
	Item(int _id, std::string _name, std::string desc) : id(_id), name(_name), description(desc) {}

	const std::string GetName() const;

private:
	int id;
	std::string name;
	std::string description;
};

