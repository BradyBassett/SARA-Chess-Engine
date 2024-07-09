#include <gtest/gtest.h>

#include "../include/MagicBitboards.hpp"

class MagicBitboardsTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		MagicBitboards::init();
	}
};

TEST_F(MagicBitboardsTest, AccessingRookAttacks)
{
	ASSERT_EQ(MagicBitboards::getSliderAttacks(0, Bitboard(0x10000000050ULL), PieceType::ROOK).getValue(), 0x1010101011eULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(7, Bitboard(0x80000000008000ULL), PieceType::ROOK).getValue(), 0x807fULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(56, Bitboard(0x400000001010000ULL), PieceType::ROOK).getValue(), 0x601010101000000ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(63, Bitboard(0x400800000800000ULL), PieceType::ROOK).getValue(), 0x7c80800000000000ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(28, Bitboard(0x100089001000ULL), PieceType::ROOK).getValue(), 0x1010e8101000ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(2, Bitboard(0x40004000042ULL), PieceType::ROOK).getValue(), 0x404047aULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(57, Bitboard(0x1400000000020000ULL), PieceType::ROOK).getValue(), 0x502020202020000ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(16, Bitboard(0x101000000100001ULL), PieceType::ROOK).getValue(), 0x10101011e0101ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(23, Bitboard(0x400000ULL), PieceType::ROOK).getValue(), 0x8080808080408080ULL);
	ASSERT_EQ(MagicBitboards::getSliderAttacks(48, Bitboard(0x88000101010000ULL), PieceType::ROOK).getValue(), 0x10e010101010000ULL);
}

// TEST_F(MagicBitboardsTest, AccessingBishopAttacks)
// {
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(0, Bitboard(0xULL), PieceType::BISHOP).getValue(), 0xULL);
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(7, Bitboard(0xULL), PieceType::BISHOP).getValue(), 0xULL);
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(56, Bitboard(0xULL), PieceType::BISHOP).getValue(), 0xULL);
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(63, Bitboard(0xULL), PieceType::BISHOP).getValue(), 0xULL);
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(28, Bitboard(0xULL), PieceType::BISHOP).getValue(), 0xULL);
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(2, Bitboard(0xULL), PieceType::BISHOP).getValue(), 0xULL);
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(57, Bitboard(0xULL), PieceType::BISHOP).getValue(), 0xULL);
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(16, Bitboard(0xULL), PieceType::BISHOP).getValue(), 0xULL);
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(23, Bitboard(0xULL), PieceType::BISHOP).getValue(), 0xULL);
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(48, Bitboard(0xULL), PieceType::BISHOP).getValue(), 0xULL);
// }

// TEST_F(MagicBitboardsTest, AccessingQueenAttacks)
// {
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(0, Bitboard(0xULL), PieceType::QUEEN).getValue(), 0xULL);
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(7, Bitboard(0xULL), PieceType::QUEEN).getValue(), 0xULL);
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(56, Bitboard(0xULL), PieceType::QUEEN).getValue(), 0xULL);
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(63, Bitboard(0xULL), PieceType::QUEEN).getValue(), 0xULL);
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(28, Bitboard(0xULL), PieceType::QUEEN).getValue(), 0xULL);
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(2, Bitboard(0xULL), PieceType::QUEEN).getValue(), 0xULL);
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(57, Bitboard(0xULL), PieceType::QUEEN).getValue(), 0xULL);
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(16, Bitboard(0xULL), PieceType::QUEEN).getValue(), 0xULL);
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(23, Bitboard(0xULL), PieceType::QUEEN).getValue(), 0xULL);
// 	ASSERT_EQ(MagicBitboards::getSliderAttacks(48, Bitboard(0xULL), PieceType::QUEEN).getValue(), 0xULL);
// }