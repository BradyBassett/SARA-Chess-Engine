#include <gtest/gtest.h>

#include "../include/MoveGenerator.hpp"
#include "../include/Utility.hpp"

struct GenerateAttacksTestParams
{
	PieceType piece;
	Color color;
	Position position;
	Bitboard expectedAttacks;
};

class GenerateAttacksTest : public ::testing::TestWithParam<GenerateAttacksTestParams> {};

TEST_P(GenerateAttacksTest, GenerateAttacks)
{
	auto params = GetParam();
	Bitboard attacks = MoveGenerator::generateAttacks(params.piece, params.color, params.position);
	EXPECT_EQ(attacks, params.expectedAttacks);
}

const auto generateAttacksTestParams = ::testing::Values(
	GenerateAttacksTestParams{PieceType::PAWN, Color::WHITE, Position{Utility::convertStringToPosition("a1")}, 0x2000000000000ULL},
	GenerateAttacksTestParams{PieceType::PAWN, Color::WHITE, Position{Utility::convertStringToPosition("h1")}, 0x40000000000000ULL},
	GenerateAttacksTestParams{PieceType::PAWN, Color::WHITE, Position{Utility::convertStringToPosition("b8")}, 0x0ULL},
	GenerateAttacksTestParams{PieceType::PAWN, Color::WHITE, Position{Utility::convertStringToPosition("a4")}, 0x2000000ULL},
	GenerateAttacksTestParams{PieceType::PAWN, Color::WHITE, Position{Utility::convertStringToPosition("d6")}, 0x1400ULL},
	GenerateAttacksTestParams{PieceType::PAWN, Color::WHITE, Position{Utility::convertStringToPosition("f7")}, 0x50ULL},
	GenerateAttacksTestParams{PieceType::PAWN, Color::WHITE, Position{Utility::convertStringToPosition("g2")}, 0xa00000000000ULL},
	GenerateAttacksTestParams{PieceType::PAWN, Color::WHITE, Position{Utility::convertStringToPosition("e6")}, 0x2800ULL},
	GenerateAttacksTestParams{PieceType::PAWN, Color::BLACK, Position{Utility::convertStringToPosition("a8")}, 0x200ULL},
	GenerateAttacksTestParams{PieceType::PAWN, Color::BLACK, Position{Utility::convertStringToPosition("h8")}, 0x4000ULL},
	GenerateAttacksTestParams{PieceType::PAWN, Color::BLACK, Position{Utility::convertStringToPosition("b1")}, 0x0ULL},
	GenerateAttacksTestParams{PieceType::PAWN, Color::BLACK, Position{Utility::convertStringToPosition("a4")}, 0x20000000000ULL},
	GenerateAttacksTestParams{PieceType::PAWN, Color::BLACK, Position{Utility::convertStringToPosition("d6")}, 0x14000000ULL},
	GenerateAttacksTestParams{PieceType::PAWN, Color::BLACK, Position{Utility::convertStringToPosition("f7")}, 0x500000ULL},
	GenerateAttacksTestParams{PieceType::PAWN, Color::BLACK, Position{Utility::convertStringToPosition("g2")}, 0xa000000000000000ULL},
	GenerateAttacksTestParams{PieceType::PAWN, Color::BLACK, Position{Utility::convertStringToPosition("e6")}, 0x28000000ULL},
	GenerateAttacksTestParams{PieceType::KNIGHT, Color::WHITE, Position{Utility::convertStringToPosition("e4")}, 0x28440044280000ULL},
	GenerateAttacksTestParams{PieceType::KNIGHT, Color::WHITE, Position{Utility::convertStringToPosition("c6")}, 0xa1100110aULL},
	GenerateAttacksTestParams{PieceType::KNIGHT, Color::BLACK, Position{Utility::convertStringToPosition("f3")}, 0x5088008850000000ULL},
	GenerateAttacksTestParams{PieceType::KNIGHT, Color::BLACK, Position{Utility::convertStringToPosition("d5")}, 0x142200221400ULL},
	GenerateAttacksTestParams{PieceType::KNIGHT, Color::WHITE, Position{Utility::convertStringToPosition("b2")}, 0x800080500000000ULL},
	GenerateAttacksTestParams{PieceType::KNIGHT, Color::WHITE, Position{Utility::convertStringToPosition("h8")}, 0x402000ULL},
	GenerateAttacksTestParams{PieceType::KNIGHT, Color::BLACK, Position{Utility::convertStringToPosition("a1")}, 0x4020000000000ULL},
	GenerateAttacksTestParams{PieceType::KNIGHT, Color::BLACK, Position{Utility::convertStringToPosition("g7")}, 0xa0100010ULL},
	GenerateAttacksTestParams{PieceType::BISHOP, Color::WHITE, Position{Utility::convertStringToPosition("g5")}, 0x40810a000a01008ULL},
	GenerateAttacksTestParams{PieceType::BISHOP, Color::WHITE, Position{Utility::convertStringToPosition("b8")}, 0x80402010080500ULL},
	GenerateAttacksTestParams{PieceType::BISHOP, Color::WHITE, Position{Utility::convertStringToPosition("d1")}, 0x14224180000000ULL},
	GenerateAttacksTestParams{PieceType::BISHOP, Color::WHITE, Position{Utility::convertStringToPosition("f4")}, 0x488500050880402ULL},
	GenerateAttacksTestParams{PieceType::BISHOP, Color::WHITE, Position{Utility::convertStringToPosition("e7")}, 0x18244280028ULL},
	GenerateAttacksTestParams{PieceType::BISHOP, Color::WHITE, Position{Utility::convertStringToPosition("c2")}, 0xa000a1120408000ULL},
	GenerateAttacksTestParams{PieceType::BISHOP, Color::WHITE, Position{Utility::convertStringToPosition("h3")}, 0x2040004020100804ULL},
	GenerateAttacksTestParams{PieceType::BISHOP, Color::WHITE, Position{Utility::convertStringToPosition("a4")}, 0x804020002040810ULL},
	GenerateAttacksTestParams{PieceType::ROOK, Color::WHITE, Position{Utility::convertStringToPosition("a5")}, 0x1010101fe010101ULL},
	GenerateAttacksTestParams{PieceType::ROOK, Color::WHITE, Position{Utility::convertStringToPosition("b3")}, 0x202fd0202020202ULL},
	GenerateAttacksTestParams{PieceType::ROOK, Color::BLACK, Position{Utility::convertStringToPosition("d4")}, 0x80808f708080808ULL},
	GenerateAttacksTestParams{PieceType::ROOK, Color::BLACK, Position{Utility::convertStringToPosition("h1")}, 0x7f80808080808080ULL},
	GenerateAttacksTestParams{PieceType::ROOK, Color::BLACK, Position{Utility::convertStringToPosition("d6")}, 0x808080808f70808ULL},
	GenerateAttacksTestParams{PieceType::ROOK, Color::BLACK, Position{Utility::convertStringToPosition("g1")}, 0xbf40404040404040ULL},
	GenerateAttacksTestParams{PieceType::ROOK, Color::BLACK, Position{Utility::convertStringToPosition("b5")}, 0x2020202fd020202ULL},
	GenerateAttacksTestParams{PieceType::ROOK, Color::BLACK, Position{Utility::convertStringToPosition("f2")}, 0x20df202020202020ULL},
	GenerateAttacksTestParams{PieceType::QUEEN, Color::WHITE, Position{Utility::convertStringToPosition("c7")}, 0x4844424150efb0eULL},
	GenerateAttacksTestParams{PieceType::QUEEN, Color::WHITE, Position{Utility::convertStringToPosition("f6")}, 0x212224a870df70a8ULL},
	GenerateAttacksTestParams{PieceType::QUEEN, Color::BLACK, Position{Utility::convertStringToPosition("g2")}, 0xe0bfe05048444241ULL},
	GenerateAttacksTestParams{PieceType::QUEEN, Color::BLACK, Position{Utility::convertStringToPosition("e8")}, 0x10101011925438efULL},
	GenerateAttacksTestParams{PieceType::QUEEN, Color::BLACK, Position{Utility::convertStringToPosition("a3")}, 0x503fe0305091121ULL},
	GenerateAttacksTestParams{PieceType::QUEEN, Color::BLACK, Position{Utility::convertStringToPosition("c5")}, 0x4424150efb0e1524ULL},
	GenerateAttacksTestParams{PieceType::QUEEN, Color::BLACK, Position{Utility::convertStringToPosition("e1")}, 0xef38549211101010ULL},
	GenerateAttacksTestParams{PieceType::QUEEN, Color::BLACK, Position{Utility::convertStringToPosition("g6")}, 0x42444850e0bfe050ULL},
	GenerateAttacksTestParams{PieceType::KING, Color::WHITE, Position{Utility::convertStringToPosition("h6")}, 0xc040c000ULL},
	GenerateAttacksTestParams{PieceType::KING, Color::WHITE, Position{Utility::convertStringToPosition("e2")}, 0x3828380000000000ULL},
	GenerateAttacksTestParams{PieceType::KING, Color::BLACK, Position{Utility::convertStringToPosition("a1")}, 0x203000000000000ULL},
	GenerateAttacksTestParams{PieceType::KING, Color::BLACK, Position{Utility::convertStringToPosition("b7")}, 0x70507ULL},
	GenerateAttacksTestParams{PieceType::KING, Color::BLACK, Position{Utility::convertStringToPosition("h4")}, 0xc040c0000000ULL},
	GenerateAttacksTestParams{PieceType::KING, Color::BLACK, Position{Utility::convertStringToPosition("d2")}, 0x1c141c0000000000ULL},
	GenerateAttacksTestParams{PieceType::KING, Color::BLACK, Position{Utility::convertStringToPosition("f8")}, 0x7050ULL},
	GenerateAttacksTestParams{PieceType::KING, Color::BLACK, Position{Utility::convertStringToPosition("c3")}, 0xe0a0e00000000ULL}
);

INSTANTIATE_TEST_SUITE_P(GenerateAttacksTests, GenerateAttacksTest, generateAttacksTestParams);