#include <iostream>
#include <string>
#include <sstream>

#include "../include/Game.hpp"

int main();

int main()
{
	std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	std::cout << "Enter Fen (or press enter for default): ";
	std::string input;
	std::getline(std::cin, input);
	if (!input.empty())
	{
		fen = input;
	}

	Game game(fen);

	bool running = true;
	while (running)
	{
		std::cout << game.getBoard().boardToAscii() << std::endl;

		std::string line, from, to, promotion;

		std::cout << (game.getActiveColor() == Color::WHITE ? "White" : "Black") << " - Enter move: ";
		std::getline(std::cin, line);
		std::istringstream iss(line);
		iss >> from >> to >> promotion;

		if (from == "quit")
		{
			running = false;
			continue;
		}

		Position fromPos = game.getBoard().convertStringToPosition(from);
		Position toPos = game.getBoard().convertStringToPosition(to);

		try
		{
			std::system("clear");
			// game.makeMove(fromPos, toPos, promotion);
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << "\n\n";
			continue;
		}
	}

	std::cout << game.getBoard().boardToAscii() << std::endl;

	return 0;
}