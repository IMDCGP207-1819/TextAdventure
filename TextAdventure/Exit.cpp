#include "Exit.h"

const std::string Exit::GetExitString() const
{
	switch (direction) {
	default:
	case ExitDirections::North:
		return "North";
	case ExitDirections::South:
		return "South";
	case ExitDirections::East:
		return "East";
	case ExitDirections::West:
		return "West";
	case ExitDirections::Up:
		return "Up";
	case ExitDirections::Down:
		return "Down";
	}
}
