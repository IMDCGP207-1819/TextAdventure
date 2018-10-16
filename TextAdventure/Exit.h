#pragma once
#include <string>
#include <memory>
#include "Room.h"

#include "UtilEnums.h"	

class Exit
{
public:
	Exit(ExitDirections dir, std::shared_ptr<Room> exitRoom) : direction(dir), roomToExitTo(exitRoom) {}

	bool HasDirection(ExitDirections dir) { return dir == direction; }
	std::shared_ptr<Room> GetRoom() { return roomToExitTo; }

private:
	ExitDirections direction;
	std::shared_ptr<Room> roomToExitTo;
};

