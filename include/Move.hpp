#ifndef MOVE_HPP
#define MOVE_HPP

#include <optional>
#include <cstdint>

#include "../include/structs/Position.hpp"
#include "../include/structs/CastleRights.hpp"
#include "../include/enums/Color.hpp"
#include "../include/enums/PieceType.hpp"
#include "../include/enums/SpecialMove.hpp"

class Move
{
public:
	Move(Position from, Position to, PieceType pieceType, Color color,
		 std::optional<PieceType> capturedPiece, std::optional<Position> enPassantTargetSquare,
		 SpecialMove specialMove, CastleRights whiteCastleRights, CastleRights blackCastleRights,
		 uint8_t halfMoveClock, uint16_t fullMoveNumber);

	Position getFrom() const;
	void setFrom(Position from);
	Position getTo() const;
	void setTo(Position to);
	PieceType getPieceType() const;
	void setPieceType(PieceType pieceType);
	Color getColor() const;
	void setColor(Color color);
	std::optional<PieceType> getCapturedPiece() const;
	void setCapturedPiece(std::optional<PieceType> capturedPiece);
	std::optional<Position> getEnPassantTargetSquare() const;
	void setEnPassantTargetSquare(std::optional<Position> enPassantTargetSquare);
	SpecialMove getSpecialMove() const;
	void setSpecialMove(SpecialMove specialMove);
	CastleRights getWhiteCastleRights() const;
	void setWhiteCastleRights(CastleRights whiteCastleRights);
	CastleRights getBlackCastleRights() const;
	void setBlackCastleRights(CastleRights blackCastleRights);
	uint8_t getHalfMoveClock() const;
	void setHalfMoveClock(uint8_t halfMoveClock);
	uint16_t getFullMoveNumber() const;
	void setFullMoveNumber(uint16_t fullMoveNumber);

private:
	uint64_t move;

	// bit masks
	static constexpr uint64_t FROM_MASK = 0x3F;									// 6 bits
	static constexpr uint64_t TO_MASK = 0xFC0;									// 6 bits, shifted 6 bits
	static constexpr uint64_t PIECE_TYPE_MASK = 0x7000;							// 3 bits, shifted 12 bits
	static constexpr uint64_t COLOR_MASK = 0x8000;								// 1 bit, shifted 15 bits
	static constexpr uint64_t CAPTURED_PIECE_PRESENT_MASK = 0x10000;			// 1 bit, shifted 16 bits
	static constexpr uint64_t CAPTURED_PIECE_MASK = 0xe0000;					// 3 bits, shifted 17 bits
	static constexpr uint64_t EN_PASSANT_TARGET_SQUARE_PRESENT_MASK = 0x100000; // 1 bit, shifted 20 bits
	static constexpr uint64_t EN_PASSANT_TARGET_SQUARE_MASK = 0x7e00000;		// 6 bits, shifted 21 bits
	static constexpr uint64_t SPECIAL_MOVE_MASK = 0x38000000;					// 3 bits, shifted 27 bits
	static constexpr uint64_t WHITE_CASTLE_KINGSIDE_MASK = 0x40000000;			// 1 bit, shifted 30 bits
	static constexpr uint64_t WHITE_CASTLE_QUEENSIDE_MASK = 0x80000000;			// 1 bit, shifted 31 bits
	static constexpr uint64_t BLACK_CASTLE_KINGSIDE_MASK = 0x100000000;			// 1 bit, shifted 32 bits
	static constexpr uint64_t BLACK_CASTLE_QUEENSIDE_MASK = 0x200000000;		// 1 bit, shifted 33 bits
	static constexpr uint64_t HALF_MOVE_CLOCK_MASK = 0x1fc00000000;				// 7 bits, shifted 34 bits based on maximum of 100 half moves
	static constexpr uint64_t FULL_MOVE_NUMBER_MASK = 0x3ffe0000000000;			// 13 bits, shifted 41 bits based on theorical maximum of 5899 moves
	static constexpr uint64_t RESERVED = 0xffc0000000000000;					// 10 bits, shifted 54 bits

	// bit shifts
	static constexpr uint8_t FROM_SHIFT = 0;
	static constexpr uint8_t TO_SHIFT = 6;
	static constexpr uint8_t PIECE_TYPE_SHIFT = 12;
	static constexpr uint8_t COLOR_SHIFT = 15;
	static constexpr uint8_t CAPTURED_PIECE_PRESENT_SHIFT = 16;
	static constexpr uint8_t CAPTURED_PIECE_SHIFT = 17;
	static constexpr uint8_t EN_PASSANT_TARGET_SQUARE_PRESENT_SHIFT = 20;
	static constexpr uint8_t EN_PASSANT_TARGET_SQUARE_SHIFT = 21;
	static constexpr uint8_t SPECIAL_MOVE_SHIFT = 27;
	static constexpr uint8_t WHITE_CASTLE_KINGSIDE_SHIFT = 30;
	static constexpr uint8_t WHITE_CASTLE_QUEENSIDE_SHIFT = 31;
	static constexpr uint8_t BLACK_CASTLE_KINGSIDE_SHIFT = 32;
	static constexpr uint8_t BLACK_CASTLE_QUEENSIDE_SHIFT = 33;
	static constexpr uint8_t HALF_MOVE_CLOCK_SHIFT = 34;
	static constexpr uint8_t FULL_MOVE_NUMBER_SHIFT = 41;
	static constexpr uint8_t RESERVED_SHIFT = 54;

	uint64_t decode(uint64_t mask, uint8_t shift) const;
	uint64_t encode(uint64_t mask, uint8_t shift, uint64_t value);
};

#endif // MOVE_HPP