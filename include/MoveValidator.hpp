#ifndef MOVEVALIDATOR_HPP
#define MOVEVALIDATOR_HPP

#include "Game.hpp"

class MoveValidator
{
public:
	static void validateMove(Position from, Position to, PieceType piece, Color color, Game &game);

private:
	static bool isPathClear(Position from, Position to, PieceType piece, Color color, Game &game);
	static Bitboard calculatePath(Position from, Position to, PieceType piece, Color color);
	static Bitboard calculatePawnPath(Position from, Position to, Color color);
	static Bitboard calculateDiagonalPath(Position from, Position to);
	static Bitboard calculateOrthaogonalPath(Position from, Position to);
	static bool isDiagonal(Position from, Position to);
	static bool isOrthogonal(Position from, Position to);

	static void validatePawnMove(Position from, Position to, PieceType piece, Color color, Game &game);
	static void validateKingMove(Position from, Position to, PieceType piece, Color color, Game &game);
	static void validateCastlingMove(Position from, Position to, Color color, Game &game);
	static bool isValidRookPosition(Position from, Color color, Game &game);

	static bool isSquareAttacked(Position position, Color color, Game &game);
};

#endif // MOVEVALIDATOR_HPP