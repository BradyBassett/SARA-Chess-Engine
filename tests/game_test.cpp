#include "gtest/gtest.h"

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
			Color activeColor = fenParts[1] == "w" ? Color::BLACK : Color::WHITE;

			bool whiteCanCastleKingside = fenParts[2].find('K') != std::string::npos;
			bool whiteCanCastleQueenside = fenParts[2].find('Q') != std::string::npos;
			bool blackCanCastleKingside = fenParts[2].find('k') != std::string::npos;
			bool blackCanCastleQueenside = fenParts[2].find('q') != std::string::npos;
			CastleRights whiteCastleRights(whiteCanCastleKingside, whiteCanCastleQueenside);
			CastleRights blackCastleRights(blackCanCastleKingside, blackCanCastleQueenside);

			std::optional<Position> enPassantTargetSquare;
			if (fenParts[3] != "-")
			{
				enPassantTargetSquare = Utility::convertStringToPosition(fenParts[3]);
			}
			else
			{
				enPassantTargetSquare = std::nullopt;
			}

			uint8_t halfMoveClock = std::stoi(fenParts[4]);
			uint8_t fullMoveNumber = std::stoi(fenParts[5]);

			return Move{from, to, pieceType, activeColor, capturedPiece, enPassantTargetSquare, specialMove, promotionPiece, whiteCastleRights, blackCastleRights, halfMoveClock, fullMoveNumber};
		}


};

TEST_P(GameMakeMoveTest, GameMakeMove)
{
	auto params = GetParam();

	Position from = Utility::convertStringToPosition(params.from);
	Position to = Utility::convertStringToPosition(params.to);

	Game game(params.fen);
	game.makeMove(from, to, params.promotionPiece);

	Move expectedMove = composeMove(game.getFenTokens(params.expectedFen), from, to, params.promotionPiece, params.pieceType, params.capturedPiece, params.specialMove);
	Move actualMove = game.getLastMove();

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
	// Because the active color is switched after the move
	EXPECT_EQ(game.getActiveColor(), expectedMove.getColor() == Color::WHITE ? Color::BLACK : Color::WHITE);
	EXPECT_EQ(game.getHalfMoveClock(), expectedMove.getHalfMoveClock());
	EXPECT_EQ(game.getFullMoveNumber(), expectedMove.getFullMoveNumber());
	EXPECT_EQ(game.getWhiteCastleRights(), expectedMove.getWhiteCastleRights());
	EXPECT_EQ(game.getBlackCastleRights(), expectedMove.getBlackCastleRights());
	EXPECT_EQ(game.getBoard().getEnPassantTargetSquare(), expectedMove.getEnPassantTargetSquare());
}

const auto gameMakeMoveTestParams = ::testing::Values(
	GameMakeMoveTestParams{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "e2", "e3", PromotionPiece::NONE, PieceType::PAWN, std::nullopt, SpecialMove::NONE, "rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR b KQkq - 0 1"}
);

INSTANTIATE_TEST_SUITE_P(GameMakeMoveTests, GameMakeMoveTest, gameMakeMoveTestParams);

// struct GameUnmakeMoveTestParams
// {
// 	std::string fen;
// 	std::string expectedFen;
// };

// class GameUnmakeMoveTest : public ::testing::TestWithParam<GameUnmakeMoveTestParams> {};

// TEST_P(GameUnmakeMoveTest, GameUnmakeMove)
// {
// 	auto params = GetParam();
// 	Game game(params.fen);
// 	Board &board = game.getBoard();

// 	game.makeMove();
// 	game.unmakeMove();

// 	EXPECT_EQ(game.getBoard().getFenPosition(), params.expectedFen);
// }

// auto gameUnmakeMoveTestParams = ::testing::Values(

// );

// INSTANTIATE_TEST_SUITE_P(GameUnmakeMoveTests, GameUnmakeMoveTest, gameUnmakeMoveTestParams);

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