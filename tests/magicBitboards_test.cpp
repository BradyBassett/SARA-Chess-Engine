#include <gtest/gtest.h>

#include "../include/MagicBitboards.hpp"

class MagicBitboardsTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		MagicBitboards::init("../src/data/rookMagics.json", "../src/data/bishopMagics.json", "../src/data/rookShifts.json", "../src/data/bishopShifts.json", "../src/data/rookMasks.json", "../src/data/bishopMasks.json");
	}
};

TEST_F(MagicBitboardsTest, AccessingRookAttacks)
{
	ASSERT_EQ(MagicBitboards::getSliderAttacks(0, Bitboard(0x8004010048040050ULL), PieceType::ROOK).getValue(), 0x1010101011eULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(7, Bitboard(0x4082000011048000ULL), PieceType::ROOK).getValue(), 0x807fULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(56, Bitboard(0x1440800801014400ULL), PieceType::ROOK).getValue(), 0x601010101000000ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(63, Bitboard(0x400820100a42010ULL), PieceType::ROOK).getValue(), 0x7c80800000000000ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(28, Bitboard(0x8812420008025000ULL), PieceType::ROOK).getValue(), 0x101010e8101000ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(2, Bitboard(0x8008400059000c2ULL), PieceType::ROOK).getValue(), 0x404047aULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(57, Bitboard(0x14000050001a0160ULL), PieceType::ROOK).getValue(), 0x502020202020000ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(16, Bitboard(0x2501804440100051ULL), PieceType::ROOK).getValue(), 0x10101011e0101ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(23, Bitboard(0x4401100202400008ULL), PieceType::ROOK).getValue(), 0x8080808080408080ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(48, Bitboard(0x2488480129050440ULL), PieceType::ROOK).getValue(), 0x10e010100000000ULL);
}

TEST_F(MagicBitboardsTest, AccessingBishopAttacks)
{
	ASSERT_EQ(MagicBitboards::getSliderAttacks(0, Bitboard(0x8004010048040050ULL), PieceType::BISHOP).getValue(), 0x40200ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(7, Bitboard(0x4082000011048000ULL), PieceType::BISHOP).getValue(), 0x10204000ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(56, Bitboard(0x1440800801014400ULL), PieceType::BISHOP).getValue(), 0x2040800000000ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(63, Bitboard(0x400820100a42010ULL), PieceType::BISHOP).getValue(), 0x40201008040000ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(28, Bitboard(0x8812420008025000ULL), PieceType::BISHOP).getValue(), 0x2442800284402ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(2, Bitboard(0x8008400059000c2ULL), PieceType::BISHOP).getValue(), 0x110a00ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(57, Bitboard(0x14000050001a0160ULL), PieceType::BISHOP).getValue(), 0x5081000000000ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(16, Bitboard(0x2501804440100051ULL), PieceType::BISHOP).getValue(), 0x402000204ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(23, Bitboard(0x4401100202400008ULL), PieceType::BISHOP).getValue(), 0x102040004020ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(48, Bitboard(0x2488480129050440ULL), PieceType::BISHOP).getValue(), 0x200020408000000ULL);
}

TEST_F(MagicBitboardsTest, AccessingQueenAttacks)
{
	ASSERT_EQ(MagicBitboards::getSliderAttacks(0, Bitboard(0x8004010048040050ULL), PieceType::QUEEN).getValue(), 0x1010105031eULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(7, Bitboard(0x4082000011048000ULL), PieceType::QUEEN).getValue(), 0x1020c07fULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(56, Bitboard(0x1440800801014400ULL), PieceType::QUEEN).getValue(), 0x603050901000000ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(63, Bitboard(0x400820100a42010ULL), PieceType::QUEEN).getValue(), 0x7cc0a01008040000ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(28, Bitboard(0x8812420008025000ULL), PieceType::QUEEN).getValue(), 0x125438e8385402ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(2, Bitboard(0x8008400059000c2ULL), PieceType::QUEEN).getValue(), 0x4150e7aULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(57, Bitboard(0x14000050001a0160ULL), PieceType::QUEEN).getValue(), 0x5070a1202020000ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(16, Bitboard(0x2501804440100051ULL), PieceType::QUEEN).getValue(), 0x10105031e0305ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(23, Bitboard(0x4401100202400008ULL), PieceType::QUEEN).getValue(), 0x808090a0c040c0a0ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(48, Bitboard(0x2488480129050440ULL), PieceType::QUEEN).getValue(), 0x30e030508000000ULL);
}