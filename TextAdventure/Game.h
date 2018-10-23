#pragma once
#include <memory>

#include "Room.h"

///
/// Game class, contains core game loop
///
class Game 
{
public:
	/// Run - handles all user input and output
	void Run();
	
private:
	/// Init - loads data files and creates "physical" space.
	void Init();

	bool ChangeRoom(std::shared_ptr<Room> NewRoom);
	void HandleMovement(std::string direction);

	std::shared_ptr<Room> CurrentRoom;
};