#ifndef BOARD_HPP
#define BOARD_HPP

#include <array>
#include <string>
#include <optional>

#include "Bitboard.hpp"
#include "enums/PieceType.hpp"
#include "enums/Color.hpp"
#include "structs/Position.hpp"

class Board
{
public:
	Board(std::string fenPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", std::string fenEnPassantTargetSquare = "-");

	Bitboard getPieceBitboard(PieceType piece, Color color) const;
	void setPieceBitboard(PieceType piece, Color color, Bitboard bitboard);
	std::optional<Position> getEnPassantTargetSquare() const;
	void setEnPassantTargetSquare(Position enPassantTargetSquare);

	// Bitboard getPawnAttackTable(Position position, Color color) const;
	// Bitboard getKnightAttackTable(Position position) const;
	// Bitboard getBishopAttackTable(Position position) const;
	// Bitboard getRookAttackTable(Position position) const;
	// Bitboard getQueenAttackTable(Position position) const;
	// Bitboard getKingAttackTable(Position position) const;

private:
	std::array<std::array<Bitboard, 6>, 2> pieceBitboards;
	std::array<std::array<Bitboard, 64>, 2> pawnAttackTable;
	std::array<Bitboard, 64> knightAttackTable;
	std::array<Bitboard, 64> bishopAttackTable;
	std::array<Bitboard, 64> rookAttackTable;
	std::array<Bitboard, 64> queenAttackTable;
	std::array<Bitboard, 64> kingAttackTable;
	std::optional<Position> enPassantTargetSquare;

	void parseFenPosition(std::string fenPosition);
	void parseFenEnPassantTargetSquare(std::string fenEnPassantTargetSquare);
	Position convertStringToPosition(std::string position);
};

#endif // BOARD_HPP