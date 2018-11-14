#pragma once
#include <string>
#include <memory>

#include "Room.h"
#include "UtilEnums.h"	

struct TempExit {
	std::string dir;
	int toId;
	int fromId;
};

/// Exit Class - contains a direction and a room to travel to.
class Exit
{
public:
	/// Exit constructor - always require a direction and room shared_ptr
	Exit(ExitDirections dir, std::shared_ptr<Room> exitRoom) : direction(dir), roomToExitTo(exitRoom) {}

	/// HasDirection - checks a given direction against that stored in the exit
	bool HasDirection(ExitDirections dir) { return dir == direction; }

	/// GetRoom - returns the shared_ptr to the exits room
	std::shared_ptr<Room> GetRoom() { return roomToExitTo; }

	const std::string GetExitString() const;

private:
	ExitDirections direction;
	std::shared_ptr<Room> roomToExitTo;
};

