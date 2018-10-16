#pragma once
#include <memory>

#include "Room.h"

/// Game class, contains core game loop
class Game 
{
public:
	/// Run - handles all user input and output
	void Run();
	
private:
	/// Init - loads data files and creates "physical" space.
	void Init();
	std::shared_ptr<Room> CurrentRoom;
};