#ifndef MOVEVALIDATOR_HPP
#define MOVEVALIDATOR_HPP

#include "Board.hpp"
#include "Move.hpp"

class MoveValidator
{
public:
	static void validateMove();

private:
	static bool isPathClear();
	static void validatePawnMove();
	static void validateKingMove();
};

#endif // MOVEVALIDATOR_HPP