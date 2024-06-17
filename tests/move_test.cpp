#include <gtest/gtest.h>

#include "../include/Move.hpp"
#include "../include/structs/Position.hpp"
#include "../include/structs/CastleRights.hpp"
#include "../include/enums/Color.hpp"
#include "../include/enums/PieceType.hpp"
#include "../include/enums/SpecialMove.hpp"

struct MoveConstructorTestParams
{
	Position from;
	Position to;
	PieceType pieceType;
	Color color;
	std::optional<PieceType> capturedPiece;
	std::optional<Position> enPassantTargetSquare;
	SpecialMove specialMove;
	CastleRights whiteCastleRights;
	CastleRights blackCastleRights;
	uint8_t halfMoveClock;
	uint16_t fullMoveNumber;
};

class MoveConstructorTest : public ::testing::TestWithParam<MoveConstructorTestParams> {};

TEST_P(MoveConstructorTest, MoveConstructor)
{
	auto params = GetParam();
	Move move(params.from, params.to, params.pieceType, params.color,
			  params.capturedPiece, params.enPassantTargetSquare,
			  params.specialMove, params.whiteCastleRights, params.blackCastleRights,
			  params.halfMoveClock, params.fullMoveNumber);

	EXPECT_EQ(move.getFrom(), params.from);
	EXPECT_EQ(move.getTo(), params.to);
	EXPECT_EQ(move.getPieceType(), params.pieceType);
	EXPECT_EQ(move.getColor(), params.color);
	EXPECT_EQ(move.getCapturedPiece(), params.capturedPiece);
	EXPECT_EQ(move.getEnPassantTargetSquare(), params.enPassantTargetSquare);
	EXPECT_EQ(move.getSpecialMove(), params.specialMove);
	EXPECT_EQ(move.getWhiteCastleRights(), params.whiteCastleRights);
	EXPECT_EQ(move.getBlackCastleRights(), params.blackCastleRights);
	EXPECT_EQ(move.getHalfMoveClock(), params.halfMoveClock);
	EXPECT_EQ(move.getFullMoveNumber(), params.fullMoveNumber);
}

const auto moveConstructorTestParams = ::testing::Values(
	MoveConstructorTestParams{Position{0, 0}, Position{0, 1}, PieceType::PAWN, Color::WHITE, std::nullopt, std::nullopt, SpecialMove::NONE, CastleRights{true, true}, CastleRights{true, true}, 0, 1},
	MoveConstructorTestParams{Position{1, 5}, Position{4, 2}, PieceType::KNIGHT, Color::BLACK, std::nullopt, Position{1, 1}, SpecialMove::DOUBLE_PAWN_PUSH, CastleRights{false, true}, CastleRights{true, true}, 4, 10},
	MoveConstructorTestParams{Position{7, 7}, Position{7, 6}, PieceType::BISHOP, Color::WHITE, PieceType::PAWN, std::nullopt, SpecialMove::EN_PASSANT, CastleRights{true, true}, CastleRights{false, false}, 100, 1000},
	MoveConstructorTestParams{Position{3, 3}, Position{3, 4}, PieceType::ROOK, Color::BLACK, PieceType::KNIGHT, Position{2, 4}, SpecialMove::KINGSIDE_CASTLE, CastleRights{true, false}, CastleRights{true, true}, 50, 5899},
	MoveConstructorTestParams{Position{5, 5}, Position{5, 6}, PieceType::QUEEN, Color::WHITE, PieceType::BISHOP, Position{4, 6}, SpecialMove::QUEENSIDE_CASTLE, CastleRights{false, true}, CastleRights{true, false}, 99, 356},
	MoveConstructorTestParams{Position{6, 6}, Position{6, 7}, PieceType::KING, Color::BLACK, PieceType::ROOK, Position{7, 7}, SpecialMove::PROMOTION, CastleRights{false, false}, CastleRights{false, false}, 1, 2}
);

INSTANTIATE_TEST_SUITE_P(MoveConstructorTest, MoveConstructorTest, moveConstructorTestParams);