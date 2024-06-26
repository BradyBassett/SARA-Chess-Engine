#include "gtest/gtest.h"

#include "../include/Game.hpp"

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