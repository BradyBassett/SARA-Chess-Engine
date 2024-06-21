#ifndef MOVE_GENERATOR_HPP
#define MOVE_GENERATOR_HPP

#include "../include/enums/PieceType.hpp"
#include "../include/enums/Color.hpp"
#include "Bitboard.hpp"

class MoveGenerator
{
public:
	// static Bitboard generateMoves();
	static Bitboard generateAttacks(PieceType piece, Color color, Position position); // Color is only used for pawns

private:
	// static Bitboard generatePotentialPawnMoves();
	// static Bitboard generatePotentialKnightMoves();
	// static Bitboard generatePotentialSlidingMoves();
	// static Bitboard generatePotentialKingMoves();

	static Bitboard generatePawnAttacks(Color color, Position position);
	static Bitboard generateKnightAttacks(Position position);
	static Bitboard generateSlidingAttacks(PieceType piece, Position position);
	static Bitboard generateKingAttacks(Position position);
};

#endif // MOVE_GENERATOR_HPP