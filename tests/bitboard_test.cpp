#include "gtest/gtest.h"

#include "../include/Bitboard.hpp"

TEST(Bitboard, DefaultConstructor)
{
	Bitboard bitboard;
	EXPECT_EQ(bitboard.getValue(), 0x0ULL);
}

TEST(Bitboard, ValueConstructor)
{
	{
		Bitboard bitboard(0x0ULL);
		EXPECT_EQ(bitboard.getValue(), 0x0ULL) << "Test case 1: Value is not as expected";;
	}
	{
		Bitboard bitboard(0xffffffffffffffffULL);
		EXPECT_EQ(bitboard.getValue(), 0xffffffffffffffffULL) << "Test case 2: Value is not as expected";;
	}
	{
		Bitboard bitboard(0x1ULL);
		EXPECT_EQ(bitboard.getValue(), 0x1ULL) << "Test case 3: Value is not as expected";;
	}
	{
		Bitboard bitboard(0x7fffffffffffffffULL);
		EXPECT_EQ(bitboard.getValue(), 0x7fffffffffffffffULL) << "Test case 4: Value is not as expected";;
	}
	{
		Bitboard bitboard(0xdfba6ff5ddeb75deULL);
		EXPECT_EQ(bitboard.getValue(), 0xdfba6ff5ddeb75deULL) << "Test case 5: Value is not as expected";;
	}
	{
		Bitboard bitboard(0x8a082928084a080aULL);
		EXPECT_EQ(bitboard.getValue(), 0x8a082928084a080aULL) << "Test case 6: Value is not as expected";;
	}
}

TEST(Bitboard, PositionConstructor)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Position position = {i, j};
			uint64_t expectedValue = 0x1ULL << (i * 8 + j);
			Bitboard bitboard(position);

			ASSERT_EQ(bitboard.getValue(), expectedValue);
		}
	}
}