#include <iostream>

#include "../include/Bitboard.hpp"

int main();

int main()
{
	Bitboard bitboard = Bitboard();

	std::cout << "Bitboard value: " << bitboard.getValue() << std::endl;

	return 0;
}