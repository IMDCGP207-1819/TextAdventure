#include "Game.h"

#include <string>
#include <iostream>
#include <memory>

#include "Room.h"
#include "Exit.h"

void Game::Run()
{
	Init();

	while (true)
	{
		std::string input; 
		std::getline(std::cin, input);

		if (input == "north" || input == "south" || input == "east" || input == "west")
		{
			ExitDirections dir = ExitDirections::South;
			if ( input == "north")
				dir = ExitDirections::North;
			else if (input == "east")
				dir = ExitDirections::East;
			else if (input == "west")
				dir = ExitDirections::West;

			std::shared_ptr<Room> newRoom = CurrentRoom->CheckExit(dir);

			if (newRoom != nullptr)
			{
				CurrentRoom = newRoom;
				std::cout << CurrentRoom->GetName() << "\n";
			}
		}

		if (input == "look")
		{
			std::cout << CurrentRoom->GetName() << "\n";
		}

		if (input == "quit")
			break;
	}
}

void Game::Init()
{
	CurrentRoom = std::make_shared<Room>("starting room");

	CurrentRoom->AddExit(std::make_shared<Exit>(ExitDirections::North, std::make_shared<Room>("other room", "it's a different room from the starting room")));
}
