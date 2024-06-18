#include "../include/Move.hpp"
#include "../include/Utility.hpp"

Move::Move(Position from, Position to, PieceType pieceType, Color color,
		   std::optional<PieceType> capturedPiece, std::optional<Position> enPassantTargetSquare,
		   SpecialMove specialMove, CastleRights whiteCastleRights, CastleRights blackCastleRights,
		   uint8_t halfMoveClock, uint16_t fullMoveNumber)
{
	setFrom(from);
	setTo(to);
	setPieceType(pieceType);
	setColor(color);
	setCapturedPiece(capturedPiece);
	setEnPassantTargetSquare(enPassantTargetSquare);
	setSpecialMove(specialMove);
	setWhiteCastleRights(whiteCastleRights);
	setBlackCastleRights(blackCastleRights);
	setHalfMoveClock(halfMoveClock);
	setFullMoveNumber(fullMoveNumber);
}

Position Move::getFrom() const
{
	int fromSquare = decode(FROM_MASK, FROM_SHIFT);
	int row = fromSquare / 8;
	int col = fromSquare % 8;

	return Position{row, col};
}

void Move::setFrom(Position from)
{
	Utility::validatePosition(from);

	int fromSquare = from.row * 8 + from.col;

	move = encode(FROM_MASK, FROM_SHIFT, fromSquare);
}

Position Move::getTo() const
{
	int toSquare = decode(TO_MASK, TO_SHIFT);
	int row = toSquare / 8;
	int col = toSquare % 8;

	return Position{row, col};
}

void Move::setTo(Position to)
{
	Utility::validatePosition(to);

	int toSquare = to.row * 8 + to.col;

	move = encode(TO_MASK, TO_SHIFT, toSquare);
}

PieceType Move::getPieceType() const
{
	int pieceType = decode(PIECE_TYPE_MASK, PIECE_TYPE_SHIFT);

	return static_cast<PieceType>(pieceType);
}

void Move::setPieceType(PieceType pieceType)
{
	move = encode(PIECE_TYPE_MASK, PIECE_TYPE_SHIFT, static_cast<int>(pieceType));
}

Color Move::getColor() const
{
	int color = decode(COLOR_MASK, COLOR_SHIFT);

	return static_cast<Color>(color);
}

void Move::setColor(Color color)
{
	move = encode(COLOR_MASK, COLOR_SHIFT, static_cast<int>(color));
}

std::optional<PieceType> Move::getCapturedPiece() const
{
	if (decode(CAPTURED_PIECE_PRESENT_MASK, CAPTURED_PIECE_PRESENT_SHIFT))
	{
		int capturedPiece = decode(CAPTURED_PIECE_MASK, CAPTURED_PIECE_SHIFT);

		return static_cast<PieceType>(capturedPiece);
	}

	return std::nullopt;
}

void Move::setCapturedPiece(std::optional<PieceType> capturedPiece)
{
	if (capturedPiece.has_value())
	{
		move = encode(CAPTURED_PIECE_PRESENT_MASK, CAPTURED_PIECE_PRESENT_SHIFT, 1);
		move = encode(CAPTURED_PIECE_MASK, CAPTURED_PIECE_SHIFT, static_cast<int>(capturedPiece.value()));
	}
	else
	{
		move = encode(CAPTURED_PIECE_PRESENT_MASK, CAPTURED_PIECE_PRESENT_SHIFT, 0);
	}
}

std::optional<Position> Move::getEnPassantTargetSquare() const
{
	if (decode(EN_PASSANT_TARGET_SQUARE_PRESENT_MASK, EN_PASSANT_TARGET_SQUARE_PRESENT_SHIFT))
	{
		int enPassantTargetSquare = decode(EN_PASSANT_TARGET_SQUARE_MASK, EN_PASSANT_TARGET_SQUARE_SHIFT);
		int row = enPassantTargetSquare / 8;
		int col = enPassantTargetSquare % 8;

		return Position{row, col};
	}

	return std::nullopt;
}

void Move::setEnPassantTargetSquare(std::optional<Position> enPassantTargetSquare)
{
	if (enPassantTargetSquare.has_value())
	{
		int row = enPassantTargetSquare.value().row;
		int col = enPassantTargetSquare.value().col;

		Utility::validatePosition(enPassantTargetSquare.value());

		int enPassantTargetSquare = row * 8 + col;

		move = encode(EN_PASSANT_TARGET_SQUARE_PRESENT_MASK, EN_PASSANT_TARGET_SQUARE_PRESENT_SHIFT, 1);
		move = encode(EN_PASSANT_TARGET_SQUARE_MASK, EN_PASSANT_TARGET_SQUARE_SHIFT, enPassantTargetSquare);
	}
	else
	{
		move = encode(EN_PASSANT_TARGET_SQUARE_PRESENT_MASK, EN_PASSANT_TARGET_SQUARE_PRESENT_SHIFT, 0);
	}
}

SpecialMove Move::getSpecialMove() const
{
	int specialMove = decode(SPECIAL_MOVE_MASK, SPECIAL_MOVE_SHIFT);

	return static_cast<SpecialMove>(specialMove);
}

void Move::setSpecialMove(SpecialMove specialMove)
{
	move = encode(SPECIAL_MOVE_MASK, SPECIAL_MOVE_SHIFT, static_cast<int>(specialMove));
}

CastleRights Move::getWhiteCastleRights() const
{
	bool whiteCastleKingside = decode(WHITE_CASTLE_KINGSIDE_MASK, WHITE_CASTLE_KINGSIDE_SHIFT);
	bool whiteCastleQueenside = decode(WHITE_CASTLE_QUEENSIDE_MASK, WHITE_CASTLE_QUEENSIDE_SHIFT);

	return CastleRights{whiteCastleKingside, whiteCastleQueenside};
}

void Move::setWhiteCastleRights(CastleRights whiteCastleRights)
{
	move = encode(WHITE_CASTLE_KINGSIDE_MASK, WHITE_CASTLE_KINGSIDE_SHIFT, whiteCastleRights.canCastleKingSide());
	move = encode(WHITE_CASTLE_QUEENSIDE_MASK, WHITE_CASTLE_QUEENSIDE_SHIFT, whiteCastleRights.canCastleQueenSide());
}

CastleRights Move::getBlackCastleRights() const
{
	bool blackCastleKingside = decode(BLACK_CASTLE_KINGSIDE_MASK, BLACK_CASTLE_KINGSIDE_SHIFT);
	bool blackCastleQueenside = decode(BLACK_CASTLE_QUEENSIDE_MASK, BLACK_CASTLE_QUEENSIDE_SHIFT);

	return CastleRights{blackCastleKingside, blackCastleQueenside};
}

void Move::setBlackCastleRights(CastleRights blackCastleRights)
{
	move = encode(BLACK_CASTLE_KINGSIDE_MASK, BLACK_CASTLE_KINGSIDE_SHIFT, blackCastleRights.canCastleKingSide());
	move = encode(BLACK_CASTLE_QUEENSIDE_MASK, BLACK_CASTLE_QUEENSIDE_SHIFT, blackCastleRights.canCastleQueenSide());
}

uint8_t Move::getHalfMoveClock() const
{
	return decode(HALF_MOVE_CLOCK_MASK, HALF_MOVE_CLOCK_SHIFT);
}

void Move::setHalfMoveClock(uint8_t halfMoveClock)
{
	move = encode(HALF_MOVE_CLOCK_MASK, HALF_MOVE_CLOCK_SHIFT, halfMoveClock);
}

uint16_t Move::getFullMoveNumber() const
{
	return decode(FULL_MOVE_NUMBER_MASK, FULL_MOVE_NUMBER_SHIFT);
}

void Move::setFullMoveNumber(uint16_t fullMoveNumber)
{
	move = encode(FULL_MOVE_NUMBER_MASK, FULL_MOVE_NUMBER_SHIFT, fullMoveNumber);
}

uint64_t Move::decode(uint64_t mask, uint8_t shift) const
{
	return (move & mask) >> shift;
}

uint64_t Move::encode(uint64_t mask, uint8_t shift, uint64_t value)
{
	return (move & ~mask) | ((value << shift) & mask);
}