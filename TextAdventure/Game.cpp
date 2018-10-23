#include "Game.h"

#include <string>
#include <iostream>
#include <sstream>
#include <iterator>

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

		// split the input string at every space ' ' found, storing the resulting strings into a vector
		std::istringstream iss(input);
		std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
			std::istream_iterator<std::string>());

		// TODO: refine input handling
		std::string verb = results[0];

		// check the verb for the move command
		if (verb == "move") {

			// check if we have a single noun provided
			if (results.size() != 2)
			{
				std::cout << "invalid command structure" << "\n";
				continue;
			}
			// pass the noun off to the movement handler
			HandleMovement(results[1]);
		}

		if (verb == "look")
		{
			// TODO: break look up into looking at the room 'look' and looking at thing 'look <item>'
			if (results.size() == 2) {

			}
			else {
				DoRoomLook();
			}
			
		}

		if (verb == "quit")
			break;
	}
}

void Game::Init()
{
	// build the player object, only Game cares about it, so it can be unique.
	Player = std::make_unique<Adventurer>();

	// we use shared pointers to save us worrying about tidying up memory as we go.
	CurrentRoom = std::make_shared<Room>("starting room");

	// which does mean that code like this is a bit longer than might be nice, but the hassle it saves is worth it.
	CurrentRoom->AddExit(std::make_shared<Exit>(ExitDirections::North, std::make_shared<Room>("other room", "it's a different room from the starting room")));
}

bool Game::ChangeRoom(std::shared_ptr<Room> newRoom)
{
	if (newRoom == nullptr)
		return false;

	// move room if we have a new room to move to
	CurrentRoom = newRoom;
	std::cout << CurrentRoom->GetName() << "\n";

	return true;
}

void Game::HandleMovement(std::string direction)
{
	// generate an enum version of the direction
	ExitDirections dir = ExitDirections::South;
	if (direction[0] == 'n')
		dir = ExitDirections::North;
	else if (direction[0] == 'e')
		dir = ExitDirections::East;
	else if (direction[0] == 'w')
		dir = ExitDirections::West;

	// see if the current room has that direction as a valid exit
	if (!ChangeRoom(CurrentRoom->CheckExit(dir)))
	{
		std::cout << "You cannot move in that direction\n";
	}
}

void Game::DoRoomLook()
{
	std::cout << CurrentRoom->GetName() << "\n";
	std::cout << CurrentRoom->GetDescription() << "\n";
}
