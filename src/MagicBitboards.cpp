#include "../include/MagicBitboards.hpp"
#include "../include/structs/Position.hpp"
#include "../include/Utility.hpp"

std::once_flag MagicBitboards::initFlag;

std::array<uint64_t, 64> MagicBitboards::rookMagics = {};
std::array<uint64_t, 64> MagicBitboards::bishopMagics = {};
std::array<int, 64> MagicBitboards::rookShifts = {};
std::array<int, 64> MagicBitboards::bishopShifts = {};
std::array<uint64_t, 64> MagicBitboards::rookMasks = {};
std::array<uint64_t, 64> MagicBitboards::bishopMasks = {};
std::array<std::array<uint64_t, 4096>, 64> MagicBitboards::rookAttacks = {};
std::array<std::array<uint64_t, 512>, 64> MagicBitboards::bishopAttacks = {};

void MagicBitboards::init(std::string rookMagicsPath, std::string bishopMagicsPath, std::string rookShiftsPath, std::string bishopShiftsPath, std::string rookMasksPath, std::string bishopMasksPath)
{
	std::call_once(initFlag, [rookMagicsPath, bishopMagicsPath, rookShiftsPath, bishopShiftsPath, rookMasksPath, bishopMasksPath]() {
		Utility::loadArrayFromJson(rookMagicsPath, rookMagics);
		Utility::loadArrayFromJson(bishopMagicsPath, bishopMagics);
		Utility::loadArrayFromJson(rookShiftsPath, rookShifts);
		Utility::loadArrayFromJson(bishopShiftsPath, bishopShifts);
		Utility::loadArrayFromJson(rookMasksPath, rookMasks);
		Utility::loadArrayFromJson(bishopMasksPath, bishopMasks);

		std::array<uint64_t, 64> rm = rookMagics;
		std::array<uint64_t, 64> bm = bishopMagics;
		std::array<int, 64> rs = rookShifts;
		std::array<int, 64> bs = bishopShifts;
		std::array<uint64_t, 64> rmasks = rookMasks;
		std::array<uint64_t, 64> bmasks = bishopMasks;

		for (int square = 0; square < 64; ++square)
		{
			rookAttacks[square] = createTable<4096>(square, true, rookMagics[square], rookShifts[square]);
			bishopAttacks[square] = createTable<512>(square, false, bishopMagics[square], bishopShifts[square]);
		}
	});
}

Bitboard MagicBitboards::getSliderAttacks(int square, Bitboard occupied, PieceType pieceType)
{
	switch (pieceType)
	{
	case PieceType::ROOK:
		return getRookAttacks(square, occupied);
	case PieceType::BISHOP:
		return getBishopAttacks(square, occupied);
	case PieceType::QUEEN:
		return getBishopAttacks(square, occupied) | getRookAttacks(square, occupied);
	default:
		return Bitboard(0);
	}
}

Bitboard MagicBitboards::getRookAttacks(int square, Bitboard occupied)
{
	uint64_t index = (rookMasks[square] & occupied.getValue()) * rookMagics[square] >> rookShifts[square];
	return rookAttacks[square][index];
}

Bitboard MagicBitboards::getBishopAttacks(int square, Bitboard occupied)
{
	uint64_t index = (bishopMasks[square] & occupied.getValue()) * bishopMagics[square] >> bishopShifts[square];
	return bishopAttacks[square][index];
}

std::vector<uint64_t> MagicBitboards::createAllBlockerPatterns(uint64_t mask)
{
	// Create a vector containing indices of the set bits in the mask
	std::vector<int> maskBits = {};
	for (int square = 0; square < 64; square++)
	{
		if (((mask >> square) & 1) == 1)
		{
			maskBits.push_back(square);
		}
	}

	int numPatterns = 1 << maskBits.size();
	std::vector<uint64_t> blockerBitboards;
	blockerBitboards.resize(numPatterns, 0);

	for (int patternIndex = 0; patternIndex < numPatterns; patternIndex++)
	{
		for (int bitIndex = 0; bitIndex < maskBits.size(); bitIndex++)
		{
			int bit = (patternIndex >> bitIndex) & 1;
			blockerBitboards[patternIndex] |= (static_cast<u_int64_t>(bit) << maskBits[bitIndex]);
		}
	}

	return blockerBitboards;
}

uint64_t MagicBitboards::legalMovesFromBlockers(int square, uint64_t blockers, bool rook)
{
	Bitboard moves;
	std::array<Position, 4> directions = rook ?
		std::array<Position, 4>{{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}} :
		std::array<Position, 4>{{{1, 1}, {-1, -1}, {1, -1}, {-1, 1}}};
	Position startPos = Utility::calculatePosition(square);

	for (Position direction : directions)
	{
		for (int distance = 1; distance < 8; distance++)
		{
			Position targetPos = Position{startPos.row + (direction.row * distance), startPos.col + (direction.col * distance)};
			try
			{
				Utility::validatePosition(targetPos);
			}
			catch(const std::exception& e)
			{
				break;
			}

			// If target is valid, set the bit in the moves bitboard, and check if there is a blocker at the target position
			moves.setBit(targetPos);
			if (Bitboard(blockers).getBit(targetPos))
			{
				break;
			}
		}
	}

	return moves.getValue();
}
