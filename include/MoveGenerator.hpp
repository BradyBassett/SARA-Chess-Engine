#ifndef MOVE_GENERATOR_HPP
#define MOVE_GENERATOR_HPP

#include "../include/enums/PieceType.hpp"
#include "../include/enums/Color.hpp"
#include "Bitboard.hpp"

class MoveGenerator
{
public:
	static Bitboard generatePotentialMoves(PieceType piece, Color color, Position position);

private:
	static Bitboard generatePotentialPawnMoves(Color color, Position position);
	static Bitboard generatePotentialKingMoves(Color color, Position position);

	static Bitboard generatePawnAttacks(Color color, Position position);
	static Bitboard generateKnightAttacks(Position position);
	static Bitboard generateSlidingAttacks(PieceType piece, Position position);
	static Bitboard generateKingAttacks(Position position);

	static bool canDoublePawnPush(Color color, Position position);
};

#endif // MOVE_GENERATOR_HPP