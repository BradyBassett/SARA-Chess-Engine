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
	EXPECT_EQ(board.getPawns(Color::WHITE).count, __builtin_popcountll(params.whitePawns));
	EXPECT_EQ(board.getPawns(Color::BLACK).count, __builtin_popcountll(params.blackPawns));
	EXPECT_EQ(board.getKnights(Color::WHITE).count, __builtin_popcountll(params.whiteKnights));
	EXPECT_EQ(board.getKnights(Color::BLACK).count, __builtin_popcountll(params.blackKnights));
	EXPECT_EQ(board.getBishops(Color::WHITE).count, __builtin_popcountll(params.whiteBishops));
	EXPECT_EQ(board.getBishops(Color::BLACK).count, __builtin_popcountll(params.blackBishops));
	EXPECT_EQ(board.getRooks(Color::WHITE).count, __builtin_popcountll(params.whiteRooks));
	EXPECT_EQ(board.getRooks(Color::BLACK).count, __builtin_popcountll(params.blackRooks));
	EXPECT_EQ(board.getQueens(Color::WHITE).count, __builtin_popcountll(params.whiteQueens));
	EXPECT_EQ(board.getQueens(Color::BLACK).count, __builtin_popcountll(params.blackQueens));
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
		Position{Utility::convertStringToPosition("g3")}
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

struct BoardFenPartsParams
{
	std::string fenPosition;
	std::string enPassantTargetSquare;
};

class BoardFenPartsTest : public ::testing::TestWithParam<BoardFenPartsParams> {};

TEST_P(BoardFenPartsTest, BoardFenParts)
{
	auto params = GetParam();
	Board board(params.fenPosition, params.enPassantTargetSquare);

	EXPECT_EQ(board.getFenPosition(), params.fenPosition);
	EXPECT_EQ(board.getFenEnPassantTargetSquare(), params.enPassantTargetSquare);
}

const auto boardFenPartsParams = ::testing::Values(
	BoardFenPartsParams{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", "-"},
	BoardFenPartsParams{"1k5B/3p2pq/Qp1bb1PP/p1n1N1P1/1pP3Pp/N2RKp1r/P1B1Pp1P/1r2nR2", "-"},
	BoardFenPartsParams{"2bq2k1/1p2br2/2p1p2p/2P3p1/2PPQpP1/3B1N1P/5P2/R5K1", "g3"},
	BoardFenPartsParams{"5r1k/1b6/p3p2n/4q1p1/1p3N2/1P1B3Q/P1P5/1K4R1", "h3"},
	BoardFenPartsParams{"5r1k/1b6/p3p2n/4q1p1/1p3N2/1P1B3Q/P1P5/1K4R1", "-"}
);

INSTANTIATE_TEST_CASE_P(Board, BoardFenPartsTest, boardFenPartsParams);

struct BoardMovePieceParams
{
	std::string fenPosition;
	std::string fenEnPassantTargetSquare;
	Move move;
	std::string expectedFenPosition;
	std::string expectedFenEnPassantTargetSquare;
	std::array<int, 5> expectedWhitePieces;
	std::array<int, 5> expectedBlackPieces;
};

class BoardMovePieceTest : public ::testing::TestWithParam<BoardMovePieceParams> {};

TEST_P(BoardMovePieceTest, BoardMovePiece)
{
	auto params = GetParam();
	Board board(params.fenPosition, params.fenEnPassantTargetSquare);

	board.movePiece(params.move);

	// Validate fen position and en passant target square - Also validates bitboards because getFenPosition() uses the bitboards
	EXPECT_EQ(board.getFenPosition(), params.expectedFenPosition);
	EXPECT_EQ(board.getFenEnPassantTargetSquare(), params.expectedFenEnPassantTargetSquare);

	// Validate white piece counts
	EXPECT_EQ(board.getPawns(Color::WHITE).count, params.expectedWhitePieces[0]);
	EXPECT_EQ(board.getKnights(Color::WHITE).count, params.expectedWhitePieces[1]);
	EXPECT_EQ(board.getBishops(Color::WHITE).count, params.expectedWhitePieces[2]);
	EXPECT_EQ(board.getRooks(Color::WHITE).count, params.expectedWhitePieces[3]);
	EXPECT_EQ(board.getQueens(Color::WHITE).count, params.expectedWhitePieces[4]);

	// Validate black piece counts
	EXPECT_EQ(board.getPawns(Color::BLACK).count, params.expectedBlackPieces[0]);
	EXPECT_EQ(board.getKnights(Color::BLACK).count, params.expectedBlackPieces[1]);
	EXPECT_EQ(board.getBishops(Color::BLACK).count, params.expectedBlackPieces[2]);
	EXPECT_EQ(board.getRooks(Color::BLACK).count, params.expectedBlackPieces[3]);
	EXPECT_EQ(board.getQueens(Color::BLACK).count, params.expectedBlackPieces[4]);
}

const auto boardMovePieceParams = ::testing::Values(
	// Regular move
	BoardMovePieceParams{
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",
		"-",
		Move{Position{Utility::convertStringToPosition("e2")}, Position{Utility::convertStringToPosition("e3")}, PieceType::PAWN, Color::WHITE, std::nullopt, std::nullopt, SpecialMove::NONE, PromotionPiece::NONE, CastleRights{true, true}, CastleRights{true, true}, 0, 1},
		"rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR",
		"-",
		{8, 2, 2, 2, 1},
		{8, 2, 2, 2, 1}
	},
	BoardMovePieceParams{
		"rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR",
		"-",
		Move{Position{Utility::convertStringToPosition("g8")}, Position{Utility::convertStringToPosition("f6")}, PieceType::KNIGHT, Color::BLACK, std::nullopt, std::nullopt, SpecialMove::NONE, PromotionPiece::NONE, CastleRights{true, true}, CastleRights{true, true}, 0, 2},
		"rnbqkb1r/pppppppp/5n2/8/8/4P3/PPPP1PPP/RNBQKBNR",
		"-",
		{8, 2, 2, 2, 1},
		{8, 2, 2, 2, 1}
	},
	// Regular capture
	BoardMovePieceParams{
		"rnbqkb1r/pp1ppppp/5n2/2p5/3PP3/8/PPP2PPP/RNBQKBNR",
		"-",
		Move{Position{Utility::convertStringToPosition("f6")}, Position{Utility::convertStringToPosition("e4")}, PieceType::KNIGHT, Color::BLACK, PieceType::PAWN, std::nullopt, SpecialMove::NONE, PromotionPiece::NONE, CastleRights{true, true}, CastleRights{true, true}, 0, 3},
		"rnbqkb1r/pp1ppppp/8/2p5/3Pn3/8/PPP2PPP/RNBQKBNR",
		"-",
		{7, 2, 2, 2, 1},
		{8, 2, 2, 2, 1}
	},
	BoardMovePieceParams{
		"rnbqkb1r/pp1ppppp/8/2p5/3Pn3/8/PPP2PPP/RNBQKBNR",
		"-",
		Move{Position{Utility::convertStringToPosition("d4")}, Position{Utility::convertStringToPosition("c5")}, PieceType::PAWN, Color::WHITE, PieceType::PAWN, std::nullopt, SpecialMove::NONE, PromotionPiece::NONE, CastleRights{true, true}, CastleRights{true, true}, 0, 4},
		"rnbqkb1r/pp1ppppp/8/2P5/4n3/8/PPP2PPP/RNBQKBNR",
		"-",
		{7, 2, 2, 2, 1},
		{7, 2, 2, 2, 1}
	},
	// En passant
	BoardMovePieceParams{
		"rnbqkbnr/ppp2ppp/8/3Pp3/8/8/PPPP1PPP/RNBQKBNR",
		"e6",
		Move{Position{Utility::convertStringToPosition("d5")}, Position{Utility::convertStringToPosition("e6")}, PieceType::PAWN, Color::WHITE, PieceType::PAWN, Utility::convertStringToPosition("e6"), SpecialMove::EN_PASSANT, PromotionPiece::NONE, CastleRights{true, true}, CastleRights{true, true}, 0, 3},
		"rnbqkbnr/ppp2ppp/4P3/8/8/8/PPPP1PPP/RNBQKBNR",
		"-",
		{8, 2, 2, 2, 1},
		{6, 2, 2, 2, 1}
	},
	BoardMovePieceParams{
		"rnbqkbnr/ppp3pp/8/8/4pP1P/8/PPPP2P1/RNBQKBNR",
		"f3",
		Move{Position{Utility::convertStringToPosition("e4")}, Position{Utility::convertStringToPosition("f3")}, PieceType::PAWN, Color::BLACK, PieceType::PAWN, Utility::convertStringToPosition("f3"), SpecialMove::EN_PASSANT, PromotionPiece::NONE, CastleRights{true, true}, CastleRights{true, true}, 0, 6},
		"rnbqkbnr/ppp3pp/8/8/7P/5p2/PPPP2P1/RNBQKBNR",
		"-",
		{6, 2, 2, 2, 1},
		{6, 2, 2, 2, 1}
	},
	// Double pawn push
	BoardMovePieceParams{
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",
		"-",
		Move{Position{Utility::convertStringToPosition("e2")}, Position{Utility::convertStringToPosition("e4")}, PieceType::PAWN, Color::WHITE, std::nullopt, std::nullopt, SpecialMove::DOUBLE_PAWN_PUSH, PromotionPiece::NONE, CastleRights{true, true}, CastleRights{true, true}, 0, 1},
		"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR",
		"e3",
		{8, 2, 2, 2, 1},
		{8, 2, 2, 2, 1}
	},
	// Castling
	BoardMovePieceParams{
		"r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R",
		"-",
		Move{Position{Utility::convertStringToPosition("e1")}, Position{Utility::convertStringToPosition("g1")}, PieceType::KING, Color::WHITE, std::nullopt, std::nullopt, SpecialMove::KINGSIDE_CASTLE, PromotionPiece::NONE, CastleRights{true, true}, CastleRights{true, true}, 0, 1},
		"r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R4RK1",
		"-",
		{8, 0, 0, 2, 0},
		{8, 0, 0, 2, 0}
	},
	BoardMovePieceParams{
		"r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R",
		"-",
		Move{Position{Utility::convertStringToPosition("e1")}, Position{Utility::convertStringToPosition("c1")}, PieceType::KING, Color::WHITE, std::nullopt, std::nullopt, SpecialMove::QUEENSIDE_CASTLE, PromotionPiece::NONE, CastleRights{true, true}, CastleRights{true, true}, 0, 1},
		"r3k2r/pppppppp/8/8/8/8/PPPPPPPP/2KR3R",
		"-",
		{8, 0, 0, 2, 0},
		{8, 0, 0, 2, 0}
	},
	BoardMovePieceParams{
		"r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R",
		"-",
		Move{Position{Utility::convertStringToPosition("e8")}, Position{Utility::convertStringToPosition("g8")}, PieceType::KING, Color::BLACK, std::nullopt, std::nullopt, SpecialMove::KINGSIDE_CASTLE, PromotionPiece::NONE, CastleRights{true, true}, CastleRights{true, true}, 0, 1},
		"r4rk1/pppppppp/8/8/8/8/PPPPPPPP/R3K2R",
		"-",
		{8, 0, 0, 2, 0},
		{8, 0, 0, 2, 0}
	},
	BoardMovePieceParams{
		"r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R",
		"-",
		Move{Position{Utility::convertStringToPosition("e8")}, Position{Utility::convertStringToPosition("c8")}, PieceType::KING, Color::BLACK, std::nullopt, std::nullopt, SpecialMove::QUEENSIDE_CASTLE, PromotionPiece::NONE, CastleRights{true, true}, CastleRights{true, true}, 0, 1},
		"2kr3r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R",
		"-",
		{8, 0, 0, 2, 0},
		{8, 0, 0, 2, 0}
	},
	// Promotion
	BoardMovePieceParams{
		"r1bqkbnr/pPpppppp/6n1/8/8/6N1/PpPPPPPP/R1BQKBNR",
		"-",
		Move{Position{Utility::convertStringToPosition("b7")}, Position{Utility::convertStringToPosition("b8")}, PieceType::PAWN, Color::WHITE, std::nullopt, std::nullopt, SpecialMove::PROMOTION, PromotionPiece::KNIGHT, CastleRights{true, true}, CastleRights{true, true}, 0, 1},
		"rNbqkbnr/p1pppppp/6n1/8/8/6N1/PpPPPPPP/R1BQKBNR",
		"-",
		{7, 3, 2, 2, 1},
		{8, 2, 2, 2, 1}
	},
	BoardMovePieceParams{
		"r1bqkbnr/pPpppppp/6n1/8/8/6N1/PpPPPPPP/R1BQKBNR",
		"-",
		Move{Position{Utility::convertStringToPosition("b7")}, Position{Utility::convertStringToPosition("b8")}, PieceType::PAWN, Color::WHITE, std::nullopt, std::nullopt, SpecialMove::PROMOTION, PromotionPiece::BISHOP, CastleRights{true, true}, CastleRights{true, true}, 0, 1},
		"rBbqkbnr/p1pppppp/6n1/8/8/6N1/PpPPPPPP/R1BQKBNR",
		"-",
		{7, 2, 3, 2, 1},
		{8, 2, 2, 2, 1}
	},
	BoardMovePieceParams{
		"r1bqkbnr/pPpppppp/6n1/8/8/6N1/PpPPPPPP/R1BQKBNR",
		"-",
		Move{Position{Utility::convertStringToPosition("b2")}, Position{Utility::convertStringToPosition("b1")}, PieceType::PAWN, Color::BLACK, std::nullopt, std::nullopt, SpecialMove::PROMOTION, PromotionPiece::ROOK, CastleRights{true, true}, CastleRights{true, true}, 0, 1},
		"r1bqkbnr/pPpppppp/6n1/8/8/6N1/P1PPPPPP/RrBQKBNR",
		"-",
		{8, 2, 2, 2, 1},
		{7, 2, 2, 3, 1}
	},
	BoardMovePieceParams{
		"r1bqkbnr/pPpppppp/6n1/8/8/6N1/PpPPPPPP/R1BQKBNR",
		"-",
		Move{Position{Utility::convertStringToPosition("b2")}, Position{Utility::convertStringToPosition("b1")}, PieceType::PAWN, Color::BLACK, std::nullopt, std::nullopt, SpecialMove::PROMOTION, PromotionPiece::QUEEN, CastleRights{true, true}, CastleRights{true, true}, 0, 1},
		"r1bqkbnr/pPpppppp/6n1/8/8/6N1/P1PPPPPP/RqBQKBNR",
		"-",
		{8, 2, 2, 2, 1},
		{7, 2, 2, 2, 2}
	}
);

INSTANTIATE_TEST_CASE_P(Board, BoardMovePieceTest, boardMovePieceParams);

struct GetAttacksTestParams
{
	PieceType piece;
	Color color;
	int square;
	Bitboard expectedAttacks;
};

class GetAttacksTest : public ::testing::TestWithParam<GetAttacksTestParams> {};

TEST_P(GetAttacksTest, GetAttacks)
{
	auto params = GetParam();
	Board board("../src/data/whitePawnAttacks.json", "../src/data/blackPawnAttacks.json", "../src/data/knightAttacks.json", "../src/data/kingAttacks.json");
	Bitboard attacks = board.getAttacks(params.piece, params.color, params.square);
	EXPECT_EQ(attacks, params.expectedAttacks);
}

const auto getAttacksTestParams = ::testing::Values(
	GetAttacksTestParams{PieceType::PAWN, Color::WHITE, Utility::convertStringToSquareNumber("a1"), 0x2000000000000ULL},
	GetAttacksTestParams{PieceType::PAWN, Color::WHITE, Utility::convertStringToSquareNumber("h1"), 0x40000000000000ULL},
	GetAttacksTestParams{PieceType::PAWN, Color::WHITE, Utility::convertStringToSquareNumber("b8"), 0x0ULL},
	GetAttacksTestParams{PieceType::PAWN, Color::WHITE, Utility::convertStringToSquareNumber("a4"), 0x2000000ULL},
	GetAttacksTestParams{PieceType::PAWN, Color::WHITE, Utility::convertStringToSquareNumber("d6"), 0x1400ULL},
	GetAttacksTestParams{PieceType::PAWN, Color::WHITE, Utility::convertStringToSquareNumber("f7"), 0x50ULL},
	GetAttacksTestParams{PieceType::PAWN, Color::WHITE, Utility::convertStringToSquareNumber("g2"), 0xa00000000000ULL},
	GetAttacksTestParams{PieceType::PAWN, Color::WHITE, Utility::convertStringToSquareNumber("e6"), 0x2800ULL},
	GetAttacksTestParams{PieceType::PAWN, Color::BLACK, Utility::convertStringToSquareNumber("a8"), 0x200ULL},
	GetAttacksTestParams{PieceType::PAWN, Color::BLACK, Utility::convertStringToSquareNumber("h8"), 0x4000ULL},
	GetAttacksTestParams{PieceType::PAWN, Color::BLACK, Utility::convertStringToSquareNumber("b1"), 0x0ULL},
	GetAttacksTestParams{PieceType::PAWN, Color::BLACK, Utility::convertStringToSquareNumber("a4"), 0x20000000000ULL},
	GetAttacksTestParams{PieceType::PAWN, Color::BLACK, Utility::convertStringToSquareNumber("d6"), 0x14000000ULL},
	GetAttacksTestParams{PieceType::PAWN, Color::BLACK, Utility::convertStringToSquareNumber("f7"), 0x500000ULL},
	GetAttacksTestParams{PieceType::PAWN, Color::BLACK, Utility::convertStringToSquareNumber("g2"), 0xa000000000000000ULL},
	GetAttacksTestParams{PieceType::PAWN, Color::BLACK, Utility::convertStringToSquareNumber("e6"), 0x28000000ULL},
	GetAttacksTestParams{PieceType::KNIGHT, Color::WHITE, Utility::convertStringToSquareNumber("e4"), 0x28440044280000ULL},
	GetAttacksTestParams{PieceType::KNIGHT, Color::WHITE, Utility::convertStringToSquareNumber("c6"), 0xa1100110aULL},
	GetAttacksTestParams{PieceType::KNIGHT, Color::BLACK, Utility::convertStringToSquareNumber("f3"), 0x5088008850000000ULL},
	GetAttacksTestParams{PieceType::KNIGHT, Color::BLACK, Utility::convertStringToSquareNumber("d5"), 0x142200221400ULL},
	GetAttacksTestParams{PieceType::KNIGHT, Color::WHITE, Utility::convertStringToSquareNumber("b2"), 0x800080500000000ULL},
	GetAttacksTestParams{PieceType::KNIGHT, Color::WHITE, Utility::convertStringToSquareNumber("h8"), 0x402000ULL},
	GetAttacksTestParams{PieceType::KNIGHT, Color::BLACK, Utility::convertStringToSquareNumber("a1"), 0x4020000000000ULL},
	GetAttacksTestParams{PieceType::KNIGHT, Color::BLACK, Utility::convertStringToSquareNumber("g7"), 0xa0100010ULL},
	GetAttacksTestParams{PieceType::KING, Color::WHITE, Utility::convertStringToSquareNumber("h6"), 0xc040c000ULL},
	GetAttacksTestParams{PieceType::KING, Color::WHITE, Utility::convertStringToSquareNumber("e2"), 0x3828380000000000ULL},
	GetAttacksTestParams{PieceType::KING, Color::BLACK, Utility::convertStringToSquareNumber("a1"), 0x203000000000000ULL},
	GetAttacksTestParams{PieceType::KING, Color::BLACK, Utility::convertStringToSquareNumber("b7"), 0x70507ULL},
	GetAttacksTestParams{PieceType::KING, Color::BLACK, Utility::convertStringToSquareNumber("h4"), 0xc040c0000000ULL},
	GetAttacksTestParams{PieceType::KING, Color::BLACK, Utility::convertStringToSquareNumber("d2"), 0x1c141c0000000000ULL},
	GetAttacksTestParams{PieceType::KING, Color::BLACK, Utility::convertStringToSquareNumber("f8"), 0x7050ULL},
	GetAttacksTestParams{PieceType::KING, Color::BLACK, Utility::convertStringToSquareNumber("c3"), 0xe0a0e00000000ULL}
	// Sliding pieces are tested in the MagicBitboardsTest
);

INSTANTIATE_TEST_SUITE_P(GetAttacksTests, GetAttacksTest, getAttacksTestParams);