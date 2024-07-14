#ifndef MOVEVALIDATOR_HPP
#define MOVEVALIDATOR_HPP

#include "Game.hpp"

class MoveValidator
{
public:
	static void validateMove(Position from, Position to, PieceType piece, Color color, Game &game);
	static bool calculateInCheck();

private:
	static Bitboard generatePotentialMoves(Position position, PieceType piece, Color color, Board &board);
	static Bitboard generatePotentialPawnMoves(Color color, Position position, Bitboard moves);
	static bool canDoublePawnPush(Color color, Position position);
	static Bitboard generatePotentialKingMoves(Color color, Position position, Bitboard moves);
	static bool isPathClear(Position from, Position to, PieceType piece, Color color, Board &board);
	static Bitboard calculatePath(Position from, Position to, PieceType piece, Color color);
	static Bitboard calculatePawnPath(Position from, Position to, Color color);
	static Bitboard calculateDiagonalPath(Position from, Position to);
	static Bitboard calculateOrthaogonalPath(Position from, Position to);
	static bool isDiagonal(Position from, Position to);
	static bool isOrthogonal(Position from, Position to);

	static void validatePawnMove(Position from, Position to, PieceType piece, Color color, Board &board);
	static void validateKingMove(Position from, Position to, PieceType piece, Color color, Game &game);
	static void validateCastlingMove(Position from, Position to, Color color, Game &game);
	static bool isValidRookPosition(Position from, Color color, Game &game);

	static bool isSquareAttacked(Position position, Color color, Board &board);
};

#endif // MOVEVALIDATOR_HPP