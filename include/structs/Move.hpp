#ifndef MOVE_HPP
#define MOVE_HPP

#include <optional>

#include "Position.hpp"
#include "CastleRights.hpp"
#include "../enums/Color.hpp"
#include "../enums/PieceType.hpp"
#include "../enums/SpecialMove.hpp"

struct Move
{
	Position from;
	Position to;
	PieceType PieceType;
	Color color;
	std::optional<PieceType> capturedPiece;
	std::optional<Position> enPassantTargetSquare;
	SpecialMove specialMove;
	CastleRights whiteCastleRights;
	CastleRights blackCastleRights;
	int halfMoveClock;
	int fullMoveNumber;
};

#endif // MOVE_HPP