#pragma once
#include <memory>

#include "Room.h"

class Game 
{
public:
	void Run();
	
private:
	void Init();
	std::shared_ptr<Room> CurrentRoom;
};