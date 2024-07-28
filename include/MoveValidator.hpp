#ifndef MOVEVALIDATOR_HPP
#define MOVEVALIDATOR_HPP

#include "Game.hpp"

class MoveValidator
{
public:
	static void validateMove(Position from, Position to, PieceType piece, Color friendlyColor, Game &game);
	static bool calculateInCheck();
	static Bitboard findAbsolutePins(Board &board, Color friendlyColor);

private:
	static Bitboard generatePotentialMoves(Position position, PieceType piece, Color friendlyColor, Board &board);
	static Bitboard generatePotentialPawnMoves(Color friendlyColor, Position position, Bitboard moves, Bitboard occupied);
	static bool canDoublePawnPush(Color friendlyColor, Position position, Bitboard occupied);
	static Bitboard generatePotentialKingMoves(Color friendlyColor, Position position, Bitboard moves);
	static bool isDiagonal(Position from, Position to);

	static void validatePawnMove(Position from, Position to, PieceType piece, Color friendlyColor, Board &board);
	static void validateKingMove(Position from, Position to, PieceType piece, Color friendlyColor, Game &game);
	static bool isValidPinnedPieceMove(Position from, Position to, Color friendlyColor, Board &board);
	static void validateCastlingMove(Position from, Position to, Color friendlyColor, Game &game);

	static bool isSquareAttacked(Position position, Color friendlyColor, Board &board);
	static Bitboard xrayAttacks(Bitboard occupied, Bitboard friendlyPieces, int kingSquare, PieceType piece);
	static Bitboard getObstructedRay(Bitboard ray, Bitboard occupied);
};

#endif // MOVEVALIDATOR_HPP