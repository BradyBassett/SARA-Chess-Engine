#include <iostream>
#include <string>
#include <sstream>
#include <chrono>

#include "../include/Game.hpp"
#include "../include/Utility.hpp"
#include "../include/MagicBitboards.hpp"

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

    std::cout << "Enter 'perft' to run a perft test or 'play' to play the game: ";
    std::string choice;
    std::getline(std::cin, choice);

    if (choice == "perft")
    {
        std::cout << "Enter depth for perft test: ";
        int depth;
        std::cin >> depth;

        std::map<std::string, int> output;
        auto start = std::chrono::high_resolution_clock::now();
        game.perftRoot(depth, output);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        std::cout << "Perft results:\n";
        for (const auto& entry : output)
        {
            std::cout << entry.first << ": " << entry.second << "\n";
        }
        std::cout << "Perft depth " << depth << " took " << elapsed.count() << " seconds." << std::endl;
    }
    else if (choice == "play")
    {
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

            Position fromPos = Utility::convertStringToPosition(from);
            Position toPos = Utility::convertStringToPosition(to);
            PromotionPiece promotionPiece = PromotionPiece::NONE;
            if (promotion == "q")
            {
                promotionPiece = PromotionPiece::QUEEN;
            }
            else if (promotion == "r")
            {
                promotionPiece = PromotionPiece::ROOK;
            }
            else if (promotion == "b")
            {
                promotionPiece = PromotionPiece::BISHOP;
            }
            else if (promotion == "n")
            {
                promotionPiece = PromotionPiece::KNIGHT;
            }

            try
            {
                std::system("clear");
                game.makeMove(fromPos, toPos, promotionPiece);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << "\n\n";
                continue;
            }
        }

        std::cout << game.getBoard().boardToAscii() << std::endl;
    }

    return 0;
}