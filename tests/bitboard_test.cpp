#include "gtest/gtest.h"

#include "../include/Bitboard.hpp"

TEST(Bitboard, DefaultConstructor)
{
	Bitboard bitboard;
	EXPECT_EQ(bitboard.getValue(), 0x0ULL);
}

class BitboardValueConstructorTest : public ::testing::TestWithParam<uint64_t> {};

TEST_P(BitboardValueConstructorTest, ValueConstructor)
{
	uint64_t value = GetParam();
	Bitboard bitboard(value);
	EXPECT_EQ(bitboard.getValue(), value);
}

INSTANTIATE_TEST_SUITE_P(
    BitboardValues,
    BitboardValueConstructorTest,
    ::testing::Values(
        0x0ULL,
        0xffffffffffffffffULL,
        0x1ULL,
        0x7fffffffffffffffULL,
        0xdfba6ff5ddeb75deULL,
        0x8a082928084a080aULL
    )
);

TEST(Bitboard, PositionConstructor)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Position position{i, j};
			uint64_t expectedValue = 0x1ULL << (i * 8 + j);
			Bitboard bitboard(position);

			ASSERT_EQ(bitboard.getValue(), expectedValue);
		}
	}
}

TEST(Bitboard, SetGetClearBit)
{
	{
		Bitboard bitboard;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				Position position{i, j};
				bitboard.setBit(position);
				ASSERT_TRUE(bitboard.getBit(position));
				bitboard.clearBit(position);
				ASSERT_FALSE(bitboard.getBit(position));
			}
		}
	}
	{
		Bitboard bitboard;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				Position position{i, j};
				ASSERT_FALSE(bitboard.getBit(position));
			}
		}
	}
}

TEST(Bitboard, SetGetClearInvalidPositions)
{
	Bitboard bitboard;

	std::vector<Position> invalidPositions = {
		{-1, 0},
		{0, -1},
		{-1, -1},
		{8, 0},
		{0, 8},
		{8, 8}};

	for (Position position : invalidPositions)
	{
		// Check that setBit throws an exception
		ASSERT_THROW(bitboard.setBit(position), std::invalid_argument);

		// Check that getBit throws an exception
		ASSERT_THROW(bitboard.getBit(position), std::invalid_argument);

		// Check that clearBit throws an exception
		ASSERT_THROW(bitboard.clearBit(position), std::invalid_argument);
	}
}

struct BitboardAndOrXorOperatorTestParams
{
	uint64_t value1;
	uint64_t value2;
	uint64_t expectedValue;
};

std::string BitboardAndOrXorOperatorTestParamsToString(const testing::TestParamInfo<BitboardAndOrXorOperatorTestParams>& info) {
    std::ostringstream ss;
    ss << "Value1_" << std::hex << info.param.value1;
    ss << "_Value2_" << std::hex << info.param.value2;
    ss << "_Expected_" << std::hex << info.param.expectedValue;
    return ss.str();
}

class BitboardOrOperatorTest : public ::testing::TestWithParam<BitboardAndOrXorOperatorTestParams> {};
class BitboardOrEqualOperatorTest : public ::testing::TestWithParam<BitboardAndOrXorOperatorTestParams> {};

TEST_P(BitboardOrOperatorTest, OrOperator)
{
	BitboardAndOrXorOperatorTestParams params = GetParam();
	Bitboard bitboard1(params.value1);
	Bitboard bitboard2(params.value2);
	Bitboard result = bitboard1 | bitboard2;

	EXPECT_EQ(result.getValue(), params.expectedValue);
}

TEST_P(BitboardOrEqualOperatorTest, OrEqualOperator)
{
    BitboardAndOrXorOperatorTestParams params = GetParam();
    Bitboard bitboard1(params.value1);
    Bitboard bitboard2(params.value2);
    bitboard1 |= bitboard2;

    EXPECT_EQ(bitboard1.getValue(), params.expectedValue);
}

const auto orOperationTestParams = ::testing::Values(
    BitboardAndOrXorOperatorTestParams{0xffffffffffffffffULL, 0x0ULL, 0xffffffffffffffffULL},
    BitboardAndOrXorOperatorTestParams{0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL},
    BitboardAndOrXorOperatorTestParams{0xaa55aa55aa55aa55ULL, 0x55aa55aa55aa55aaULL, 0xffffffffffffffffULL},
    BitboardAndOrXorOperatorTestParams{0xaa55aa55aa55aa55ULL, 0xaa55aa55aa55aa55ULL, 0xaa55aa55aa55aa55ULL},
    BitboardAndOrXorOperatorTestParams{0x800000ULL, 0x0ULL, 0x800000ULL},
    BitboardAndOrXorOperatorTestParams{0x8000000ULL, 0x8000000ULL, 0x8000000ULL},
    BitboardAndOrXorOperatorTestParams{0x800ULL, 0x80000000000ULL, 0x80000000800ULL},
    BitboardAndOrXorOperatorTestParams{0x400000000ULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL}
);

INSTANTIATE_TEST_SUITE_P(
    BitboardValues,
    BitboardOrOperatorTest,
    orOperationTestParams,
    BitboardAndOrXorOperatorTestParamsToString
);

INSTANTIATE_TEST_SUITE_P(
    BitboardValues,
    BitboardOrEqualOperatorTest,
    orOperationTestParams,
    BitboardAndOrXorOperatorTestParamsToString
);

class BitboardAndOperatorTest : public ::testing::TestWithParam<BitboardAndOrXorOperatorTestParams> {};
class BitboardAndEqualOperatorTest : public ::testing::TestWithParam<BitboardAndOrXorOperatorTestParams> {};

TEST_P(BitboardAndOperatorTest, AndOperator)
{
	BitboardAndOrXorOperatorTestParams params = GetParam();
	Bitboard bitboard1(params.value1);
	Bitboard bitboard2(params.value2);
	Bitboard result = bitboard1 & bitboard2;

	EXPECT_EQ(result.getValue(), params.expectedValue);
}

TEST_P(BitboardAndEqualOperatorTest, AndEqualOperator)
{
	BitboardAndOrXorOperatorTestParams params = GetParam();
	Bitboard bitboard1(params.value1);
	Bitboard bitboard2(params.value2);
	bitboard1 &= bitboard2;

	EXPECT_EQ(bitboard1.getValue(), params.expectedValue);
}

const auto andOperationTestParams = ::testing::Values(
	BitboardAndOrXorOperatorTestParams{0xffffffffffffffffULL, 0x0ULL, 0x0ULL},
	BitboardAndOrXorOperatorTestParams{0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL},
	BitboardAndOrXorOperatorTestParams{0xaa55aa55aa55aa55ULL, 0x55aa55aa55aa55aaULL, 0x0ULL},
	BitboardAndOrXorOperatorTestParams{0xaa55aa55aa55aa55ULL, 0xaa55aa55aa55aa55ULL, 0xaa55aa55aa55aa55ULL},
	BitboardAndOrXorOperatorTestParams{0x8000000ULL, 0x0ULL, 0x0ULL},
	BitboardAndOrXorOperatorTestParams{0x8000000ULL, 0x8000000ULL, 0x8000000ULL},
	BitboardAndOrXorOperatorTestParams{0x8000000ULL, 0x1000000000ULL, 0x0ULL},
	BitboardAndOrXorOperatorTestParams{0x20000000000000ULL, 0xffffffffffffffffULL, 0x20000000000000ULL}
);

INSTANTIATE_TEST_SUITE_P(
	BitboardValues,
	BitboardAndOperatorTest,
	andOperationTestParams,
	BitboardAndOrXorOperatorTestParamsToString
);

INSTANTIATE_TEST_SUITE_P(
	BitboardValues,
	BitboardAndEqualOperatorTest,
	andOperationTestParams,
	BitboardAndOrXorOperatorTestParamsToString
);

class BitboardXorOperatorTest : public ::testing::TestWithParam<BitboardAndOrXorOperatorTestParams> {};
class BitboardXorEqualOperatorTest : public ::testing::TestWithParam<BitboardAndOrXorOperatorTestParams> {};

TEST_P(BitboardXorOperatorTest, XorOperator)
{
	BitboardAndOrXorOperatorTestParams params = GetParam();
	Bitboard bitboard1(params.value1);
	Bitboard bitboard2(params.value2);
	Bitboard result = bitboard1 ^ bitboard2;

	EXPECT_EQ(result.getValue(), params.expectedValue);
}

TEST_P(BitboardXorEqualOperatorTest, XorEqualOperator)
{
	BitboardAndOrXorOperatorTestParams params = GetParam();
	Bitboard bitboard1(params.value1);
	Bitboard bitboard2(params.value2);
	bitboard1 ^= bitboard2;

	EXPECT_EQ(bitboard1.getValue(), params.expectedValue);
}

const auto xorOperationTestParams = ::testing::Values(
	BitboardAndOrXorOperatorTestParams{0xffffffffffffffffULL, 0x0ULL, 0xffffffffffffffffULL},
	BitboardAndOrXorOperatorTestParams{0xffffffffffffffffULL, 0xffffffffffffffffULL, 0x0ULL},
	BitboardAndOrXorOperatorTestParams{0xaa55aa55aa55aa55ULL, 0x55aa55aa55aa55aaULL, 0xffffffffffffffffULL},
	BitboardAndOrXorOperatorTestParams{0xaa55aa55aa55aa55ULL, 0xaa55aa55aa55aa55ULL, 0x0ULL},
	BitboardAndOrXorOperatorTestParams{0x800000ULL, 0x0ULL, 0x800000ULL},
	BitboardAndOrXorOperatorTestParams{0x8000000ULL, 0x8000000ULL, 0x0ULL},
	BitboardAndOrXorOperatorTestParams{0x800ULL, 0x80000000000ULL, 0x80000000800ULL},
	BitboardAndOrXorOperatorTestParams{0x400000000ULL, 0xffffffffffffffffULL, 0xfffffffbffffffffULL}
);

INSTANTIATE_TEST_SUITE_P(
	BitboardValues,
	BitboardXorOperatorTest,
	xorOperationTestParams,
	BitboardAndOrXorOperatorTestParamsToString
);

INSTANTIATE_TEST_SUITE_P(
	BitboardValues,
	BitboardXorEqualOperatorTest,
	xorOperationTestParams,
	BitboardAndOrXorOperatorTestParamsToString
);

struct BitboardNotOperatorTestParams
{
	uint64_t value;
	uint64_t expectedValue;
};

std::string BitboardNotOperatorTestParamsToString(const testing::TestParamInfo<BitboardNotOperatorTestParams>& info) {
	std::ostringstream ss;
	ss << "Value_" << std::hex << info.param.value;
	ss << "_Expected_" << std::hex << info.param.expectedValue;
	return ss.str();
}

class BitboardNotOperatorTest : public ::testing::TestWithParam<BitboardNotOperatorTestParams> {};

TEST_P(BitboardNotOperatorTest, NotOperator)
{
	BitboardNotOperatorTestParams params = GetParam();
	Bitboard bitboard(params.value);
	Bitboard result = ~bitboard;

	EXPECT_EQ(result.getValue(), params.expectedValue);
}

const auto notOperationTestParams = ::testing::Values(
	BitboardNotOperatorTestParams{0x0ULL, 0xffffffffffffffffULL},
	BitboardNotOperatorTestParams{0x1ULL, 0xfffffffffffffffeULL},
	BitboardNotOperatorTestParams{0xffffffffffffffffULL, 0x0ULL},
	BitboardNotOperatorTestParams{0x5555555555555555ULL, 0xaaaaaaaaaaaaaaaaULL},
	BitboardNotOperatorTestParams{0x8000000000000000ULL, 0x7fffffffffffffffULL},
	BitboardNotOperatorTestParams{0xaaaaaaaaaaaaaaaaULL, 0x5555555555555555ULL}
);

INSTANTIATE_TEST_SUITE_P(
	BitboardValues,
	BitboardNotOperatorTest,
	notOperationTestParams,
	BitboardNotOperatorTestParamsToString
);

struct BitboardShiftOperatorTestParams
{
	uint64_t value;
	int shift;
	uint64_t expectedValue;
};

std::string BitboardShiftOperatorTestParamsToString(const testing::TestParamInfo<BitboardShiftOperatorTestParams>& info) {
	std::ostringstream ss;
	ss << "Value_" << std::hex << info.param.value;
	ss << "_Shift_" << info.param.shift;
	ss << "_Expected_" << std::hex << info.param.expectedValue;
	return ss.str();
}

class BitboardLeftShiftOperatorTest : public ::testing::TestWithParam<BitboardShiftOperatorTestParams> {};
class BitboardLeftShiftEqualOperatorTest : public ::testing::TestWithParam<BitboardShiftOperatorTestParams> {};

TEST_P(BitboardLeftShiftOperatorTest, LeftShiftOperator)
{
	BitboardShiftOperatorTestParams params = GetParam();
	Bitboard bitboard(params.value);
	Bitboard result = bitboard << params.shift;

	EXPECT_EQ(result.getValue(), params.expectedValue);
}

TEST_P(BitboardLeftShiftEqualOperatorTest, LeftShiftEqualOperator)
{
	BitboardShiftOperatorTestParams params = GetParam();
	Bitboard bitboard(params.value);
	bitboard <<= params.shift;

	EXPECT_EQ(bitboard.getValue(), params.expectedValue);
}

const auto leftShiftOperationTestParams = ::testing::Values(
    BitboardShiftOperatorTestParams{0x0ULL, 1, 0x0ULL},
    BitboardShiftOperatorTestParams{0x10000000ULL, 1, 0x20000000ULL},
    BitboardShiftOperatorTestParams{0x1ULL, 63, 0x8000000000000000ULL},
    BitboardShiftOperatorTestParams{0xffffffffffffffffULL, 1, 0xfffffffffffffffeULL},
    BitboardShiftOperatorTestParams{0xffffffffffffffffULL, 64, 0x0ULL},
    BitboardShiftOperatorTestParams{0x5555555555555555ULL, 1, 0xaaaaaaaaaaaaaaaaULL},
    BitboardShiftOperatorTestParams{0x5555555555555555ULL, 32, 0x5555555500000000ULL},
    BitboardShiftOperatorTestParams{0x8000000000000000ULL, 0, 0x8000000000000000ULL},
    BitboardShiftOperatorTestParams{0x1ULL, 64, 0x0ULL}
);

INSTANTIATE_TEST_SUITE_P(
	BitboardValues,
	BitboardLeftShiftOperatorTest,
	leftShiftOperationTestParams,
	BitboardShiftOperatorTestParamsToString
);

INSTANTIATE_TEST_SUITE_P(
	BitboardValues,
	BitboardLeftShiftEqualOperatorTest,
	leftShiftOperationTestParams,
	BitboardShiftOperatorTestParamsToString
);

class BitboardRightShiftOperatorTest : public ::testing::TestWithParam<BitboardShiftOperatorTestParams> {};
class BitboardRightShiftEqualOperatorTest : public ::testing::TestWithParam<BitboardShiftOperatorTestParams> {};

TEST_P(BitboardRightShiftOperatorTest, RightShiftOperator)
{
	BitboardShiftOperatorTestParams params = GetParam();
	Bitboard bitboard(params.value);
	Bitboard result = bitboard >> params.shift;

	EXPECT_EQ(result.getValue(), params.expectedValue);
}

TEST_P(BitboardRightShiftEqualOperatorTest, RightShiftEqualOperator)
{
	BitboardShiftOperatorTestParams params = GetParam();
	Bitboard bitboard(params.value);
	bitboard >>= params.shift;

	EXPECT_EQ(bitboard.getValue(), params.expectedValue);
}

const auto rightShiftOperationTestParams = ::testing::Values(
    BitboardShiftOperatorTestParams{0x0ULL, 1, 0x0ULL},
    BitboardShiftOperatorTestParams{0x20000000ULL, 1, 0x10000000ULL},
    BitboardShiftOperatorTestParams{0x8000000000000000ULL, 63, 0x1ULL},
    BitboardShiftOperatorTestParams{0xffffffffffffffffULL, 1, 0x7fffffffffffffffULL},
    BitboardShiftOperatorTestParams{0xffffffffffffffffULL, 64, 0x0ULL},
    BitboardShiftOperatorTestParams{0xaaaaaaaaaaaaaaaaULL, 1, 0x5555555555555555ULL},
    BitboardShiftOperatorTestParams{0x5555555500000000ULL, 32, 0x55555555ULL},
    BitboardShiftOperatorTestParams{0x8000000000000000ULL, 0, 0x8000000000000000ULL},
    BitboardShiftOperatorTestParams{0x8000000000000000ULL, 64, 0x0ULL}
);

INSTANTIATE_TEST_SUITE_P(
	BitboardValues,
	BitboardRightShiftOperatorTest,
	rightShiftOperationTestParams,
	BitboardShiftOperatorTestParamsToString
);

INSTANTIATE_TEST_SUITE_P(
	BitboardValues,
	BitboardRightShiftEqualOperatorTest,
	rightShiftOperationTestParams,
	BitboardShiftOperatorTestParamsToString
);

struct BitboardEqualityOperatorTestParams
{
	uint64_t value1;
	uint64_t value2;
	bool expectedEquality;
};

std::string BitboardEqualityOperatorTestParamsToString(const testing::TestParamInfo<BitboardEqualityOperatorTestParams>& info) {
	std::ostringstream ss;
	ss << "Value1_" << std::hex << info.param.value1;
	ss << "_Value2_" << std::hex << info.param.value2;
	ss << "_Expected_" << info.param.expectedEquality;
	return ss.str();
}

class BitboardEqualityOperatorTest : public ::testing::TestWithParam<BitboardEqualityOperatorTestParams> {};
class BitboardInequalityOperatorTest : public ::testing::TestWithParam<BitboardEqualityOperatorTestParams> {};

TEST_P(BitboardEqualityOperatorTest, EqualityOperator)
{
	BitboardEqualityOperatorTestParams params = GetParam();
	Bitboard bitboard1(params.value1);
	Bitboard bitboard2(params.value2);

	EXPECT_EQ(bitboard1 == bitboard2, params.expectedEquality);
}

TEST_P(BitboardInequalityOperatorTest, InequalityOperator)
{
	BitboardEqualityOperatorTestParams params = GetParam();
	Bitboard bitboard1(params.value1);
	Bitboard bitboard2(params.value2);

	EXPECT_EQ(bitboard1 != bitboard2, !params.expectedEquality);
}

const auto equalityOperationTestParams = ::testing::Values(
    BitboardEqualityOperatorTestParams{0x0ULL, 0x0ULL, true},
    BitboardEqualityOperatorTestParams{0x0ULL, 0x1ULL, false},
    BitboardEqualityOperatorTestParams{0x1ULL, 0x0ULL, false},
    BitboardEqualityOperatorTestParams{0x1ULL, 0x1ULL, true},
    BitboardEqualityOperatorTestParams{0x1ULL, 0x2ULL, false},
    BitboardEqualityOperatorTestParams{0xffffffffffffffffULL, 0x0ULL, false},
    BitboardEqualityOperatorTestParams{0xffffffffffffffffULL, 0xffffffffffffffffULL, true}
);

INSTANTIATE_TEST_SUITE_P(
	BitboardValues,
	BitboardEqualityOperatorTest,
	equalityOperationTestParams,
	BitboardEqualityOperatorTestParamsToString
);

INSTANTIATE_TEST_SUITE_P(
	BitboardValues,
	BitboardInequalityOperatorTest,
	equalityOperationTestParams,
	BitboardEqualityOperatorTestParamsToString
);

struct BitboardBitScanForwardTestParams
{
	uint64_t value;
	int expectedBitScanForward;
};

class BitboardBitScanForwardTest : public ::testing::TestWithParam<BitboardBitScanForwardTestParams> {};

TEST_P(BitboardBitScanForwardTest, BitScanForward)
{
	BitboardBitScanForwardTestParams params = GetParam();
	Bitboard bitboard(params.value);

	EXPECT_EQ(bitboard.bitScanForward(), params.expectedBitScanForward);
}

const auto bitScanForwardTestParams = ::testing::Values(
	BitboardBitScanForwardTestParams{0x0ULL, -1},
	BitboardBitScanForwardTestParams{0xa28ffdec2e1f93c0ULL, 6},
	BitboardBitScanForwardTestParams{0x37a3a0b6b8ef8f68ULL, 3},
	BitboardBitScanForwardTestParams{0xd816baa730035800ULL, 11},
	BitboardBitScanForwardTestParams{0x79a902f96badb600ULL, 9},
	BitboardBitScanForwardTestParams{0x835717f7b1f44f70ULL, 4},
	BitboardBitScanForwardTestParams{0x9bcae086dea6cf00ULL, 8},
	BitboardBitScanForwardTestParams{0x6b21546f1fdd9000ULL, 12},
	BitboardBitScanForwardTestParams{0x681c4da9a8000000ULL, 27},
	BitboardBitScanForwardTestParams{0xb273f94000000000ULL, 38},
	BitboardBitScanForwardTestParams{0x69c2000000000000ULL, 49},
	BitboardBitScanForwardTestParams{0x6800000000000000ULL, 59},
	BitboardBitScanForwardTestParams{0x8000000000000000ULL, 63},
	BitboardBitScanForwardTestParams{0x1ULL, 0},
	BitboardBitScanForwardTestParams{0xffffffffffffffffULL, 0}
);

INSTANTIATE_TEST_SUITE_P(
	BitboardValues,
	BitboardBitScanForwardTest,
	bitScanForwardTestParams
);