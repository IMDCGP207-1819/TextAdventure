#include <iostream>

#include "Game.h"

int main()
{
	std::cout << "Welcome to the wonderful world of adventure\n";

	// instantiate an instance of Game on the stack
	Game game;

	// run the game loop
	game.Run();

	return 0;
}