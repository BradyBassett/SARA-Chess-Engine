#ifndef MAGICBITBOARDS_HPP
#define MAGICBITBOARDS_HPP

#include "Bitboard.hpp"
#include "enums/PieceType.hpp"

#include <array>
#include <vector>
#include <mutex>

class MagicBitboards
{
public:
	static void init(std::string rookMagicsPath, std::string bishopMagicsPath, std::string rookShiftsPath, std::string bishopShiftsPath, std::string rookMasksPath, std::string bishopMasksPath);
	static Bitboard getSliderAttacks(int square, Bitboard occupied, PieceType pieceType);
	static std::array<std::array<uint64_t, 4096>, 64> getRookAttacks();
	static std::array<std::array<uint64_t, 512>, 64> getBishopAttacks();

private:
	static std::once_flag initFlag;

	// Precomputed magic numbers for sliders
	static std::array<uint64_t, 64> rookMagics;
	static std::array<uint64_t, 64> bishopMagics;

	// Precomputed shift values for sliders
	static std::array<int, 64> rookShifts;
	static std::array<int, 64> bishopShifts;

	// Masks for the sliders
	static std::array<uint64_t, 64> rookMasks;
	static std::array<uint64_t, 64> bishopMasks;

	// Attacks for the sliders
	static std::array<std::array<uint64_t, 4096>, 64> rookAttacks;
	static std::array<std::array<uint64_t, 512>, 64> bishopAttacks;

	static Bitboard getRookAttacks(int square, Bitboard occupied);
	static Bitboard getBishopAttacks(int square, Bitboard occupied);
	static std::vector<uint64_t> createAllBlockerPatterns(uint64_t mask);
	static uint64_t legalMovesFromBlockers(int square, uint64_t blockers, bool rook);

	template<size_t N>
	static std::array<uint64_t, N> createTable(int square, bool rook, uint64_t magic, int shift)
	{
		std::array<uint64_t, N> table = {};

		uint64_t movementMask = rook ? rookMasks[square] : bishopMasks[square];
		std::vector<uint64_t> blockerPatterns = createAllBlockerPatterns(movementMask);

		for (uint64_t pattern : blockerPatterns)
		{
			uint64_t index = (pattern * magic) >> shift;
			uint64_t moves = legalMovesFromBlockers(square, pattern, rook);
			table[index] = moves;
		}

		return table;
	}
};

#endif // MAGICBITBOARDS_HPP