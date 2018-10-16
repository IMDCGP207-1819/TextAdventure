#include "Game.h"

#include <string>
#include <iostream>
#include <memory>

#include "Room.h"
#include "Exit.h"

void Game::Run()
{
	// run all setup
	Init();

	// loop until we get a suitable exit command from the user
	while (true)
	{
		std::string input; 
		std::getline(std::cin, input);


		// TODO: current input handling is nasty - needs fixing!

		// check for directions
		if (input == "north" || input == "south" || input == "east" || input == "west")
		{
			// generate an enum version of the direction
			ExitDirections dir = ExitDirections::South;
			if ( input == "north")
				dir = ExitDirections::North;
			else if (input == "east")
				dir = ExitDirections::East;
			else if (input == "west")
				dir = ExitDirections::West;

			// see if the current room has that direction as a valid exit
			std::shared_ptr<Room> newRoom = CurrentRoom->CheckExit(dir);

			// move room if we have a new room to move to
			if (newRoom != nullptr)
			{
				CurrentRoom = newRoom;
				std::cout << CurrentRoom->GetName() << "\n";
			}
			else 
			{
				std::cout << "You cannot go that way.\n";
			}
		}

		if (input == "look")
		{
			std::cout << CurrentRoom->GetName() << "\n";
			std::cout << CurrentRoom->GetDescription() << "\n";
		}

		if (input == "quit")
			break;
	}
}

void Game::Init()
{
	// we use shared pointers to save us worrying about tidying up memory as we go.
	CurrentRoom = std::make_shared<Room>("starting room");

	// which does mean that code like this is a bit longer than might be nice, but the hassle it saves is worth it.
	CurrentRoom->AddExit(std::make_shared<Exit>(ExitDirections::North, std::make_shared<Room>("other room", "it's a different room from the starting room")));
}
