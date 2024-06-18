#include "gtest/gtest.h"

#include "../include/Board.hpp"

struct BoardConstructorTestParams
{
	std::string fenPosition;
	std::string fenEnPassantTargetSquare;
	uint64_t whitePawns;
	uint64_t blackPawns;
	uint64_t whiteKnights;
	uint64_t blackKnights;
	uint64_t whiteBishops;
	uint64_t blackBishops;
	uint64_t whiteRooks;
	uint64_t blackRooks;
	uint64_t whiteQueens;
	uint64_t blackQueens;
	uint64_t whiteKings;
	uint64_t blackKings;
	std::optional<Position> enPassantTargetSquare;
};

class BoardConstructorTest : public ::testing::TestWithParam<BoardConstructorTestParams> {};

TEST_P(BoardConstructorTest, BoardConstructor)
{
	auto params = GetParam();
	Board board(params.fenPosition, params.fenEnPassantTargetSquare);

	EXPECT_EQ(board.getPieceBitboard(PieceType::PAWN, Color::WHITE), Bitboard(params.whitePawns));
	EXPECT_EQ(board.getPieceBitboard(PieceType::PAWN, Color::BLACK), Bitboard(params.blackPawns));
	EXPECT_EQ(board.getPieceBitboard(PieceType::KNIGHT, Color::WHITE), Bitboard(params.whiteKnights));
	EXPECT_EQ(board.getPieceBitboard(PieceType::KNIGHT, Color::BLACK), Bitboard(params.blackKnights));
	EXPECT_EQ(board.getPieceBitboard(PieceType::BISHOP, Color::WHITE), Bitboard(params.whiteBishops));
	EXPECT_EQ(board.getPieceBitboard(PieceType::BISHOP, Color::BLACK), Bitboard(params.blackBishops));
	EXPECT_EQ(board.getPieceBitboard(PieceType::ROOK, Color::WHITE), Bitboard(params.whiteRooks));
	EXPECT_EQ(board.getPieceBitboard(PieceType::ROOK, Color::BLACK), Bitboard(params.blackRooks));
	EXPECT_EQ(board.getPieceBitboard(PieceType::QUEEN, Color::WHITE), Bitboard(params.whiteQueens));
	EXPECT_EQ(board.getPieceBitboard(PieceType::QUEEN, Color::BLACK), Bitboard(params.blackQueens));
	EXPECT_EQ(board.getPieceBitboard(PieceType::KING, Color::WHITE), Bitboard(params.whiteKings));
	EXPECT_EQ(board.getPieceBitboard(PieceType::KING, Color::BLACK), Bitboard(params.blackKings));

	EXPECT_EQ(board.getEnPassantTargetSquare(), params.enPassantTargetSquare);
}

const auto boardConstructorTestParams = ::testing::Values(
	BoardConstructorTestParams{
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",
		"-",
		0xff000000000000ULL,
		0xff00ULL,
		0x4200000000000000ULL,
		0x42ULL,
		0x2400000000000000ULL,
		0x24ULL,
		0x8100000000000000ULL,
		0x81ULL,
		0x800000000000000ULL,
		0x8ULL,
		0x1000000000000000ULL,
		0x10ULL,
		std::nullopt
	},
	BoardConstructorTestParams{
		"1k5B/3p2pq/Qp1bb1PP/p1n1N1P1/1pP3Pp/N2RKp1r/P1B1Pp1P/1r2nR2",
		"-",
		0x91004440c00000ULL,
		0x20208201024800ULL,
		0x10010000000ULL,
		0x1000000004000000ULL,
		0x4000000000080ULL,
		0x180000ULL,
		0x2000080000000000ULL,
		0x200800000000000ULL,
		0x10000ULL,
		0x8000ULL,
		0x100000000000ULL,
		0x2ULL,
		std::nullopt
	},
	BoardConstructorTestParams{
		"2bq2k1/1p2br2/2p1p2p/2P3p1/2PPQpP1/3B1N1P/5P2/R5K1",
		"g3",
		0x20804c04000000ULL,
		0x2040940200ULL,
		0x200000000000ULL,
		0x0ULL,
		0x80000000000ULL,
		0x1004ULL,
		0x100000000000000ULL,
		0x2000ULL,
		0x1000000000ULL,
		0x8ULL,
		0x4000000000000000ULL,
		0x40ULL,
		Position{5, 6}
	},
	BoardConstructorTestParams{
		"5r1k/1b6/p3p2n/4q1p1/1p3N2/1P1B3Q/P1P5/1K4R1",
		"-",
		0x5020000000000ULL,
		0x240110000ULL,
		0x2000000000ULL,
		0x800000ULL,
		0x80000000000ULL,
		0x200ULL,
		0x4000000000000000ULL,
		0x20ULL,
		0x800000000000ULL,
		0x10000000ULL,
		0x200000000000000ULL,
		0x80ULL,
		std::nullopt
	}
);

INSTANTIATE_TEST_CASE_P(Board, BoardConstructorTest, boardConstructorTestParams);