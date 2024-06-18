#ifndef BOARD_HPP
#define BOARD_HPP

#include <array>
#include <string>
#include <optional>

#include "Bitboard.hpp"
#include "enums/PieceType.hpp"
#include "enums/Color.hpp"
#include "structs/Position.hpp"
#include "structs/PieceList.hpp"

class Board
{
public:
	Board(std::string fenPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", std::string fenEnPassantTargetSquare = "-");

	Bitboard getPieceBitboard(PieceType piece, Color color) const;
	void setPieceBitboard(PieceType piece, Color color, Bitboard bitboard);
	std::optional<Position> getEnPassantTargetSquare() const;
	void setEnPassantTargetSquare(Position enPassantTargetSquare);

	std::string boardToAscii() const;

private:
	std::array<std::array<Bitboard, 6>, 2> pieceBitboards;
	std::array<std::array<Bitboard, 64>, 2> pawnAttackTable;
	std::array<Bitboard, 64> knightAttackTable;
	std::array<Bitboard, 64> bishopAttackTable;
	std::array<Bitboard, 64> rookAttackTable;
	std::array<Bitboard, 64> queenAttackTable;
	std::array<Bitboard, 64> kingAttackTable;
	std::optional<Position> enPassantTargetSquare;
	std::array<PieceList, 2> pawns;
	std::array<PieceList, 2> knights;
	std::array<PieceList, 2> bishops;
	std::array<PieceList, 2> rooks;
	std::array<PieceList, 2> queens;
	std::array<int, 2> kings; // King is not a list because there can only be one king per color

	void initializePieceLists();
	void parseFenPosition(std::string fenPosition);
	void loadPieceFromFen(PieceType piece, Color color, int square);
	void parseFenEnPassantTargetSquare(std::string fenEnPassantTargetSquare);
	char pieceToChar(PieceType piece, Color color) const;
};

#endif // BOARD_HPP