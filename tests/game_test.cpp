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
			Color activeColor = getActiveColorFromFen(fenParts[1]);
			CastleRights whiteCastleRights(getCastleRightsFromFen(fenParts[2], Color::WHITE));
			CastleRights blackCastleRights(getCastleRightsFromFen(fenParts[2], Color::BLACK));
			std::optional<Position> enPassantSquare = getEnPassantTargetSquareFromFen(fenParts[3]);
			uint8_t halfMoveClock = std::stoi(fenParts[4]);
			uint8_t fullMoveNumber = std::stoi(fenParts[5]);

			return Move{from, to, pieceType, activeColor, capturedPiece, enPassantSquare, specialMove, promotionPiece, whiteCastleRights, blackCastleRights, halfMoveClock, fullMoveNumber};
		}

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
	EXPECT_EQ(game.getActiveColor(), getActiveColorFromFen(fenParts[1]));
	EXPECT_EQ(game.getWhiteCastleRights(), getCastleRightsFromFen(fenParts[2], Color::WHITE));
	EXPECT_EQ(game.getBlackCastleRights(), getCastleRightsFromFen(fenParts[2], Color::BLACK));
	EXPECT_EQ(game.getBoard().getEnPassantTargetSquare(), getEnPassantTargetSquareFromFen(fenParts[3]));
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
	// En passant
	GameMakeMoveTestParams{"rnbqkbnr/pppp1ppp/8/3Pp3/8/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 1", "d5", "e6", PromotionPiece::NONE, PieceType::PAWN, PieceType::PAWN, SpecialMove::EN_PASSANT, "rnbqkbnr/pppp1ppp/4P3/8/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1"},
	GameMakeMoveTestParams{"rnbqkbnr/ppp1pppp/8/8/3pP3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1", "d4", "e3", PromotionPiece::NONE, PieceType::PAWN, PieceType::PAWN, SpecialMove::EN_PASSANT, "rnbqkbnr/ppp1pppp/8/8/8/4p3/PPPP1PPP/RNBQKBNR w KQkq - 0 2"}

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