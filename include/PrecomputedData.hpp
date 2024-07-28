#ifndef PRECOMPUTEDDATA_HPP
#define PRECOMPUTEDDATA_HPP

#include "Bitboard.hpp"

#include <array>
#include <cstdint>

namespace PrecomputedData
{
	// Precomputed magic numbers
	extern const std::array<uint64_t, 64> rookMagics;
	extern const std::array<uint64_t, 64> bishopMagics;
	// Precomputed shifts
	extern const std::array<int, 64> rookShifts;
	extern const std::array<int, 64> bishopShifts;
	// Precomputed masks
	extern const std::array<uint64_t, 64> rookMasks;
	extern const std::array<uint64_t, 64> bishopMasks;
	// // Precomputed rook attacks
	// extern const std::array<std::array<Bitboard, 4096>, 64> rookAttacks;
	// // Precomputed bishop attacks
	// extern const std::array<std::array<Bitboard, 512>, 64> bishopAttacks;
	// Precomputed pawn attacks
	extern const std::array<std::array<Bitboard, 64>, 2> pawnAttacks;
	// Precomputed knight attacks
	extern const std::array<Bitboard, 64> knightAttacks;
	// Precomputed king attacks
	extern const std::array<Bitboard, 64> kingAttacks;
	// // Precomputed rays
	// extern const std::array<std::array<Bitboard, 64>, 64> rays;
}

#endif // PRECOMPUTEDDATA_HPP