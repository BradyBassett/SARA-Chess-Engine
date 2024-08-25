#include "gtest/gtest.h"
#include <map>

#include "../include/Game.hpp"
#include "../include/Utility.hpp"

struct GameConstructorTestParams
{
	std::string fen;
	Color activeColor;
	int halfMoveClock;
	int fullMoveNumber;
	bool whiteCanCastleKingside;
	bool whiteCanCastleQueenside;
	bool blackCanCastleKingside;
	bool blackCanCastleQueenside;
};

class GameConstructorTest : public ::testing::TestWithParam<GameConstructorTestParams> {};

TEST_P(GameConstructorTest, GameConstructor)
{
	auto params = GetParam();
	Game game(params.fen);

	EXPECT_EQ(game.getActiveColor(), params.activeColor);
	EXPECT_EQ(game.getHalfMoveClock(), params.halfMoveClock);
	EXPECT_EQ(game.getFullMoveNumber(), params.fullMoveNumber);

	bool whiteCanCastleKingside = game.getWhiteCastleRights().canCastleKingSide();
	bool whiteCanCastleQueenside = game.getWhiteCastleRights().canCastleQueenSide();
	bool blackCanCastleKingside = game.getBlackCastleRights().canCastleKingSide();
	bool blackCanCastleQueenside = game.getBlackCastleRights().canCastleQueenSide();

	EXPECT_EQ(whiteCanCastleKingside, params.whiteCanCastleKingside);
	EXPECT_EQ(whiteCanCastleQueenside, params.whiteCanCastleQueenside);
	EXPECT_EQ(blackCanCastleKingside, params.blackCanCastleKingside);
	EXPECT_EQ(blackCanCastleQueenside, params.blackCanCastleQueenside);
}

const auto gameConstructorTestParams = ::testing::Values(
	GameConstructorTestParams{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", Color::WHITE, 0, 1, true, true, true, true},
	GameConstructorTestParams{"3R1N2/2p5/7K/1PP5/k1P4n/b1P1rPpR/2r1Ppbq/4BN2 w - - 0 1", Color::WHITE, 0, 1, false, false, false, false},
	GameConstructorTestParams{"3R1N2/2p5/7K/1PP5/k1P4n/b1P1rPpR/2r1Ppbq/4BN2 b - - 0 1", Color::BLACK, 0, 1, false, false, false, false},
	GameConstructorTestParams{"r3k2r/p1qp1ppp/bpnb1n2/2p1p3/1P1P1P2/B1P2NPB/P1QNP2P/R3K2R b KQkq - 3 10", Color::BLACK, 3, 10, true, true, true, true},
	GameConstructorTestParams{"r3k1r1/p1qp1ppp/bpnb1n2/2p1p3/1P1P1P2/B1P2NPB/P1QNP2P/R3K2R w KQq - 4 11", Color::WHITE, 4, 11, true, true, false, true},
	GameConstructorTestParams{"r3k1r1/p1qp1ppp/bpnb1n2/2p1p3/1P1P1P2/B1P2NPB/P1QNP2P/1R2K2R b Kq - 5 11", Color::BLACK, 5, 11, true, false, false, true},
	GameConstructorTestParams{"2kr2r1/p1qp1ppp/bpnb1n2/8/1P1N1P2/B5PB/P1QNP2P/1R2K2R w K - 1 14", Color::WHITE, 1, 14, true, false, false, false},
	GameConstructorTestParams{"2kr2r1/p1qp1ppp/bpnb1n2/8/1P1N1P2/B5PB/P1QNP2P/1R3RK1 b - - 2 14", Color::BLACK, 2, 14, false, false, false, false}
);

INSTANTIATE_TEST_SUITE_P(GameConstructorTests, GameConstructorTest, gameConstructorTestParams);

// TODO: Add tests for makeMove
// ? use this fen: r1bq1rk1/1pp2ppp/p4n2/2bpp3/B2nP3/2NPBP2/PPP3PP/R2QK1NR w KQ d6 0 9 and the move e4 d5, getting bad optional access?

namespace MoveTest
{
	Color getActiveColorFromFen(std::string color)
	{
		return color == "w" ? Color::WHITE : Color::BLACK;
	}

	CastleRights getCastleRightsFromFen(std::string castleRights, Color color)
	{
		bool canCastleKingside = castleRights.find(color == Color::WHITE ? 'K' : 'k') != std::string::npos;
		bool canCastleQueenside = castleRights.find(color == Color::WHITE ? 'Q' : 'q') != std::string::npos;
		return CastleRights(canCastleKingside, canCastleQueenside);
	}

	std::optional<Position> getEnPassantTargetSquareFromFen(std::string enPassantTargetSquare)
	{
		if (enPassantTargetSquare == "-")
		{
			return std::nullopt;
		}
		return Utility::convertStringToPosition(enPassantTargetSquare);
		}
}

struct GameMakeMoveTestParams
{
	std::string fen;
	std::string from;
	std::string to;
	PromotionPiece promotionPiece;
	PieceType pieceType;
	std::optional<PieceType> capturedPiece;
	SpecialMove specialMove;
	std::string expectedFen;
};

class GameMakeMoveTest : public ::testing::TestWithParam<GameMakeMoveTestParams> {
	protected:
		Move composeMove(std::vector<std::string> fenParts, Position from, Position to, PromotionPiece promotionPiece, PieceType pieceType, std::optional<PieceType> capturedPiece, SpecialMove specialMove)
		{
			// Because the active color is switched after the move
			Color activeColor = MoveTest::getActiveColorFromFen(fenParts[1]);
			CastleRights whiteCastleRights(MoveTest::getCastleRightsFromFen(fenParts[2], Color::WHITE));
			CastleRights blackCastleRights(MoveTest::getCastleRightsFromFen(fenParts[2], Color::BLACK));
			std::optional<Position> enPassantSquare = MoveTest::getEnPassantTargetSquareFromFen(fenParts[3]);
			uint8_t halfMoveClock = std::stoi(fenParts[4]);
			uint8_t fullMoveNumber = std::stoi(fenParts[5]);

			return Move{from, to, pieceType, activeColor, capturedPiece, enPassantSquare, specialMove, promotionPiece, whiteCastleRights, blackCastleRights, halfMoveClock, fullMoveNumber};
		}
};

TEST_P(GameMakeMoveTest, GameMakeMove)
{
	auto params = GetParam();

	Position from = Utility::convertStringToPosition(params.from);
	Position to = Utility::convertStringToPosition(params.to);

	Game game(params.fen);
	game.makeMove(from, to, params.promotionPiece);
	std::vector<std::string> fenParts = game.getFenTokens(params.expectedFen);
	Move expectedMove = composeMove(game.getFenTokens(params.fen), from, to, params.promotionPiece, params.pieceType, params.capturedPiece, params.specialMove);
	Move actualMove = game.getLastMove();

	std::optional<Position> test1 = actualMove.getEnPassantTargetSquare();
	std::optional<Position> test2 = expectedMove.getEnPassantTargetSquare();

	EXPECT_EQ(actualMove.getFrom(), expectedMove.getFrom());
	EXPECT_EQ(actualMove.getTo(), expectedMove.getTo());
	EXPECT_EQ(actualMove.getPieceType(), expectedMove.getPieceType());
	EXPECT_EQ(actualMove.getColor(), expectedMove.getColor());
	EXPECT_EQ(actualMove.getCapturedPiece(), expectedMove.getCapturedPiece());
	EXPECT_EQ(actualMove.getEnPassantTargetSquare(), expectedMove.getEnPassantTargetSquare());
	EXPECT_EQ(actualMove.getSpecialMove(), expectedMove.getSpecialMove());
	EXPECT_EQ(actualMove.getPromotionPiece(), expectedMove.getPromotionPiece());
	EXPECT_EQ(actualMove.getWhiteCastleRights(), expectedMove.getWhiteCastleRights());
	EXPECT_EQ(actualMove.getBlackCastleRights(), expectedMove.getBlackCastleRights());
	EXPECT_EQ(actualMove.getHalfMoveClock(), expectedMove.getHalfMoveClock());
	EXPECT_EQ(actualMove.getFullMoveNumber(), expectedMove.getFullMoveNumber());
	EXPECT_EQ(actualMove, expectedMove);

	EXPECT_EQ(game.getFen(), params.expectedFen);
	EXPECT_EQ(game.getActiveColor(), MoveTest::getActiveColorFromFen(fenParts[1]));
	EXPECT_EQ(game.getWhiteCastleRights(), MoveTest::getCastleRightsFromFen(fenParts[2], Color::WHITE));
	EXPECT_EQ(game.getBlackCastleRights(), MoveTest::getCastleRightsFromFen(fenParts[2], Color::BLACK));
	EXPECT_EQ(game.getBoard().getEnPassantTargetSquare(), MoveTest::getEnPassantTargetSquareFromFen(fenParts[3]));
	EXPECT_EQ(game.getHalfMoveClock(), std::stoi(fenParts[4]));
	EXPECT_EQ(game.getFullMoveNumber(), std::stoi(fenParts[5]));
}

const auto gameMakeMoveTestParams = ::testing::Values(
	// Non capturing ordinary moves
	GameMakeMoveTestParams{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "e2", "e3", PromotionPiece::NONE, PieceType::PAWN, std::nullopt, SpecialMove::NONE, "rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR b KQkq - 0 1"},
	GameMakeMoveTestParams{"rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR b KQkq - 0 1", "e7", "e6", PromotionPiece::NONE, PieceType::PAWN, std::nullopt, SpecialMove::NONE, "rnbqkbnr/pppp1ppp/4p3/8/8/4P3/PPPP1PPP/RNBQKBNR w KQkq - 0 2"},
	GameMakeMoveTestParams{"rnbqkbnr/pppp1ppp/4p3/8/8/4P3/PPPP1PPP/RNBQKBNR w KQkq - 0 2", "g1", "f3", PromotionPiece::NONE, PieceType::KNIGHT, std::nullopt, SpecialMove::NONE, "rnbqkbnr/pppp1ppp/4p3/8/8/4PN2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"},
	GameMakeMoveTestParams{"rnbqkbnr/pppp1ppp/4p3/8/8/4PN2/PPPP1PPP/RNBQKB1R b KQkq - 1 2", "g8", "f6", PromotionPiece::NONE, PieceType::KNIGHT, std::nullopt, SpecialMove::NONE, "rnbqkb1r/pppp1ppp/4pn2/8/8/4PN2/PPPP1PPP/RNBQKB1R w KQkq - 2 3"},
	GameMakeMoveTestParams{"rnbqkb1r/pppp1ppp/4pn2/8/8/4PN2/PPPP1PPP/RNBQKB1R w KQkq - 2 3", "f1", "b5", PromotionPiece::NONE, PieceType::BISHOP, std::nullopt, SpecialMove::NONE, "rnbqkb1r/pppp1ppp/4pn2/1B6/8/4PN2/PPPP1PPP/RNBQK2R b KQkq - 3 3"},
	GameMakeMoveTestParams{"rnbqkb1r/pppp1ppp/4pn2/1B6/8/4PN2/PPPP1PPP/RNBQK2R b KQkq - 3 3", "f8", "b4", PromotionPiece::NONE, PieceType::BISHOP, std::nullopt, SpecialMove::NONE, "rnbqk2r/pppp1ppp/4pn2/1B6/1b6/4PN2/PPPP1PPP/RNBQK2R w KQkq - 4 4"},
	GameMakeMoveTestParams{"rnbqk2r/pppp1ppp/4pn2/1B6/1b6/4PN2/PPPP1PPP/RNBQK2R w KQkq - 4 4", "h1", "g1", PromotionPiece::NONE, PieceType::ROOK, std::nullopt, SpecialMove::NONE, "rnbqk2r/pppp1ppp/4pn2/1B6/1b6/4PN2/PPPP1PPP/RNBQK1R1 b Qkq - 5 4"},
	GameMakeMoveTestParams{"rnbqk2r/pppp1ppp/4pn2/1B6/1b6/4PN2/PPPP1PPP/RNBQK1R1 b Qkq - 5 4", "h8", "g8", PromotionPiece::NONE, PieceType::ROOK, std::nullopt, SpecialMove::NONE, "rnbqk1r1/pppp1ppp/4pn2/1B6/1b6/4PN2/PPPP1PPP/RNBQK1R1 w Qq - 6 5"},
	GameMakeMoveTestParams{"rnbqk1r1/pppp1ppp/4pn2/1B6/1b6/4PN2/PPPP1PPP/RNBQK1R1 w Qq - 6 5", "d1", "e2", PromotionPiece::NONE, PieceType::QUEEN, std::nullopt, SpecialMove::NONE, "rnbqk1r1/pppp1ppp/4pn2/1B6/1b6/4PN2/PPPPQPPP/RNB1K1R1 b Qq - 7 5"},
	GameMakeMoveTestParams{"rnbqk1r1/pppp1ppp/4pn2/1B6/1b6/4PN2/PPPPQPPP/RNB1K1R1 b Qq - 7 5", "d8", "e7", PromotionPiece::NONE, PieceType::QUEEN, std::nullopt, SpecialMove::NONE, "rnb1k1r1/ppppqppp/4pn2/1B6/1b6/4PN2/PPPPQPPP/RNB1K1R1 w Qq - 8 6"},
	GameMakeMoveTestParams{"rnb1k1r1/ppppqppp/4pn2/1B6/1b6/4PN2/PPPPQPPP/RNB1K1R1 w Qq - 8 6", "e1", "d1", PromotionPiece::NONE, PieceType::KING, std::nullopt, SpecialMove::NONE, "rnb1k1r1/ppppqppp/4pn2/1B6/1b6/4PN2/PPPPQPPP/RNBK2R1 b q - 9 6"},
	GameMakeMoveTestParams{"rnb1k1r1/ppppqppp/4pn2/1B6/1b6/4PN2/PPPPQPPP/RNBK2R1 b q - 9 6", "e8", "d8", PromotionPiece::NONE, PieceType::KING, std::nullopt, SpecialMove::NONE, "rnbk2r1/ppppqppp/4pn2/1B6/1b6/4PN2/PPPPQPPP/RNBK2R1 w - - 10 7"},
	// Capturing ordinary moves
	GameMakeMoveTestParams{"7k/8/8/3p4/2P5/8/8/7K w - - 0 1", "c4", "d5", PromotionPiece::NONE, PieceType::PAWN, PieceType::PAWN, SpecialMove::NONE, "7k/8/8/3P4/8/8/8/7K b - - 0 1"},
	GameMakeMoveTestParams{"7k/8/8/3p4/2P5/8/8/7K b - - 0 1", "d5", "c4", PromotionPiece::NONE, PieceType::PAWN, PieceType::PAWN, SpecialMove::NONE, "7k/8/8/8/2p5/8/8/7K w - - 0 2"},
	GameMakeMoveTestParams{"7k/8/8/3n4/8/2N5/8/7K w - - 0 1", "c3", "d5", PromotionPiece::NONE, PieceType::KNIGHT, PieceType::KNIGHT, SpecialMove::NONE, "7k/8/8/3N4/8/8/8/7K b - - 0 1"},
	GameMakeMoveTestParams{"7k/8/8/3n4/8/2N5/8/7K b - - 0 1", "d5", "c3", PromotionPiece::NONE, PieceType::KNIGHT, PieceType::KNIGHT, SpecialMove::NONE, "7k/8/8/8/8/2n5/8/7K w - - 0 2"},
	GameMakeMoveTestParams{"6k1/8/8/5b2/8/8/2B5/K7 w - - 0 1", "c2", "f5", PromotionPiece::NONE, PieceType::BISHOP, PieceType::BISHOP, SpecialMove::NONE, "6k1/8/8/5B2/8/8/8/K7 b - - 0 1"},
	GameMakeMoveTestParams{"6k1/8/8/5b2/8/8/2B5/K7 b - - 0 1", "f5", "c2", PromotionPiece::NONE, PieceType::BISHOP, PieceType::BISHOP, SpecialMove::NONE, "6k1/8/8/8/8/8/2b5/K7 w - - 0 2"},
	GameMakeMoveTestParams{"K7/8/8/8/R6r/8/8/7k w - - 0 1", "a4", "h4", PromotionPiece::NONE, PieceType::ROOK, PieceType::ROOK, SpecialMove::NONE, "K7/8/8/8/7R/8/8/7k b - - 0 1"},
	GameMakeMoveTestParams{"K7/8/8/8/R6r/8/8/7k b - - 0 1", "h4", "a4", PromotionPiece::NONE, PieceType::ROOK, PieceType::ROOK, SpecialMove::NONE, "K7/8/8/8/r7/8/8/7k w - - 0 2"},
	GameMakeMoveTestParams{"5k2/8/1Q6/8/8/8/1q6/7K w - - 0 1", "b6", "b2", PromotionPiece::NONE, PieceType::QUEEN, PieceType::QUEEN, SpecialMove::NONE, "5k2/8/8/8/8/8/1Q6/7K b - - 0 1"},
	GameMakeMoveTestParams{"5k2/8/1Q6/8/8/8/1q6/7K b - - 0 1", "b2", "b6", PromotionPiece::NONE, PieceType::QUEEN, PieceType::QUEEN, SpecialMove::NONE, "5k2/8/1q6/8/8/8/8/7K w - - 0 2"},
	GameMakeMoveTestParams{"8/6N1/6k1/8/8/3Kn3/8/8 w - - 0 1", "d3", "e3", PromotionPiece::NONE, PieceType::KING, PieceType::KNIGHT, SpecialMove::NONE, "8/6N1/6k1/8/8/4K3/8/8 b - - 0 1"},
	GameMakeMoveTestParams{"8/6N1/6k1/8/8/3Kn3/8/8 b - - 0 1", "g6", "g7", PromotionPiece::NONE, PieceType::KING, PieceType::KNIGHT, SpecialMove::NONE, "8/6k1/8/8/8/3Kn3/8/8 w - - 0 2"},
	// Double pawn push
	GameMakeMoveTestParams{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "e2", "e4", PromotionPiece::NONE, PieceType::PAWN, std::nullopt, SpecialMove::DOUBLE_PAWN_PUSH, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"},
	GameMakeMoveTestParams{"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1", "e7", "e5", PromotionPiece::NONE, PieceType::PAWN, std::nullopt, SpecialMove::DOUBLE_PAWN_PUSH, "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 2"},
	GameMakeMoveTestParams{"r1bqkbnr/pppppppp/n7/P7/8/8/1PPPPPPP/RNBQKBNR b KQkq - 0 2", "b7", "b5", PromotionPiece::NONE, PieceType::PAWN, std::nullopt, SpecialMove::DOUBLE_PAWN_PUSH, "r1bqkbnr/p1pppppp/n7/Pp6/8/8/1PPPPPPP/RNBQKBNR w KQkq b6 0 3"},
	// En passant
	GameMakeMoveTestParams{"rnbqkbnr/pppp1ppp/8/3Pp3/8/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 1", "d5", "e6", PromotionPiece::NONE, PieceType::PAWN, PieceType::PAWN, SpecialMove::EN_PASSANT, "rnbqkbnr/pppp1ppp/4P3/8/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1"},
	GameMakeMoveTestParams{"rnbqkbnr/ppp1pppp/8/8/3pP3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1", "d4", "e3", PromotionPiece::NONE, PieceType::PAWN, PieceType::PAWN, SpecialMove::EN_PASSANT, "rnbqkbnr/ppp1pppp/8/8/8/4p3/PPPP1PPP/RNBQKBNR w KQkq - 0 2"},
	// Castling
	GameMakeMoveTestParams{"r3k2r/pppppppp/8/1nbqbn2/8/1NBQBN2/PPPPPPPP/R3K2R w KQkq - 0 1", "e1", "g1", PromotionPiece::NONE, PieceType::KING, std::nullopt, SpecialMove::KINGSIDE_CASTLE, "r3k2r/pppppppp/8/1nbqbn2/8/1NBQBN2/PPPPPPPP/R4RK1 b kq - 1 1"},
	GameMakeMoveTestParams{"r3k2r/pppppppp/8/1nbqbn2/8/1NBQBN2/PPPPPPPP/R3K2R w KQkq - 0 1", "e1", "c1", PromotionPiece::NONE, PieceType::KING, std::nullopt, SpecialMove::QUEENSIDE_CASTLE, "r3k2r/pppppppp/8/1nbqbn2/8/1NBQBN2/PPPPPPPP/2KR3R b kq - 1 1"},
	GameMakeMoveTestParams{"r3k2r/pppppppp/8/1nbqbn2/8/1NBQBN2/PPPPPPPP/R3K2R b KQkq - 0 1", "e8", "g8", PromotionPiece::NONE, PieceType::KING, std::nullopt, SpecialMove::KINGSIDE_CASTLE, "r4rk1/pppppppp/8/1nbqbn2/8/1NBQBN2/PPPPPPPP/R3K2R w KQ - 1 2"},
	GameMakeMoveTestParams{"r3k2r/pppppppp/8/1nbqbn2/8/1NBQBN2/PPPPPPPP/R3K2R b KQkq - 0 1", "e8", "c8", PromotionPiece::NONE, PieceType::KING, std::nullopt, SpecialMove::QUEENSIDE_CASTLE, "2kr3r/pppppppp/8/1nbqbn2/8/1NBQBN2/PPPPPPPP/R3K2R w KQ - 1 2"},
	// Promotion
	GameMakeMoveTestParams{"3nk2r/pP1bpppp/8/8/8/8/4P2P/4KBNR w Kk - 0 1", "b7", "b8", PromotionPiece::BISHOP, PieceType::PAWN, std::nullopt, SpecialMove::PROMOTION, "1B1nk2r/p2bpppp/8/8/8/8/4P2P/4KBNR b Kk - 0 1"},
	GameMakeMoveTestParams{"3nk2r/pP1bpppp/8/8/8/8/4P2P/4KBNR w Kk - 0 1", "b7", "b8", PromotionPiece::KNIGHT, PieceType::PAWN, std::nullopt, SpecialMove::PROMOTION, "1N1nk2r/p2bpppp/8/8/8/8/4P2P/4KBNR b Kk - 0 1"},
	GameMakeMoveTestParams{"3nk2r/pP1bpppp/8/8/8/8/4P2P/4KBNR w Kk - 0 1", "b7", "b8", PromotionPiece::ROOK, PieceType::PAWN, std::nullopt, SpecialMove::PROMOTION, "1R1nk2r/p2bpppp/8/8/8/8/4P2P/4KBNR b Kk - 0 1"},
	GameMakeMoveTestParams{"3nk2r/pP1bpppp/8/8/8/8/4P2P/4KBNR w Kk - 0 1", "b7", "b8", PromotionPiece::QUEEN, PieceType::PAWN, std::nullopt, SpecialMove::PROMOTION, "1Q1nk2r/p2bpppp/8/8/8/8/4P2P/4KBNR b Kk - 0 1"},
	GameMakeMoveTestParams{"4kbnr/pr1ppppp/8/8/8/8/Pp1PPPPP/3BK1NR b K - 0 1", "b2", "b1", PromotionPiece::BISHOP, PieceType::PAWN, std::nullopt, SpecialMove::PROMOTION, "4kbnr/pr1ppppp/8/8/8/8/P2PPPPP/1b1BK1NR w K - 0 2"},
	GameMakeMoveTestParams{"4kbnr/pr1ppppp/8/8/8/8/Pp1PPPPP/3BK1NR b K - 0 1", "b2", "b1", PromotionPiece::KNIGHT, PieceType::PAWN, std::nullopt, SpecialMove::PROMOTION, "4kbnr/pr1ppppp/8/8/8/8/P2PPPPP/1n1BK1NR w K - 0 2"},
	GameMakeMoveTestParams{"4kbnr/pr1ppppp/8/8/8/8/Pp1PPPPP/3BK1NR b K - 0 1", "b2", "b1", PromotionPiece::ROOK, PieceType::PAWN, std::nullopt, SpecialMove::PROMOTION, "4kbnr/pr1ppppp/8/8/8/8/P2PPPPP/1r1BK1NR w K - 0 2"},
	GameMakeMoveTestParams{"4kbnr/pr1ppppp/8/8/8/8/Pp1PPPPP/3BK1NR b K - 0 1", "b2", "b1", PromotionPiece::QUEEN, PieceType::PAWN, std::nullopt, SpecialMove::PROMOTION, "4kbnr/pr1ppppp/8/8/8/8/P2PPPPP/1q1BK1NR w K - 0 2"}
);

INSTANTIATE_TEST_SUITE_P(GameMakeMoveTests, GameMakeMoveTest, gameMakeMoveTestParams);

struct GameUnmakeMoveTestParams
{
	std::string fen;
	std::string from;
	std::string to;
	PromotionPiece promotionPiece;
	std::array<int, 2> expectedPawnCounts;
	std::array<int, 2> expectedBishopCounts;
	std::array<int, 2> expectedKnightCounts;
	std::array<int, 2> expectedRookCounts;
	std::array<int, 2> expectedQueenCounts;
};

class GameUnmakeMoveTest : public ::testing::TestWithParam<GameUnmakeMoveTestParams> {};

TEST_P(GameUnmakeMoveTest, GameUnmakeMove)
{
	auto params = GetParam();

	Position from = Utility::convertStringToPosition(params.from);
	Position to = Utility::convertStringToPosition(params.to);

	Game game(params.fen);
	Board &board = game.getBoard();
	std::vector<std::string> fenParts = game.getFenTokens(params.fen);

	game.makeMove(from, to, params.promotionPiece);
	game.unmakeMove();

	EXPECT_EQ(game.getFen(), params.fen);
	EXPECT_EQ(game.getActiveColor(), MoveTest::getActiveColorFromFen(fenParts[1]));
	EXPECT_EQ(game.getWhiteCastleRights(), MoveTest::getCastleRightsFromFen(fenParts[2], Color::WHITE));
	EXPECT_EQ(game.getBlackCastleRights(), MoveTest::getCastleRightsFromFen(fenParts[2], Color::BLACK));
	EXPECT_EQ(game.getBoard().getEnPassantTargetSquare(), MoveTest::getEnPassantTargetSquareFromFen(fenParts[3]));
	EXPECT_EQ(game.getHalfMoveClock(), std::stoi(fenParts[4]));
	EXPECT_EQ(game.getFullMoveNumber(), std::stoi(fenParts[5]));

	// Validate piece counts
	EXPECT_EQ(board.getPawns(Color::WHITE).count, params.expectedPawnCounts[0]);
	EXPECT_EQ(board.getPawns(Color::BLACK).count, params.expectedPawnCounts[1]);
	EXPECT_EQ(board.getBishops(Color::WHITE).count, params.expectedBishopCounts[0]);
	EXPECT_EQ(board.getBishops(Color::BLACK).count, params.expectedBishopCounts[1]);
	EXPECT_EQ(board.getKnights(Color::WHITE).count, params.expectedKnightCounts[0]);
	EXPECT_EQ(board.getKnights(Color::BLACK).count, params.expectedKnightCounts[1]);
	EXPECT_EQ(board.getRooks(Color::WHITE).count, params.expectedRookCounts[0]);
	EXPECT_EQ(board.getRooks(Color::BLACK).count, params.expectedRookCounts[1]);
	EXPECT_EQ(board.getQueens(Color::WHITE).count, params.expectedQueenCounts[0]);
	EXPECT_EQ(board.getQueens(Color::BLACK).count, params.expectedQueenCounts[1]);
}

auto gameUnmakeMoveTestParams = ::testing::Values(
	// Non capturing ordinary moves
	GameUnmakeMoveTestParams{"2Q5/p2K4/1r3N1P/p2b2pr/7n/2R1n3/2P1B3/k5b1 w - - 0 1", "h6", "h7", PromotionPiece::NONE, {2, 3}, {1, 2}, {1, 2}, {1, 2}, {1, 0}},
	GameUnmakeMoveTestParams{"2Q5/p2K4/1r3N1P/p2b2pr/7n/2R1n3/2P1B3/k5b1 w - - 0 1", "e2", "f1", PromotionPiece::NONE, {2, 3}, {1, 2}, {1, 2}, {1, 2}, {1, 0}},
	GameUnmakeMoveTestParams{"2Q5/p2K4/1r3N1P/p2b2pr/7n/2R1n3/2P1B3/k5b1 w - - 0 1", "f6", "e8", PromotionPiece::NONE, {2, 3}, {1, 2}, {1, 2}, {1, 2}, {1, 0}},
	GameUnmakeMoveTestParams{"2Q5/p2K4/1r3N1P/p2b2pr/7n/2R1n3/2P1B3/k5b1 w - - 0 1", "c3", "c5", PromotionPiece::NONE, {2, 3}, {1, 2}, {1, 2}, {1, 2}, {1, 0}},
	GameUnmakeMoveTestParams{"2Q5/p2K4/1r3N1P/p2b2pr/7n/2R1n3/2P1B3/k5b1 w - - 0 1", "c8", "h8", PromotionPiece::NONE, {2, 3}, {1, 2}, {1, 2}, {1, 2}, {1, 0}},
	GameUnmakeMoveTestParams{"2Q5/p2K4/1r3N1P/p2b2pr/7n/2R1n3/2P1B3/k5b1 w - - 0 1", "d7", "c7", PromotionPiece::NONE, {2, 3}, {1, 2}, {1, 2}, {1, 2}, {1, 0}},
	GameUnmakeMoveTestParams{"5B2/rB5p/2bpqP2/3N1p2/2Kp4/5R2/1k2pP1n/7N b - - 0 1", "d4", "d3", PromotionPiece::NONE, {2, 5}, {2, 1}, {2, 1}, {1, 1}, {0, 1}},
	GameUnmakeMoveTestParams{"5B2/rB5p/2bpqP2/3N1p2/2Kp4/5R2/1k2pP1n/7N b - - 0 1", "c6", "a4", PromotionPiece::NONE, {2, 5}, {2, 1}, {2, 1}, {1, 1}, {0, 1}},
	GameUnmakeMoveTestParams{"5B2/rB5p/2bpqP2/3N1p2/2Kp4/5R2/1k2pP1n/7N b - - 0 1", "h2", "g4", PromotionPiece::NONE, {2, 5}, {2, 1}, {2, 1}, {1, 1}, {0, 1}},
	GameUnmakeMoveTestParams{"5B2/rB5p/2bpqP2/3N1p2/2Kp4/5R2/1k2pP1n/7N b - - 0 1", "a7", "a3", PromotionPiece::NONE, {2, 5}, {2, 1}, {2, 1}, {1, 1}, {0, 1}},
	GameUnmakeMoveTestParams{"5B2/rB5p/2bpqP2/3N1p2/2Kp4/5R2/1k2pP1n/7N b - - 0 1", "e6", "c8", PromotionPiece::NONE, {2, 5}, {2, 1}, {2, 1}, {1, 1}, {0, 1}},
	GameUnmakeMoveTestParams{"5B2/rB5p/2bpqP2/3N1p2/2Kp4/5R2/1k2pP1n/7N b - - 0 1", "b2", "a1", PromotionPiece::NONE, {2, 5}, {2, 1}, {2, 1}, {1, 1}, {0, 1}},
	// Ordinary capturing moves
	GameUnmakeMoveTestParams{"Q3B2r/2K3n1/2p2B2/3PR2p/pp3P2/1p6/N5k1/2q5 w - - 0 1", "d5", "c6", PromotionPiece::NONE, {2, 5}, {2, 0}, {1, 1}, {1, 1}, {1, 1}},
	GameUnmakeMoveTestParams{"Q3B2r/2K3n1/2p2B2/3PR2p/pp3P2/1p6/N5k1/2q5 w - - 0 1", "e8", "c6", PromotionPiece::NONE, {2, 5}, {2, 0}, {1, 1}, {1, 1}, {1, 1}},
	GameUnmakeMoveTestParams{"Q3B2r/2K3n1/2p2B2/3PR2p/pp3P2/1p6/N5k1/2q5 w - - 0 1", "a2", "b4", PromotionPiece::NONE, {2, 5}, {2, 0}, {1, 1}, {1, 1}, {1, 1}},
	GameUnmakeMoveTestParams{"Q3B2r/2K3n1/2p2B2/3PR2p/pp3P2/1p6/N5k1/2q5 w - - 0 1", "e5", "h5", PromotionPiece::NONE, {2, 5}, {2, 0}, {1, 1}, {1, 1}, {1, 1}},
	GameUnmakeMoveTestParams{"Q3B2r/2K3n1/2p2B2/3PR2p/pp3P2/1p6/N5k1/2q5 w - - 0 1", "a8", "a4", PromotionPiece::NONE, {2, 5}, {2, 0}, {1, 1}, {1, 1}, {1, 1}},
	GameUnmakeMoveTestParams{"Q3B2r/2K3n1/2p2B2/3PR2p/pp3P2/1p6/N5k1/1q6 w - - 0 1", "c7", "c6", PromotionPiece::NONE, {2, 5}, {2, 0}, {1, 1}, {1, 1}, {1, 1}},
	GameUnmakeMoveTestParams{"4BkBr/K7/2pn3R/3PP2p/pp6/1p6/1q1b4/8 b - - 0 1", "c6", "d5", PromotionPiece::NONE, {2, 5}, {2, 1}, {0, 1}, {1, 1}, {0, 1}},
	GameUnmakeMoveTestParams{"4BkBr/K7/2pn3R/3PP2p/pp6/1p6/1q1b4/8 b - - 0 1", "d2", "h6", PromotionPiece::NONE, {2, 5}, {2, 1}, {0, 1}, {1, 1}, {0, 1}},
	GameUnmakeMoveTestParams{"4BkBr/K7/2pn3R/3PP2p/pp6/1p6/1q1b4/8 b - - 0 1", "d6", "e8", PromotionPiece::NONE, {2, 5}, {2, 1}, {0, 1}, {1, 1}, {0, 1}},
	GameUnmakeMoveTestParams{"4BkBr/K7/2pn3R/3PP2p/pp6/1p6/1q1b4/8 b - - 0 1", "h8", "g8", PromotionPiece::NONE, {2, 5}, {2, 1}, {0, 1}, {1, 1}, {0, 1}},
	GameUnmakeMoveTestParams{"4BkBr/K7/2pn3R/3PP2p/pp6/1p6/1q1b4/8 b - - 0 1", "b2", "e5", PromotionPiece::NONE, {2, 5}, {2, 1}, {0, 1}, {1, 1}, {0, 1}},
	GameUnmakeMoveTestParams{"4BkBr/K7/2pn3R/3PP2p/pp6/1p6/1q1b4/8 b - - 0 1", "f8", "e8", PromotionPiece::NONE, {2, 5}, {2, 1}, {0, 1}, {1, 1}, {0, 1}},
	// Double pawn push
	GameUnmakeMoveTestParams{"R1n3bk/N1pP4/8/B3p3/P4n2/3P3p/2P5/1Q5K w - - 0 1", "c2", "c4", PromotionPiece::NONE, {4, 3}, {1, 1}, {1, 2}, {1, 0}, {1, 0}},
	GameUnmakeMoveTestParams{"R1n3bk/N1pP4/8/B3p3/P4n2/3P3p/2P5/1Q5K b - - 0 1", "c7", "c5", PromotionPiece::NONE, {4, 3}, {1, 1}, {1, 2}, {1, 0}, {1, 0}},
	// En passant
	GameUnmakeMoveTestParams{"rnbqkbnr/p1pppppp/8/1pP5/8/8/PPPPP1PP/RNBQKBNR w KQkq b6 0 1", "c5", "b6", PromotionPiece::NONE, {8, 8}, {2, 2}, {2, 2}, {2, 2}, {1, 1}},
	GameUnmakeMoveTestParams{"rnbqkbnr/ppp1pppp/8/8/2Pp4/8/PP1PPPPP/RNBQKBNR b KQkq c3 0 1", "d4", "c3", PromotionPiece::NONE, {8, 8}, {2, 2}, {2, 2}, {2, 2}, {1, 1}},
	// Castling
	GameUnmakeMoveTestParams{"r3k2r/pppppppp/8/1nbqbn2/8/1NBQBN2/PPPPPPPP/R3K2R w KQkq - 0 1", "e1", "g1", PromotionPiece::NONE, {8, 8}, {2, 2}, {2, 2}, {2, 2}, {1, 1}},
	GameUnmakeMoveTestParams{"r3k2r/pppppppp/8/1nbqbn2/8/1NBQBN2/PPPPPPPP/R3K2R w KQkq - 0 1", "e1", "c1", PromotionPiece::NONE, {8, 8}, {2, 2}, {2, 2}, {2, 2}, {1, 1}},
	GameUnmakeMoveTestParams{"r3k2r/pppppppp/8/1nbqbn2/8/1NBQBN2/PPPPPPPP/R3K2R b KQkq - 0 1", "e8", "g8", PromotionPiece::NONE, {8, 8}, {2, 2}, {2, 2}, {2, 2}, {1, 1}},
	GameUnmakeMoveTestParams{"r3k2r/pppppppp/8/1nbqbn2/8/1NBQBN2/PPPPPPPP/R3K2R b KQkq - 0 1", "e8", "c8", PromotionPiece::NONE, {8, 8}, {2, 2}, {2, 2}, {2, 2}, {1, 1}},
	// Promotion
	GameUnmakeMoveTestParams{"3nk2r/pP1bpppp/8/8/8/8/4P2P/4KBNR w Kk - 0 1", "b7", "b8", PromotionPiece::BISHOP, {3, 5}, {1, 1}, {1, 1}, {1, 1}, {0, 0}},
	GameUnmakeMoveTestParams{"3nk2r/pP1bpppp/8/8/8/8/4P2P/4KBNR w Kk - 0 1", "b7", "b8", PromotionPiece::KNIGHT, {3, 5}, {1, 1}, {1, 1}, {1, 1}, {0, 0}},
	GameUnmakeMoveTestParams{"3nk2r/pP1bpppp/8/8/8/8/4P2P/4KBNR w Kk - 0 1", "b7", "b8", PromotionPiece::ROOK, {3, 5}, {1, 1}, {1, 1}, {1, 1}, {0, 0}},
	GameUnmakeMoveTestParams{"3nk2r/pP1bpppp/8/8/8/8/4P2P/4KBNR w Kk - 0 1", "b7", "b8", PromotionPiece::QUEEN, {3, 5}, {1, 1}, {1, 1}, {1, 1}, {0, 0}},
	GameUnmakeMoveTestParams{"4kbnr/pr1ppppp/8/8/8/8/Pp1PPPPP/3BK1NR b K - 0 1", "b2", "b1", PromotionPiece::BISHOP, {6, 7}, {1, 1}, {1, 1}, {1, 2}, {0, 0}},
	GameUnmakeMoveTestParams{"4kbnr/pr1ppppp/8/8/8/8/Pp1PPPPP/3BK1NR b K - 0 1", "b2", "b1", PromotionPiece::KNIGHT, {6, 7}, {1, 1}, {1, 1}, {1, 2}, {0, 0}},
	GameUnmakeMoveTestParams{"4kbnr/pr1ppppp/8/8/8/8/Pp1PPPPP/3BK1NR b K - 0 1", "b2", "b1", PromotionPiece::ROOK, {6, 7}, {1, 1}, {1, 1}, {1, 2}, {0, 0}},
	GameUnmakeMoveTestParams{"4kbnr/pr1ppppp/8/8/8/8/Pp1PPPPP/3BK1NR b K - 0 1", "b2", "b1", PromotionPiece::QUEEN, {6, 7}, {1, 1}, {1, 1}, {1, 2}, {0, 0}}
);

INSTANTIATE_TEST_SUITE_P(GameUnmakeMoveTests, GameUnmakeMoveTest, gameUnmakeMoveTestParams);

TEST(GameUnmakeMoveTest, GameUnmakeMoveEmptyHistory)
{
	Game game("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	EXPECT_THROW({
		try
		{
			game.unmakeMove();
		}
		catch (const std::invalid_argument &e)
		{
			EXPECT_STREQ(e.what(), "No moves to undo");
			throw;
		}
	}, std::invalid_argument);
}

struct PerftTestParams
{
	int depth;
	std::map<std::string, int> expectedOutput;
};

class PerftTest : public ::testing::TestWithParam<PerftTestParams> {};

TEST_P(PerftTest, Perft)
{
	auto params = GetParam();

	Game game("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	std::map<std::string, int> output;
	game.perftRoot(params.depth, output);

	EXPECT_EQ(output, params.expectedOutput);
}

auto perftTestParams = ::testing::Values(
	PerftTestParams{1, {{"a2a3", 1}, {"b2b3", 1}, {"c2c3", 1}, {"d2d3", 1}, {"e2e3", 1}, {"f2f3", 1}, {"g2g3", 1}, {"h2h3", 1}, {"a2a4", 1}, {"b2b4", 1}, {"c2c4", 1}, {"d2d4", 1}, {"e2e4", 1}, {"f2f4", 1}, {"g2g4", 1}, {"h2h4", 1}, {"b1a3", 1}, {"b1c3", 1}, {"g1f3", 1}, {"g1h3", 1}, {"Nodes", 20}}},
	PerftTestParams{2, {{"a2a3", 20}, {"b2b3", 20}, {"c2c3", 20}, {"d2d3", 20}, {"e2e3", 20}, {"f2f3", 20}, {"g2g3", 20}, {"h2h3", 20}, {"a2a4", 20}, {"b2b4", 20}, {"c2c4", 20}, {"d2d4", 20}, {"e2e4", 20}, {"f2f4", 20}, {"g2g4", 20}, {"h2h4", 20}, {"b1a3", 20}, {"b1c3", 20}, {"g1f3", 20}, {"g1h3", 20}, {"Nodes", 400}}},
	PerftTestParams{3, {{"a2a3", 380}, {"b2b3", 420}, {"c2c3", 420}, {"d2d3", 539}, {"e2e3", 599}, {"f2f3", 380}, {"g2g3", 420}, {"h2h3", 380}, {"a2a4", 420}, {"b2b4", 421}, {"c2c4", 441}, {"d2d4", 560}, {"e2e4", 600}, {"f2f4", 401}, {"g2g4", 421}, {"h2h4", 420}, {"b1a3", 400}, {"b1c3", 440}, {"g1f3", 440}, {"g1h3", 400}, {"Nodes", 8902}}},
	PerftTestParams{4, {{"a2a3", 8457}, {"b2b3", 9345}, {"c2c3", 9272}, {"d2d3", 11959}, {"e2e3", 13134}, {"f2f3", 8457}, {"g2g3", 9345}, {"h2h3", 8457}, {"a2a4", 9329}, {"b2b4", 9332}, {"c2c4", 9744}, {"d2d4", 12435}, {"e2e4", 13160}, {"f2f4", 8929}, {"g2g4", 9328}, {"h2h4", 9329}, {"b1a3", 8885}, {"b1c3", 9755}, {"g1f3", 9748}, {"g1h3", 8881}, {"Nodes", 197281}}},
	PerftTestParams{5, {{"a2a3", 181046}, {"b2b3", 215255}, {"c2c3", 222861}, {"d2d3", 328511}, {"e2e3", 402988}, {"f2f3", 178889}, {"g2g3", 217210}, {"h2h3", 181044}, {"a2a4", 217832}, {"b2b4", 216145}, {"c2c4", 240082}, {"d2d4", 361790}, {"e2e4", 405385}, {"f2f4", 198473}, {"g2g4", 214048}, {"h2h4", 218829}, {"b1a3", 198572}, {"b1c3", 234656}, {"g1f3", 233491}, {"g1h3", 198502}, {"Nodes", 4865609}}},
	PerftTestParams{6, {{"a2a3", 4463267}, {"b2b3", 5310358}, {"c2c3", 5417640}, {"d2d3", 8073082}, {"e2e3", 9726018}, {"f2f3", 4404141}, {"g2g3", 5346260}, {"h2h3", 4463070}, {"a2a4", 5363555}, {"b2b4", 5293555}, {"c2c4", 5866666}, {"d2d4", 8879566}, {"e2e4", 9771632}, {"f2f4", 4890429}, {"g2g4", 5239875}, {"h2h4", 5385554}, {"b1a3", 4856835}, {"b1c3", 5708064}, {"g1f3", 5723523}, {"g1h3", 4877234}, {"Nodes", 119060324}}}
);

INSTANTIATE_TEST_SUITE_P(PerftTestsStartingPosition, PerftTest, perftTestParams);

// TEST(CustomPerft, customFen)
// {
// 	Game game("rnbqkbnr/ppp1pppp/8/3p4/8/2P5/PP1PPPPP/RNBQKBNR w KQkq d6 0 2");
// 	std::map<std::string, int> output;
// 	game.perftRoot(1, output);

// 	std::map<std::string, int> expectedOutput = {
// 		{"a2a3", 1},
// 		{"b2b3", 1},
// 		{"d2d3", 1},
// 		{"e2e3", 1},
// 		{"f2f3", 1},
// 		{"g2g3", 1},
// 		{"h2h3", 1},
// 		{"c3c4", 1},
// 		{"a2a4", 1},
// 		{"b2b4", 1},
// 		{"d2d4", 1},
// 		{"e2e4", 1},
// 		{"f2f4", 1},
// 		{"g2g4", 1},
// 		{"h2h4", 1},
// 		{"b1a3", 1},
// 		{"g1f3", 1},
// 		{"g1h3", 1},
// 		{"d1c2", 1},
// 		{"d1b3", 1},
// 		{"d1a4", 1},
// 		{"Nodes", 21}
// 	};

// 	EXPECT_EQ(output, expectedOutput);
// }