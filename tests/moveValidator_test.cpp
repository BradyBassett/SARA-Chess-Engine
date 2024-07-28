#include <gtest/gtest.h>

#include "../include/MoveValidator.hpp"
#include "../include/Utility.hpp"

struct ValidValidateMoveTestParams
{
	std::string fen;
	Position from;
	Position to;
	PieceType piece;
	Color color;
};

class validValidateMoveTest : public ::testing::TestWithParam<ValidValidateMoveTestParams> {};

TEST_P(validValidateMoveTest, test)
{
	auto params = GetParam();
	Game game(params.fen);
	EXPECT_NO_THROW(MoveValidator::validateMove(params.from, params.to, params.piece, params.color, game));
}

const auto validValidateMoveTestParams = ::testing::Values(
	// Valid pawn moves
	ValidValidateMoveTestParams{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", Position{Utility::convertStringToPosition("e2")}, Position{Utility::convertStringToPosition("e3")}, PieceType::PAWN, Color::WHITE},
	ValidValidateMoveTestParams{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", Position{Utility::convertStringToPosition("e2")}, Position{Utility::convertStringToPosition("e4")}, PieceType::PAWN, Color::WHITE},
	ValidValidateMoveTestParams{"rnbqkbnr/ppp2ppp/3p4/4p3/4P3/3P4/PPP2PPP/RNBQKBNR w KQkq - 0 3", Position{Utility::convertStringToPosition("d3")}, Position{Utility::convertStringToPosition("d4")}, PieceType::PAWN, Color::WHITE},
	ValidValidateMoveTestParams{"rnbqkbnr/ppp2ppp/8/3pp3/3PP3/8/PPP2PPP/RNBQKBNR w KQkq - 0 4", Position{Utility::convertStringToPosition("d4")}, Position{Utility::convertStringToPosition("e5")}, PieceType::PAWN, Color::WHITE},
	ValidValidateMoveTestParams{"rnbqkbnr/ppp3pp/8/4Pp2/4p3/2P5/PP3PPP/RNBQKBNR w KQkq f6 0 6", Position{Utility::convertStringToPosition("e5")}, Position{Utility::convertStringToPosition("f6")}, PieceType::PAWN, Color::WHITE},
	ValidValidateMoveTestParams{"3K4/8/8/8/3P4/8/8/3r3k w - - 0 1", Position{Utility::convertStringToPosition("d4")}, Position{Utility::convertStringToPosition("d5")}, PieceType::PAWN, Color::WHITE},
	ValidValidateMoveTestParams{"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1", Position{Utility::convertStringToPosition("e7")}, Position{Utility::convertStringToPosition("e6")}, PieceType::PAWN, Color::BLACK},
	ValidValidateMoveTestParams{"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1", Position{Utility::convertStringToPosition("e7")}, Position{Utility::convertStringToPosition("e5")}, PieceType::PAWN, Color::BLACK},
	ValidValidateMoveTestParams{"rnbqkbnr/ppp2ppp/3p4/4p3/3PP3/8/PPP2PPP/RNBQKBNR b KQkq - 0 3", Position{Utility::convertStringToPosition("d6")}, Position{Utility::convertStringToPosition("d5")}, PieceType::PAWN, Color::BLACK},
	ValidValidateMoveTestParams{"rnbqkbnr/ppp2ppp/8/3pP3/4P3/8/PPP2PPP/RNBQKBNR b KQkq - 0 4", Position{Utility::convertStringToPosition("d5")}, Position{Utility::convertStringToPosition("e4")}, PieceType::PAWN, Color::BLACK},
	ValidValidateMoveTestParams{"rnbqkbnr/pp4pp/2p2P2/8/4pP2/2P5/PP4PP/RNBQKBNR b KQkq f3 0 7", Position{Utility::convertStringToPosition("e4")}, Position{Utility::convertStringToPosition("f3")}, PieceType::PAWN, Color::BLACK},
	ValidValidateMoveTestParams{"3R3K/8/8/3p4/8/8/8/3k4 b - - 0 1", Position{Utility::convertStringToPosition("d5")}, Position{Utility::convertStringToPosition("d4")}, PieceType::PAWN, Color::BLACK},
	// Valid knight moves
	ValidValidateMoveTestParams{"rnbqkbnr/ppp3pp/8/4Pp2/4p3/2P5/PP3PPP/RNBQKBNR w KQkq f6 0 6", Position{Utility::convertStringToPosition("g1")}, Position{Utility::convertStringToPosition("f3")}, PieceType::KNIGHT, Color::WHITE},
	ValidValidateMoveTestParams{"rnb2b1r/ppp3pp/5nk1/3QPp2/3qp3/2P2N2/PP3PPP/RNB1KB1R w KQ - 8 10", Position{Utility::convertStringToPosition("f3")}, Position{Utility::convertStringToPosition("d4")}, PieceType::KNIGHT, Color::WHITE},
	ValidValidateMoveTestParams{"rnbqkbnr/ppp3pp/8/4Pp2/4p3/2P2N2/PP3PPP/RNBQKB1R b KQkq - 1 6", Position{Utility::convertStringToPosition("g8")}, Position{Utility::convertStringToPosition("f6")}, PieceType::KNIGHT, Color::BLACK},
	ValidValidateMoveTestParams{"rnb2b1r/ppp3pp/5nk1/3QPp2/3Np3/2P5/PP3PPP/RNB1KB1R b KQ - 0 10", Position{Utility::convertStringToPosition("f6")}, Position{Utility::convertStringToPosition("d5")}, PieceType::KNIGHT, Color::BLACK},
	// Valid bishop moves
	ValidValidateMoveTestParams{"rnb2b1r/ppp3pp/6k1/3nPp2/3Np3/2P5/PP3PPP/RNB1KB1R w KQ - 0 11", Position{Utility::convertStringToPosition("f1")}, Position{Utility::convertStringToPosition("c4")}, PieceType::BISHOP, Color::WHITE},
	ValidValidateMoveTestParams{"rnb4r/ppp3pp/6k1/2bnPp2/2BNp3/2P5/PP3PPP/RNB1K2R w KQ - 2 12", Position{Utility::convertStringToPosition("c4")}, Position{Utility::convertStringToPosition("d5")}, PieceType::BISHOP, Color::WHITE},
	ValidValidateMoveTestParams{"K7/8/2B5/8/8/5b2/8/7k w - - 0 1", Position{Utility::convertStringToPosition("c6")}, Position{Utility::convertStringToPosition("e4")}, PieceType::BISHOP, Color::WHITE},
	ValidValidateMoveTestParams{"K7/8/2B5/8/8/5b2/8/7k w - - 0 1", Position{Utility::convertStringToPosition("c6")}, Position{Utility::convertStringToPosition("f3")}, PieceType::BISHOP, Color::WHITE},
	ValidValidateMoveTestParams{"rnb2b1r/ppp3pp/6k1/3nPp2/2BNp3/2P5/PP3PPP/RNB1K2R b KQ - 1 11", Position{Utility::convertStringToPosition("f8")}, Position{Utility::convertStringToPosition("c5")}, PieceType::BISHOP, Color::BLACK},
	ValidValidateMoveTestParams{"rnb4r/ppp3pp/6k1/2bBPp2/3Np3/2P5/PP3PPP/RNB1K2R b KQ - 0 12", Position{Utility::convertStringToPosition("c5")}, Position{Utility::convertStringToPosition("d4")}, PieceType::BISHOP, Color::BLACK},
	ValidValidateMoveTestParams{"K7/8/2B5/8/8/5b2/8/7k b - - 0 1", Position{Utility::convertStringToPosition("f3")}, Position{Utility::convertStringToPosition("d5")}, PieceType::BISHOP, Color::BLACK},
	ValidValidateMoveTestParams{"K7/8/2B5/8/8/5b2/8/7k b - - 0 1", Position{Utility::convertStringToPosition("f3")}, Position{Utility::convertStringToPosition("c6")}, PieceType::BISHOP, Color::BLACK},
	// Valid rook moves
	ValidValidateMoveTestParams{"1nb4r/2p3pp/r1B3k1/pp2Pp2/P3p3/R1b5/1P1B1PPP/1N2K2R w K b6 0 17", Position{Utility::convertStringToPosition("a3")}, Position{Utility::convertStringToPosition("b3")}, PieceType::ROOK, Color::WHITE},
	ValidValidateMoveTestParams{"1nb4r/2p3pp/1rB3k1/pp2Pp2/P3p3/1Rb5/1P1B1PPP/1N2K2R w K - 2 18", Position{Utility::convertStringToPosition("b3")}, Position{Utility::convertStringToPosition("c3")}, PieceType::ROOK, Color::WHITE},
	ValidValidateMoveTestParams{"7K/7R/8/8/8/8/7r/7k w - - 0 1", Position{Utility::convertStringToPosition("h7")}, Position{Utility::convertStringToPosition("h5")}, PieceType::ROOK, Color::WHITE},
	ValidValidateMoveTestParams{"7K/7R/8/8/8/8/7r/7k w - - 0 1", Position{Utility::convertStringToPosition("h7")}, Position{Utility::convertStringToPosition("h2")}, PieceType::ROOK, Color::WHITE},
	ValidValidateMoveTestParams{"1nb4r/2p3pp/r1B3k1/pp2Pp2/P3p3/1Rb5/1P1B1PPP/1N2K2R b K - 1 17", Position{Utility::convertStringToPosition("a6")}, Position{Utility::convertStringToPosition("b6")}, PieceType::ROOK, Color::BLACK},
	ValidValidateMoveTestParams{"1nb4r/2p3pp/1rB3k1/pp2Pp2/P3p3/2R5/1P1B1PPP/1N2K2R b K - 0 18", Position{Utility::convertStringToPosition("b6")}, Position{Utility::convertStringToPosition("c6")}, PieceType::ROOK, Color::BLACK},
	ValidValidateMoveTestParams{"7K/7R/8/8/8/8/7r/7k b - - 0 1", Position{Utility::convertStringToPosition("h2")}, Position{Utility::convertStringToPosition("h5")}, PieceType::ROOK, Color::BLACK},
	ValidValidateMoveTestParams{"7K/7R/8/8/8/8/7r/7k b - - 0 1", Position{Utility::convertStringToPosition("h2")}, Position{Utility::convertStringToPosition("h7")}, PieceType::ROOK, Color::BLACK},
	// Valid queen moves
	ValidValidateMoveTestParams{"rnbqkb1r/ppp3pp/5n2/4Pp2/4p3/2P2N2/PP3PPP/RNBQKB1R w KQkq - 2 7", Position{Utility::convertStringToPosition("d1")}, Position{Utility::convertStringToPosition("a4")}, PieceType::QUEEN, Color::WHITE},
	ValidValidateMoveTestParams{"rnb2b1r/pp3kpp/2p2n2/q3Pp2/Q3p3/2P2NP1/PP2KP1P/RNB2B1R w - - 1 10", Position{Utility::convertStringToPosition("a4")}, Position{Utility::convertStringToPosition("d4")}, PieceType::QUEEN, Color::WHITE},
	ValidValidateMoveTestParams{"rnb2b1r/pp3kpp/2p2n2/3qPp2/3Qp3/2P2NP1/PP2KP1P/RNB2B1R w - - 3 11", Position{Utility::convertStringToPosition("d4")}, Position{Utility::convertStringToPosition("e4")}, PieceType::QUEEN, Color::WHITE},
	ValidValidateMoveTestParams{"rnbq1b1r/pp3kpp/2p2n2/4Pp2/Q3p3/2P2NP1/PP2KP1P/RNB2B1R b - - 0 9", Position{Utility::convertStringToPosition("d8")}, Position{Utility::convertStringToPosition("a5")}, PieceType::QUEEN, Color::BLACK},
	ValidValidateMoveTestParams{"rnb2b1r/pp3kpp/2p2n2/q3Pp2/3Qp3/2P2NP1/PP2KP1P/RNB2B1R b - - 2 10", Position{Utility::convertStringToPosition("a5")}, Position{Utility::convertStringToPosition("d5")}, PieceType::QUEEN, Color::BLACK},
	ValidValidateMoveTestParams{"rnb2b1r/pp3kpp/2p2n2/3qPp2/4Q3/2P2NP1/PP2KP1P/RNB2B1R b - - 0 11", Position{Utility::convertStringToPosition("d5")}, Position{Utility::convertStringToPosition("e5")}, PieceType::QUEEN, Color::BLACK},
	// Valid king moves
	ValidValidateMoveTestParams{"r3k2r/pppbN1pp/2nq4/4P3/3Qpp2/N1PB4/PP1B1nPP/R3K2R w KQkq - 0 14", Position{Utility::convertStringToPosition("e1")}, Position{Utility::convertStringToPosition("e2")}, PieceType::KING, Color::WHITE},
	ValidValidateMoveTestParams{"r3k2r/pppbN1pp/2nq4/4P3/3Qpp2/N1PB4/PP1B1nPP/R3K2R w KQkq - 0 14", Position{Utility::convertStringToPosition("e1")}, Position{Utility::convertStringToPosition("f2")}, PieceType::KING, Color::WHITE},
	ValidValidateMoveTestParams{"r3k2r/pppbN1pp/2nq4/4P3/3Qpp2/N1PB4/PP1B1nPP/R3K2R w KQkq - 0 14", Position{Utility::convertStringToPosition("e1")}, Position{Utility::convertStringToPosition("g1")}, PieceType::KING, Color::WHITE},
	ValidValidateMoveTestParams{"r3k2r/pppbN1pp/2nq4/4P3/3Qppn1/N1PB2P1/PP1B3P/R3K2R w KQkq - 1 15", Position{Utility::convertStringToPosition("e1")}, Position{Utility::convertStringToPosition("c1")}, PieceType::KING, Color::WHITE},
	ValidValidateMoveTestParams{"r3k2r/pppbN1pp/2nq4/4P3/3Qpp2/N1PB4/PP1BKnPP/R6R b kq - 1 14", Position{Utility::convertStringToPosition("e8")}, Position{Utility::convertStringToPosition("f7")}, PieceType::KING, Color::BLACK},
	ValidValidateMoveTestParams{"r3k2r/pppbN1pp/2nq4/4P3/3Qpp2/N1PB4/PP1BKnPP/R6R b kq - 1 14", Position{Utility::convertStringToPosition("e8")}, Position{Utility::convertStringToPosition("e7")}, PieceType::KING, Color::BLACK},
	ValidValidateMoveTestParams{"r3k2r/pppb2pp/2nq4/3NP3/3Qp3/N1PB1p2/PP1B1nPP/R4RK1 b kq - 1 15", Position{Utility::convertStringToPosition("e8")}, Position{Utility::convertStringToPosition("g8")}, PieceType::KING, Color::BLACK},
	ValidValidateMoveTestParams{"r3k2r/pppb2pp/2nq4/4PN2/3Q1pn1/N1PBp1P1/PP1B3P/2KR3R b kq - 1 16", Position{Utility::convertStringToPosition("e8")}, Position{Utility::convertStringToPosition("c8")}, PieceType::KING, Color::BLACK}
);

INSTANTIATE_TEST_SUITE_P(validValidateMoveTest, validValidateMoveTest, validValidateMoveTestParams);

struct InvalidValidateMoveTestParams
{
	std::string fen;
	Position from;
	Position to;
	PieceType piece;
	Color color;
	std::string expectedError;
};

class invalidValidateMoveTest : public ::testing::TestWithParam<InvalidValidateMoveTestParams> {};

TEST_P(invalidValidateMoveTest, test)
{
	auto params = GetParam();
	Game game(params.fen);
	EXPECT_THROW({
		try
		{
			MoveValidator::validateMove(params.from, params.to, params.piece, params.color, game);
		}
		catch (const std::invalid_argument &e)
		{
			EXPECT_STREQ(e.what(), params.expectedError.c_str());
			throw;
		}
	}, std::invalid_argument);
}

const auto invalidValidateMoveTestParams = ::testing::Values(
	// Invalid general moves (applicable to all pieces)
	InvalidValidateMoveTestParams{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", Position{Utility::convertStringToPosition("e7")}, Position{Utility::convertStringToPosition("e6")}, PieceType::PAWN, Color::BLACK, "Invalid move - You can only move your own pieces"},
	InvalidValidateMoveTestParams{"rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR b KQkq - 0 1", Position{Utility::convertStringToPosition("d2")}, Position{Utility::convertStringToPosition("d4")}, PieceType::PAWN, Color::WHITE, "Invalid move - You can only move your own pieces"},
	InvalidValidateMoveTestParams{"4N3/3pP3/1P1P4/5r2/2PKPp2/2pp4/1P4k1/1b2n3 w - - 0 1", Position{Utility::convertStringToPosition("e4")}, Position{Utility::convertStringToPosition("e4")}, PieceType::PAWN, Color::WHITE, "Invalid move - You must move the piece to a different square"},
	InvalidValidateMoveTestParams{"4N3/3pP3/1P1P4/5r2/2PKPp2/2pp4/1P4k1/1b2n3 w - - 0 1", Position{Utility::convertStringToPosition("a1")}, Position{Utility::convertStringToPosition("a2")}, PieceType::QUEEN, Color::WHITE, "Invalid move - There is no piece on this square"},
	InvalidValidateMoveTestParams{"1n6/1kN4n/1p5P/Pp2p2P/2Q2b2/8/PN6/3b1K2 b - - 3 2", Position{Utility::convertStringToPosition("f4")}, Position{Utility::convertStringToPosition("e5")}, PieceType::BISHOP, Color::BLACK, "Invalid move - You cannot capture your own piece"},
	InvalidValidateMoveTestParams{"3k4/8/8/8/3b4/8/1P6/K7 w - - 0 1", Position{Utility::convertStringToPosition("b2")}, Position{Utility::convertStringToPosition("b3")}, PieceType::PAWN, Color::WHITE, "Invalid move - The piece is pinned and cannot move in that direction"},
	InvalidValidateMoveTestParams{"3k4/8/r7/8/8/N7/8/K7 w - - 0 1", Position{Utility::convertStringToPosition("a3")}, Position{Utility::convertStringToPosition("c4")}, PieceType::KNIGHT, Color::WHITE, "Invalid move - The piece is pinned and cannot move in that direction"},
	InvalidValidateMoveTestParams{"4k3/8/2q5/3R4/8/5K2/8/8 w - - 0 1", Position{Utility::convertStringToPosition("d5")}, Position{Utility::convertStringToPosition("d8")}, PieceType::ROOK, Color::WHITE, "Invalid move - The piece is pinned and cannot move in that direction"},
	InvalidValidateMoveTestParams{"4k3/8/8/8/8/2qP1K2/8/8 w - - 0 1", Position{Utility::convertStringToPosition("d3")}, Position{Utility::convertStringToPosition("d4")}, PieceType::PAWN, Color::WHITE, "Invalid move - The piece is pinned and cannot move in that direction"},
	InvalidValidateMoveTestParams{"4k3/3p4/8/1B6/8/8/8/K7 b - - 0 1", Position{Utility::convertStringToPosition("d7")}, Position{Utility::convertStringToPosition("d6")}, PieceType::PAWN, Color::BLACK, "Invalid move - The piece is pinned and cannot move in that direction"},
	InvalidValidateMoveTestParams{"4k3/8/4b3/8/8/4R3/8/K7 b - - 0 1", Position{Utility::convertStringToPosition("e6")}, Position{Utility::convertStringToPosition("f5")}, PieceType::BISHOP, Color::BLACK, "Invalid move - The piece is pinned and cannot move in that direction"},
	InvalidValidateMoveTestParams{"7k/8/8/8/3n4/5K2/8/Q7 b - - 0 1", Position{Utility::convertStringToPosition("d4")}, Position{Utility::convertStringToPosition("e2")}, PieceType::KNIGHT, Color::BLACK, "Invalid move - The piece is pinned and cannot move in that direction"},
	InvalidValidateMoveTestParams{"Q2n3k/8/8/8/8/5K2/8/8 b - - 0 1", Position{Utility::convertStringToPosition("d8")}, Position{Utility::convertStringToPosition("f7")}, PieceType::KNIGHT, Color::BLACK, "Invalid move - The piece is pinned and cannot move in that direction"},
	// Invalid pawn moves
	InvalidValidateMoveTestParams{"rnbqkbnr/pppppppp/8/8/8/2P5/P1P3PP/4KBNR w kq - 0 1", Position{Utility::convertStringToPosition("c2")}, Position{Utility::convertStringToPosition("e5")}, PieceType::PAWN, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"rnbqkbnr/pppppppp/8/8/8/2P5/P1P3PP/4KBNR w kq - 0 1", Position{Utility::convertStringToPosition("c2")}, Position{Utility::convertStringToPosition("c1")}, PieceType::PAWN, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"rnbqkbnr/pppppppp/8/8/8/2P5/P1P3PP/4KBNR w kq - 0 1", Position{Utility::convertStringToPosition("c2")}, Position{Utility::convertStringToPosition("d1")}, PieceType::PAWN, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"rnbqkbnr/pppppppp/8/8/8/2P5/P1P3PP/4KBNR w kq - 0 1", Position{Utility::convertStringToPosition("c2")}, Position{Utility::convertStringToPosition("c4")}, PieceType::PAWN, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"rnbqkbnr/pppppppp/8/8/8/2P5/P1P3PP/4KBNR w kq - 0 1", Position{Utility::convertStringToPosition("c2")}, Position{Utility::convertStringToPosition("d3")}, PieceType::PAWN, Color::WHITE, "Invalid move - To move diagonally, you must capture an enemy piece"},
	InvalidValidateMoveTestParams{"rnbqkbnr/pppppppp/8/8/8/2P5/P1P3PP/4KBNR w kq - 0 1", Position{Utility::convertStringToPosition("c2")}, Position{Utility::convertStringToPosition("b3")}, PieceType::PAWN, Color::WHITE, "Invalid move - To move diagonally, you must capture an enemy piece"},
	InvalidValidateMoveTestParams{"rnbqkbnr/pp1ppppp/8/2pP4/8/8/PPP1PPPP/RNBQKBNR w KQkq c6 0 2", Position{Utility::convertStringToPosition("d5")}, Position{Utility::convertStringToPosition("e6")}, PieceType::PAWN, Color::WHITE, "Invalid move - Pawn can only capture en passant if the opponents previous move was a pawn push of 2 squares that landed adjacent to this pawn"},
	InvalidValidateMoveTestParams{"rnbqkbnr/pppp1ppp/8/3Pp3/8/7P/PPP1PPP1/RNBQKBNR w KQkq - 0 3", Position{Utility::convertStringToPosition("d5")}, Position{Utility::convertStringToPosition("e6")}, PieceType::PAWN, Color::WHITE, "Invalid move - Pawn can only capture en passant if the opponents previous move was a pawn push of 2 squares that landed adjacent to this pawn"},
	InvalidValidateMoveTestParams{"rnbqkbnr/ppp1pppp/8/8/8/3p4/PPPPPPPP/RNBQKBNR w KQkq - 0 1", Position{Utility::convertStringToPosition("d2")}, Position{Utility::convertStringToPosition("d3")}, PieceType::PAWN, Color::WHITE, "Invalid move - You cannot capture a piece by moving forward"},
	InvalidValidateMoveTestParams{"r3kbnr/p1p1pppp/2p5/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", Position{Utility::convertStringToPosition("a7")}, Position{Utility::convertStringToPosition("a4")}, PieceType::PAWN, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"r3kbnr/p1p1pppp/2p5/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", Position{Utility::convertStringToPosition("c7")}, Position{Utility::convertStringToPosition("c8")}, PieceType::PAWN, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"r3kbnr/p1p1pppp/2p5/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", Position{Utility::convertStringToPosition("c7")}, Position{Utility::convertStringToPosition("d8")}, PieceType::PAWN, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"r3kbnr/p1p1pppp/2p5/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", Position{Utility::convertStringToPosition("c7")}, Position{Utility::convertStringToPosition("c5")}, PieceType::PAWN, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"r3kbnr/p1p1pppp/2p5/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", Position{Utility::convertStringToPosition("c7")}, Position{Utility::convertStringToPosition("d6")}, PieceType::PAWN, Color::BLACK, "Invalid move - To move diagonally, you must capture an enemy piece"},
	InvalidValidateMoveTestParams{"r3kbnr/p1p1pppp/2p5/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", Position{Utility::convertStringToPosition("c7")}, Position{Utility::convertStringToPosition("b6")}, PieceType::PAWN, Color::BLACK, "Invalid move - To move diagonally, you must capture an enemy piece"},
	InvalidValidateMoveTestParams{"rnbqkbnr/pppp2pp/8/8/3Pp3/8/PPP2PPP/RNBQKBNR b KQkq d3 0 1", Position{Utility::convertStringToPosition("e4")}, Position{Utility::convertStringToPosition("f3")}, PieceType::PAWN, Color::BLACK, "Invalid move - Pawn can only capture en passant if the opponents previous move was a pawn push of 2 squares that landed adjacent to this pawn"},
	InvalidValidateMoveTestParams{"rnbqkbnr/pppp1p1p/6p1/8/4pP2/8/PPPPP1PP/RNBQKBNR b KQkq - 0 2", Position{Utility::convertStringToPosition("e4")}, Position{Utility::convertStringToPosition("f3")}, PieceType::PAWN, Color::BLACK, "Invalid move - Pawn can only capture en passant if the opponents previous move was a pawn push of 2 squares that landed adjacent to this pawn"},
	InvalidValidateMoveTestParams{"rnbqkbnr/pppppppp/3P4/8/8/8/PPP1PPPP/RNBQKBNR b KQkq - 0 1", Position{Utility::convertStringToPosition("d7")}, Position{Utility::convertStringToPosition("d6")}, PieceType::PAWN, Color::BLACK, "Invalid move - You cannot capture a piece by moving forward"},
	// Invalid knight moves
	InvalidValidateMoveTestParams{"k3r3/5P2/3p3P/2P3b1/PR4P1/1K4N1/2P3Pp/1b6 w - - 0 1", Position{Utility::convertStringToPosition("g3")}, Position{Utility::convertStringToPosition("e5")}, PieceType::KNIGHT, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"2k3K1/3p4/1n4N1/p4p2/2p1R3/1QP1Bp2/pPb5/8 w - - 0 1", Position{Utility::convertStringToPosition("g6")}, Position{Utility::convertStringToPosition("f7")}, PieceType::KNIGHT, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"2k3K1/3p4/1n4N1/p4p2/2p1R3/1QP1Bp2/pPb5/8 b - - 0 1", Position{Utility::convertStringToPosition("b6")}, Position{Utility::convertStringToPosition("e4")}, PieceType::KNIGHT, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"8/p1N2r2/K7/1n1qRpB1/p5k1/4p2b/4P1n1/4R3 b - - 0 1", Position{Utility::convertStringToPosition("g2")}, Position{Utility::convertStringToPosition("a3")}, PieceType::KNIGHT, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	// Invalid bishop moves
	InvalidValidateMoveTestParams{"8/p1N2r2/K7/1n1qRpB1/p5k1/4p2b/4P1n1/4R3 w - - 0 1", Position{Utility::convertStringToPosition("g5")}, Position{Utility::convertStringToPosition("g6")}, PieceType::BISHOP, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"8/p1N2r2/K7/1n1qRpB1/p5k1/4p2b/4P1n1/4R3 w - - 0 1", Position{Utility::convertStringToPosition("g5")}, Position{Utility::convertStringToPosition("f3")}, PieceType::BISHOP, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"8/p1N2r2/K7/1n1qRpB1/p5k1/4p2b/4P1n1/4R3 b - - 0 1", Position{Utility::convertStringToPosition("h3")}, Position{Utility::convertStringToPosition("h8")}, PieceType::BISHOP, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"8/p1N2r2/K7/1n1qRpB1/p5k1/4p2b/4P1n1/4R3 b - - 0 1", Position{Utility::convertStringToPosition("h3")}, Position{Utility::convertStringToPosition("g1")}, PieceType::BISHOP, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"rnbqk1nr/ppp1pp2/6p1/3p1b2/1P2B2p/5P2/PPPPP2P/RNBQK1NR w KQkq - 0 1", Position{Utility::convertStringToPosition("e4")}, Position{Utility::convertStringToPosition("c6")}, PieceType::BISHOP, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"rnbqk1nr/ppp1pp2/6p1/3p1b2/1P2B2p/5P2/PPPPP2P/RNBQK1NR w KQkq - 0 1", Position{Utility::convertStringToPosition("e4")}, Position{Utility::convertStringToPosition("g2")}, PieceType::BISHOP, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"rnbqk1nr/ppp1pp2/6p1/3p1b2/1P2B2p/5P2/PPPPP2P/RNBQK1NR b KQkq - 0 1", Position{Utility::convertStringToPosition("f5")}, Position{Utility::convertStringToPosition("d3")}, PieceType::BISHOP, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"rnbqk1nr/ppp1pp2/6p1/3p1b2/1P2B2p/5P2/PPPPP2P/RNBQK1NR b KQkq - 0 1", Position{Utility::convertStringToPosition("f5")}, Position{Utility::convertStringToPosition("h7")}, PieceType::BISHOP, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	// Invalid rook moves
	InvalidValidateMoveTestParams{"8/n2P4/8/2kP4/r3pBpr/P5Kb/6pp/Q3R3 w - - 0 1", Position{Utility::convertStringToPosition("e1")}, Position{Utility::convertStringToPosition("c3")}, PieceType::ROOK, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"8/n2P4/8/2kP4/r3pBpr/P5Kb/6pp/Q3R3 w - - 0 1", Position{Utility::convertStringToPosition("e1")}, Position{Utility::convertStringToPosition("f3")}, PieceType::ROOK, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"8/n2P4/8/2kP4/r3pBpr/P5Kb/6pp/Q3R3 b - - 0 1", Position{Utility::convertStringToPosition("a4")}, Position{Utility::convertStringToPosition("d1")}, PieceType::ROOK, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"8/n2P4/8/2kP4/r3pBpr/P5Kb/6pp/Q3R3 b - - 0 1", Position{Utility::convertStringToPosition("h4")}, Position{Utility::convertStringToPosition("f5")}, PieceType::ROOK, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"1nbqkbnr/ppp1pppp/3p4/3rR3/4P3/8/PPPP1PPP/RNBQKBN1 w Qk - 0 1", Position{Utility::convertStringToPosition("e5")}, Position{Utility::convertStringToPosition("e3")}, PieceType::ROOK, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"1nbqkbnr/ppp1pppp/3p4/3rR3/4P3/8/PPPP1PPP/RNBQKBN1 w Qk - 0 1", Position{Utility::convertStringToPosition("e5")}, Position{Utility::convertStringToPosition("c5")}, PieceType::ROOK, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"1nbqkbnr/ppp1pppp/3p4/3rR3/4P3/8/PPPP1PPP/RNBQKBN1 b Qk - 0 1", Position{Utility::convertStringToPosition("d5")}, Position{Utility::convertStringToPosition("f5")}, PieceType::ROOK, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"1nbqkbnr/ppp1pppp/3p4/3rR3/4P3/8/PPPP1PPP/RNBQKBN1 b Qk - 0 1", Position{Utility::convertStringToPosition("d5")}, Position{Utility::convertStringToPosition("d7")}, PieceType::ROOK, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	// Invalid queen moves
	InvalidValidateMoveTestParams{"8/8/2pQ3K/qP1P4/1PP5/6n1/bN2rR1N/4k1r1 w - - 0 1", Position{Utility::convertStringToPosition("d6")}, Position{Utility::convertStringToPosition("e4")}, PieceType::QUEEN, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"8/8/2pQ3K/qP1P4/1PP5/6n1/bN2rR1N/4k1r1 w - - 0 1", Position{Utility::convertStringToPosition("d6")}, Position{Utility::convertStringToPosition("h3")}, PieceType::QUEEN, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"8/8/2pQ3K/qP1P4/1PP5/6n1/bN2rR1N/4k1r1 b - - 0 1", Position{Utility::convertStringToPosition("a5")}, Position{Utility::convertStringToPosition("b3")}, PieceType::QUEEN, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"8/8/2pQ3K/qP1P4/1PP5/6n1/bN2rR1N/4k1r1 b - - 0 1", Position{Utility::convertStringToPosition("a5")}, Position{Utility::convertStringToPosition("c8")}, PieceType::QUEEN, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"rnb1kbnr/p3p2p/p1pp1p1p/3qQ3/3PP3/8/PPP2PPP/RNBK1BNR w kq - 0 1", Position{Utility::convertStringToPosition("e5")}, Position{Utility::convertStringToPosition("c5")}, PieceType::QUEEN, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"rnb1kbnr/p3p2p/p1pp1p1p/3qQ3/3PP3/8/PPP2PPP/RNBK1BNR w kq - 0 1", Position{Utility::convertStringToPosition("e5")}, Position{Utility::convertStringToPosition("e3")}, PieceType::QUEEN, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"rnb1kbnr/p3p2p/p1pp1p1p/3qQ3/3PP3/8/PPP2PPP/RNBK1BNR w kq - 0 1", Position{Utility::convertStringToPosition("e5")}, Position{Utility::convertStringToPosition("c3")}, PieceType::QUEEN, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"rnb1kbnr/p3p2p/p1pp1p1p/3qQ3/3PP3/8/PPP2PPP/RNBK1BNR w kq - 0 1", Position{Utility::convertStringToPosition("e5")}, Position{Utility::convertStringToPosition("g7")}, PieceType::QUEEN, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"rnb1kbnr/p3p2p/p1pp1p1p/3qQ3/3PP3/8/PPP2PPP/RNBK1BNR b kq - 0 1", Position{Utility::convertStringToPosition("d5")}, Position{Utility::convertStringToPosition("f5")}, PieceType::QUEEN, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"rnb1kbnr/p3p2p/p1pp1p1p/3qQ3/3PP3/8/PPP2PPP/RNBK1BNR b kq - 0 1", Position{Utility::convertStringToPosition("d5")}, Position{Utility::convertStringToPosition("d3")}, PieceType::QUEEN, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"rnb1kbnr/p3p2p/p1pp1p1p/3qQ3/3PP3/8/PPP2PPP/RNBK1BNR b kq - 0 1", Position{Utility::convertStringToPosition("d5")}, Position{Utility::convertStringToPosition("b7")}, PieceType::QUEEN, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"rnb1kbnr/p3p2p/p1pp1p1p/3qQ3/3PP3/8/PPP2PPP/RNBK1BNR b kq - 0 1", Position{Utility::convertStringToPosition("d5")}, Position{Utility::convertStringToPosition("d7")}, PieceType::QUEEN, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	// Invalid king moves
	InvalidValidateMoveTestParams{"8/1N1k2r1/P5b1/1p4pp/Rp5Q/1n2K3/6pP/1B6 w - - 0 1", Position{Utility::convertStringToPosition("e3")}, Position{Utility::convertStringToPosition("g1")}, PieceType::KING, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"8/1N1k2r1/P5b1/1p4pp/Rp5Q/1n2K3/6pP/1B6 w - - 0 1", Position{Utility::convertStringToPosition("e3")}, Position{Utility::convertStringToPosition("e5")}, PieceType::KING, Color::WHITE, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"8/1N1k2r1/P5b1/1p4pp/Rp5Q/1n2K3/6pP/1B6 b - - 0 1", Position{Utility::convertStringToPosition("d7")}, Position{Utility::convertStringToPosition("d4")}, PieceType::KING, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"8/1N1k2r1/P5b1/1p4pp/Rp5Q/1n2K3/6pP/1B6 b - - 0 1", Position{Utility::convertStringToPosition("d7")}, Position{Utility::convertStringToPosition("f5")}, PieceType::KING, Color::BLACK, "Invalid move - The piece cannot move to the square"},
	InvalidValidateMoveTestParams{"rnbqkbnr/ppp1pppp/2p5/8/8/2P5/PPP1PPPP/RNBQKBNR w KQkq - 0 1", Position{Utility::convertStringToPosition("e1")}, Position{Utility::convertStringToPosition("d2")}, PieceType::KING, Color::WHITE, "Invalid move - The king cannot move into check"},
	InvalidValidateMoveTestParams{"rnbqkbnr/ppp1pppp/2p5/8/8/2P5/PPP1PPPP/RNBQKBNR b KQkq - 0 1", Position{Utility::convertStringToPosition("e8")}, Position{Utility::convertStringToPosition("d7")}, PieceType::KING, Color::BLACK, "Invalid move - The king cannot move into check"},
	InvalidValidateMoveTestParams{"r3k2r/ppp1pp1p/2nqbnpb/3p4/3P4/2NQBNPB/PPP1PP1P/R3K2R w kq - 0 1", Position{Utility::convertStringToPosition("e1")}, Position{Utility::convertStringToPosition("g1")}, PieceType::KING, Color::WHITE, "Invalid move - You cannot castle"},
	InvalidValidateMoveTestParams{"r3k2r/ppp1pp1p/2nqbnpb/3p4/3P4/2NQBNPB/PPP1PP1P/R3K2R b KQ - 0 1", Position{Utility::convertStringToPosition("e8")}, Position{Utility::convertStringToPosition("g8")}, PieceType::KING, Color::BLACK, "Invalid move - You cannot castle"},
	InvalidValidateMoveTestParams{"r3k2r/ppp1pp1p/2nqbnpb/3p4/3P4/2NQBNPB/PPP1PP1P/R3K2R w Qkq - 0 1", Position{Utility::convertStringToPosition("e1")}, Position{Utility::convertStringToPosition("g1")}, PieceType::KING, Color::WHITE, "Invalid move - The king cannot castle kingside"},
	InvalidValidateMoveTestParams{"r3k2r/ppp1pp1p/2nqbnpb/3p4/3P4/2NQBNPB/PPP1PP1P/R3K2R b KQq - 0 1", Position{Utility::convertStringToPosition("e8")}, Position{Utility::convertStringToPosition("g8")}, PieceType::KING, Color::BLACK, "Invalid move - The king cannot castle kingside"},
	InvalidValidateMoveTestParams{"r3k2r/ppp1pp1p/2nqbnpb/3p4/3P4/2NQBNPB/PPP1PP1P/R3K2R w Kkq - 0 1", Position{Utility::convertStringToPosition("e1")}, Position{Utility::convertStringToPosition("c1")}, PieceType::KING, Color::WHITE, "Invalid move - The king cannot castle queenside"},
	InvalidValidateMoveTestParams{"r3k2r/ppp1pp1p/2nqbnpb/3p4/3P4/2NQBNPB/PPP1PP1P/R3K2R b KQk - 0 1", Position{Utility::convertStringToPosition("e8")}, Position{Utility::convertStringToPosition("c8")}, PieceType::KING, Color::BLACK, "Invalid move - The king cannot castle queenside"},
	InvalidValidateMoveTestParams{"r3k2r/ppp1pp1p/2nqbnpb/3p4/3P4/2N1BN1B/PPPQP1PP/R3KP1R w KQkq - 0 1", Position{Utility::convertStringToPosition("e1")}, Position{Utility::convertStringToPosition("g1")}, PieceType::KING, Color::WHITE, "Invalid move - The path between the king and the rook is not clear"},
	InvalidValidateMoveTestParams{"r3k2r/ppp1pp1p/2nqbnpb/3p4/3P4/2N1BN1B/PPP1PPPP/R2QK2R w KQkq - 0 1", Position{Utility::convertStringToPosition("e1")}, Position{Utility::convertStringToPosition("c1")}, PieceType::KING, Color::WHITE, "Invalid move - The path between the king and the rook is not clear"},
	InvalidValidateMoveTestParams{"r3kp1r/ppp1p2p/2nqbnpb/3p4/3P4/2N1BN1B/PPPQPPPP/R3K2R b KQkq - 0 1", Position{Utility::convertStringToPosition("e8")}, Position{Utility::convertStringToPosition("g8")}, PieceType::KING, Color::BLACK, "Invalid move - The path between the king and the rook is not clear"},
	InvalidValidateMoveTestParams{"r2qk2r/ppp1pp1p/2n1bnpb/3p4/3P4/2N1BN1B/PPPQPPPP/R3K2R b KQkq - 0 1", Position{Utility::convertStringToPosition("e8")}, Position{Utility::convertStringToPosition("c8")}, PieceType::KING, Color::BLACK, "Invalid move - The path between the king and the rook is not clear"},
	InvalidValidateMoveTestParams{"r3k2r/ppp1p2p/2npbQpb/3p4/3P4/2NPBqPB/PPP1P2P/R3K2R w KQkq - 0 1", Position{Utility::convertStringToPosition("e1")}, Position{Utility::convertStringToPosition("g1")}, PieceType::KING, Color::WHITE, "Invalid move - The king cannot castle through check"},
	InvalidValidateMoveTestParams{"r3k2r/ppp1pp1p/2nQbnpb/3p4/3P4/2NqBNPB/PPP1PP1P/R3K2R w KQkq - 0 1", Position{Utility::convertStringToPosition("e1")}, Position{Utility::convertStringToPosition("c1")}, PieceType::KING, Color::WHITE, "Invalid move - The king cannot castle through check"},
	InvalidValidateMoveTestParams{"r3k2r/ppp1p2p/2npbQpb/3p4/3P4/2NPBqPB/PPP1P2P/R3K2R b KQkq - 0 1", Position{Utility::convertStringToPosition("e8")}, Position{Utility::convertStringToPosition("g8")}, PieceType::KING, Color::BLACK, "Invalid move - The king cannot castle through check"},
	InvalidValidateMoveTestParams{"r3k2r/ppp1pp1p/2nQbnpb/3p4/3P4/2NqBNPB/PPP1PP1P/R3K2R b KQkq - 0 1", Position{Utility::convertStringToPosition("e8")}, Position{Utility::convertStringToPosition("c8")}, PieceType::KING, Color::BLACK, "Invalid move - The king cannot castle through check"}
);

INSTANTIATE_TEST_SUITE_P(invalidValidateMoveTest, invalidValidateMoveTest, invalidValidateMoveTestParams);

struct FindAbsolutePinsTestParams {
	std::string fenPosition;
	Color color;
	Bitboard expectedPins;
};

class findAbsolutePinsTest : public ::testing::TestWithParam<FindAbsolutePinsTestParams> {};

TEST_P(findAbsolutePinsTest, findAbsolutePins) {
	auto params = GetParam();
	Board board(params.fenPosition, "-");
	Bitboard actualPins = MoveValidator::findAbsolutePins(board, params.color);
	EXPECT_EQ(actualPins, params.expectedPins);
}

const auto findAbsolutePinsTestParams = ::testing::Values(
	FindAbsolutePinsTestParams{"3k4/8/8/8/3b4/8/1P6/K7", Color::WHITE, Bitboard{0x2000000000000ULL}},
	FindAbsolutePinsTestParams{"3k4/8/r7/8/8/N7/8/K7", Color::WHITE, Bitboard{0x10000000000ULL}},
	FindAbsolutePinsTestParams{"3k4/8/2q5/3N4/8/5K2/8/8", Color::WHITE, Bitboard{0x8000000ULL}},
	FindAbsolutePinsTestParams{"3k4/8/8/8/8/2qB1K2/8/8", Color::WHITE, Bitboard{0x80000000000ULL}},
	FindAbsolutePinsTestParams{"7k/8/b7/8/2P5/3KB2r/8/8", Color::WHITE, Bitboard{0x100400000000ULL}},
	FindAbsolutePinsTestParams{"7k/4q3/b7/1P2P3/8/8/4KB1r/8", Color::WHITE, Bitboard{0x20000012000000ULL}},
	FindAbsolutePinsTestParams{"7k/4q3/b7/1P2P3/6q1/5P2/4KB1r/8", Color::WHITE, Bitboard{0x20200012000000ULL}},
	FindAbsolutePinsTestParams{"3k4/8/1r6/B7/8/5K2/8/8", Color::BLACK, Bitboard{0x20000ULL}},
	FindAbsolutePinsTestParams{"8/8/3k4/3p4/8/5K2/3R4/8", Color::BLACK, Bitboard{0x8000000ULL}},
	FindAbsolutePinsTestParams{"7k/6p1/8/8/8/8/1Q6/7K", Color::BLACK, Bitboard{0x4000ULL}},
	FindAbsolutePinsTestParams{"8/8/8/1Q2pk2/8/8/8/7K", Color::BLACK, Bitboard{0x10000000ULL}},
	FindAbsolutePinsTestParams{"K5B1/5p2/8/3k4/8/3b4/3R4/8", Color::BLACK, Bitboard{0x80000002000ULL}},
	FindAbsolutePinsTestParams{"K5B1/5p2/8/Q1bk4/8/3b4/3R4/8", Color::BLACK, Bitboard{0x80004002000ULL}},
	FindAbsolutePinsTestParams{"K5B1/5p2/8/Q1bk4/8/3b1r2/3R4/7Q", Color::BLACK, Bitboard{0x280004002000ULL}}

);

INSTANTIATE_TEST_SUITE_P(findAbsolutePinsTest, findAbsolutePinsTest, findAbsolutePinsTestParams);