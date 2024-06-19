#include "gtest/gtest.h"

#include "../include/Board.hpp"
#include "../include/Utility.hpp"

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

class BoardConstructorTest : public ::testing::TestWithParam<BoardConstructorTestParams>
{
protected:
	bool verifyPieceListsPositionsWithBitboards(PieceList pieceList, Bitboard bitboard)
	{
		for (int square : pieceList.occupiedSquares)
		{
			Position position = Utility::calculatePosition(square);

			if (!bitboard.getBit(position))
			{
				return false;
			}
		}

		return true;
	}

	bool verifyKingPositionWithBitboard(int kingSquare, Bitboard bitboard)
	{
		Position position = Utility::calculatePosition(kingSquare);

		return bitboard.getBit(position);
	}
};

TEST_P(BoardConstructorTest, BoardConstructor)
{
	auto params = GetParam();
	Board board(params.fenPosition, params.fenEnPassantTargetSquare);

	// Bitboards
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

	// Piece lists - Compares the number of pieces in the piece lists with the number of set bits in the bitboards
	EXPECT_EQ(board.getPawns(Color::WHITE).occupiedSquares.size(), __builtin_popcountll(params.whitePawns));
	EXPECT_EQ(board.getPawns(Color::BLACK).occupiedSquares.size(), __builtin_popcountll(params.blackPawns));
	EXPECT_EQ(board.getKnights(Color::WHITE).occupiedSquares.size(), __builtin_popcountll(params.whiteKnights));
	EXPECT_EQ(board.getKnights(Color::BLACK).occupiedSquares.size(), __builtin_popcountll(params.blackKnights));
	EXPECT_EQ(board.getBishops(Color::WHITE).occupiedSquares.size(), __builtin_popcountll(params.whiteBishops));
	EXPECT_EQ(board.getBishops(Color::BLACK).occupiedSquares.size(), __builtin_popcountll(params.blackBishops));
	EXPECT_EQ(board.getRooks(Color::WHITE).occupiedSquares.size(), __builtin_popcountll(params.whiteRooks));
	EXPECT_EQ(board.getRooks(Color::BLACK).occupiedSquares.size(), __builtin_popcountll(params.blackRooks));
	EXPECT_EQ(board.getQueens(Color::WHITE).occupiedSquares.size(), __builtin_popcountll(params.whiteQueens));
	EXPECT_EQ(board.getQueens(Color::BLACK).occupiedSquares.size(), __builtin_popcountll(params.blackQueens));
	// skip kings because there is only one king per color

	// Piece lists - Compares the location of the pieces in the piece lists with the set bits in the bitboards
	EXPECT_TRUE(verifyPieceListsPositionsWithBitboards(board.getPawns(Color::WHITE), Bitboard(params.whitePawns)));
	EXPECT_TRUE(verifyPieceListsPositionsWithBitboards(board.getPawns(Color::BLACK), Bitboard(params.blackPawns)));
	EXPECT_TRUE(verifyPieceListsPositionsWithBitboards(board.getKnights(Color::WHITE), Bitboard(params.whiteKnights)));
	EXPECT_TRUE(verifyPieceListsPositionsWithBitboards(board.getKnights(Color::BLACK), Bitboard(params.blackKnights)));
	EXPECT_TRUE(verifyPieceListsPositionsWithBitboards(board.getBishops(Color::WHITE), Bitboard(params.whiteBishops)));
	EXPECT_TRUE(verifyPieceListsPositionsWithBitboards(board.getBishops(Color::BLACK), Bitboard(params.blackBishops)));
	EXPECT_TRUE(verifyPieceListsPositionsWithBitboards(board.getRooks(Color::WHITE), Bitboard(params.whiteRooks)));
	EXPECT_TRUE(verifyPieceListsPositionsWithBitboards(board.getRooks(Color::BLACK), Bitboard(params.blackRooks)));
	EXPECT_TRUE(verifyPieceListsPositionsWithBitboards(board.getQueens(Color::WHITE), Bitboard(params.whiteQueens)));
	EXPECT_TRUE(verifyPieceListsPositionsWithBitboards(board.getQueens(Color::BLACK), Bitboard(params.blackQueens)));

	// King positions
	EXPECT_TRUE(verifyKingPositionWithBitboard(board.getKing(Color::WHITE), Bitboard(params.whiteKings)));
	EXPECT_TRUE(verifyKingPositionWithBitboard(board.getKing(Color::BLACK), Bitboard(params.blackKings)));

	// En passant target square
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