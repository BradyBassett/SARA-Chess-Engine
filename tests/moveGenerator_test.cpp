#include <gtest/gtest.h>

#include "../include/MoveGenerator.hpp"
#include "../include/Utility.hpp"

struct GeneratePotentialMovesTestParams
{
	PieceType piece;
	Color color;
	Position position;
	Bitboard expectedMoves;
};

class GeneratePotentialMovesTest : public ::testing::TestWithParam<GeneratePotentialMovesTestParams> {};

TEST_P(GeneratePotentialMovesTest, GeneratePotentialMoves)
{
	auto params = GetParam();
	Bitboard moves = MoveGenerator::generatePotentialMoves(params.piece, params.color, params.position );
	EXPECT_EQ(moves, params.expectedMoves);
}

// Test cases only for pawns and kings since the other pieces are tested in the GenerateAttacksTests
const auto generatePotentialMovesTestParams = ::testing::Values(
	GeneratePotentialMovesTestParams{PieceType::PAWN, Color::WHITE, Position{Utility::convertStringToPosition("a2")}, 0x30100000000ULL},
	GeneratePotentialMovesTestParams{PieceType::PAWN, Color::WHITE, Position{Utility::convertStringToPosition("h2")}, 0xc08000000000ULL},
	GeneratePotentialMovesTestParams{PieceType::PAWN, Color::WHITE, Position{Utility::convertStringToPosition("b8")}, 0x0ULL},
	GeneratePotentialMovesTestParams{PieceType::PAWN, Color::WHITE, Position{Utility::convertStringToPosition("e6")}, 0x3800ULL},
	GeneratePotentialMovesTestParams{PieceType::PAWN, Color::WHITE, Position{Utility::convertStringToPosition("d2")}, 0x1c0800000000ULL},
	GeneratePotentialMovesTestParams{PieceType::PAWN, Color::BLACK, Position{Utility::convertStringToPosition("a7")}, 0x1030000ULL},
	GeneratePotentialMovesTestParams{PieceType::PAWN, Color::BLACK, Position{Utility::convertStringToPosition("h7")}, 0x80c00000ULL},
	GeneratePotentialMovesTestParams{PieceType::PAWN, Color::WHITE, Position{Utility::convertStringToPosition("d7")}, 0x1cULL},
	GeneratePotentialMovesTestParams{PieceType::PAWN, Color::BLACK, Position{Utility::convertStringToPosition("b1")}, 0x0ULL},
	GeneratePotentialMovesTestParams{PieceType::PAWN, Color::BLACK, Position{Utility::convertStringToPosition("d6")}, 0x1c000000ULL},
	GeneratePotentialMovesTestParams{PieceType::KING, Color::WHITE, Position{Utility::convertStringToPosition("e1")}, 0x6c38000000000000ULL},
	GeneratePotentialMovesTestParams{PieceType::KING, Color::WHITE, Position{Utility::convertStringToPosition("d1")}, 0x141c000000000000ULL},
	GeneratePotentialMovesTestParams{PieceType::KING, Color::WHITE, Position{Utility::convertStringToPosition("f3")}, 0x70507000000000ULL},
	GeneratePotentialMovesTestParams{PieceType::KING, Color::WHITE, Position{Utility::convertStringToPosition("e8")}, 0x3828ULL},
	GeneratePotentialMovesTestParams{PieceType::KING, Color::BLACK, Position{Utility::convertStringToPosition("e8")}, 0x386cULL},
	GeneratePotentialMovesTestParams{PieceType::KING, Color::BLACK, Position{Utility::convertStringToPosition("d8")}, 0x1c14ULL},
	GeneratePotentialMovesTestParams{PieceType::KING, Color::BLACK, Position{Utility::convertStringToPosition("b4")}, 0x70507000000ULL},
	GeneratePotentialMovesTestParams{PieceType::KING, Color::BLACK, Position{Utility::convertStringToPosition("e1")}, 0x2838000000000000ULL}
);

INSTANTIATE_TEST_SUITE_P(GeneratePotentialMovesTests, GeneratePotentialMovesTest, generatePotentialMovesTestParams);