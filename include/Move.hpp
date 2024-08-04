#ifndef MOVE_HPP
#define MOVE_HPP

#include <optional>
#include <cstdint>

#include "../include/structs/Position.hpp"
#include "../include/structs/CastleRights.hpp"
#include "../include/enums/Color.hpp"
#include "../include/enums/PieceType.hpp"
#include "../include/enums/SpecialMove.hpp"
#include "../include/enums/PromotionPiece.hpp"

class Move
{
public:
	Move(Position from, Position to, PieceType pieceType, Color color,
		 std::optional<PieceType> capturedPiece, std::optional<Position> enPassantTargetSquare,
		 SpecialMove specialMove, PromotionPiece promotionPiece, CastleRights whiteCastleRights,
		 CastleRights blackCastleRights, uint8_t halfMoveClock, uint16_t fullMoveNumber);

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
	PromotionPiece getPromotionPiece() const;
	void setPromotionPiece(PromotionPiece promotionPiece);
	CastleRights getWhiteCastleRights() const;
	void setWhiteCastleRights(CastleRights whiteCastleRights);
	CastleRights getBlackCastleRights() const;
	void setBlackCastleRights(CastleRights blackCastleRights);
	uint8_t getHalfMoveClock() const;
	void setHalfMoveClock(uint8_t halfMoveClock);
	uint16_t getFullMoveNumber() const;
	void setFullMoveNumber(uint16_t fullMoveNumber);

	bool operator==(const Move &other) const;


private:
	uint64_t move;

	// bit masks
	static constexpr uint64_t FROM_MASK = 0x3f;									// 6 bits
	static constexpr uint64_t TO_MASK = 0xfc0;									// 6 bits
	static constexpr uint64_t PIECE_TYPE_MASK = 0x7000;							// 3 bits
	static constexpr uint64_t COLOR_MASK = 0x8000;								// 1 bit
	static constexpr uint64_t CAPTURED_PIECE_PRESENT_MASK = 0x10000;			// 1 bit
	static constexpr uint64_t CAPTURED_PIECE_MASK = 0xe0000;					// 3 bits
	static constexpr uint64_t EN_PASSANT_TARGET_SQUARE_PRESENT_MASK = 0x100000; // 1 bit
	static constexpr uint64_t EN_PASSANT_TARGET_SQUARE_MASK = 0x7e00000;		// 6 bits
	static constexpr uint64_t SPECIAL_MOVE_MASK = 0x38000000;					// 3 bits
	static constexpr uint64_t PROMOTION_PIECE_MASK = 0x1c0000000;				// 3 bits
	static constexpr uint64_t WHITE_CASTLE_KINGSIDE_MASK = 0x200000000;			// 1 bit
	static constexpr uint64_t WHITE_CASTLE_QUEENSIDE_MASK = 0x400000000;		// 1 bit
	static constexpr uint64_t BLACK_CASTLE_KINGSIDE_MASK = 0x800000000;			// 1 bit
	static constexpr uint64_t BLACK_CASTLE_QUEENSIDE_MASK = 0x1000000000;		// 1 bit
	static constexpr uint64_t HALF_MOVE_CLOCK_MASK = 0xfe000000000;				// 7 bits - bits based on maximum of 100 half moves
	static constexpr uint64_t FULL_MOVE_NUMBER_MASK = 0x1fff00000000000;		// 13 bits - bits based on theorical maximum of 5899 moves
	static constexpr uint64_t RESERVED = 0xfe00000000000000;					// 7 bits

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
	static constexpr uint8_t PROMOTION_PIECE_SHIFT = 30;
	static constexpr uint8_t WHITE_CASTLE_KINGSIDE_SHIFT = 33;
	static constexpr uint8_t WHITE_CASTLE_QUEENSIDE_SHIFT = 34;
	static constexpr uint8_t BLACK_CASTLE_KINGSIDE_SHIFT = 35;
	static constexpr uint8_t BLACK_CASTLE_QUEENSIDE_SHIFT = 36;
	static constexpr uint8_t HALF_MOVE_CLOCK_SHIFT = 37;
	static constexpr uint8_t FULL_MOVE_NUMBER_SHIFT = 44;
	static constexpr uint8_t RESERVED_SHIFT = 57;

	uint64_t decode(uint64_t mask, uint8_t shift) const;
	uint64_t encode(uint64_t mask, uint8_t shift, uint64_t value);
};

#endif // MOVE_HPP