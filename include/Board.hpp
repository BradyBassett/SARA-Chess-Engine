#ifndef BOARD_HPP
#define BOARD_HPP

#include <array>
#include <string>
#include <optional>

#include "Bitboard.hpp"
#include "Move.hpp"
#include "MagicBitboards.hpp"
#include "enums/PieceType.hpp"
#include "enums/Color.hpp"
#include "structs/Position.hpp"
#include "structs/PieceList.hpp"

class Board
{
public:
	Board(std::string fenPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", std::string fenEnPassantTargetSquare = "-");
	Board(std::string whitePawnsPath, std::string blackPawnsPath, std::string knightsPath, std::string kingsPath, std::string fenPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", std::string fenEnPassantTargetSquare = "-");

	Bitboard getPieceBitboard(PieceType piece, Color color) const;
	void setPieceBitboard(PieceType piece, Color color, Bitboard bitboard);
	Bitboard getColorBitboard(Color color) const;
	Bitboard getOccupiedBitboard() const;
	std::optional<Position> getEnPassantTargetSquare() const;
	void setEnPassantTargetSquare(std::optional<Position> enPassantTargetSquare);
	PieceList getPieceList(PieceType piece, Color color) const;
	void setPieceList(PieceType piece, Color color, PieceList pieceList);
	PieceList getPawns(Color color) const;
	void setPawns(Color color, PieceList pawns);
	PieceList getKnights(Color color) const;
	void setKnights(Color color, PieceList knights);
	PieceList getBishops(Color color) const;
	void setBishops(Color color, PieceList bishops);
	PieceList getRooks(Color color) const;
	void setRooks(Color color, PieceList rooks);
	PieceList getQueens(Color color) const;
	void setQueens(Color color, PieceList queens);
	int getKing(Color color) const;
	void setKing(Color color, int king);
	Bitboard getAttacks(PieceType piece, Color color, int square) const;
	std::optional<PieceType> getPiece(Position position, Color color) const;

	std::string boardToAscii() const;
	std::string getFenPosition() const;
	std::string getFenEnPassantTargetSquare() const;
	void movePiece(Move move);

private:
	std::array<std::array<Bitboard, 6>, 2> pieceBitboards;
	std::optional<Position> enPassantTargetSquare;
	std::array<PieceList, 2> pawns;
	std::array<PieceList, 2> knights;
	std::array<PieceList, 2> bishops;
	std::array<PieceList, 2> rooks;
	std::array<PieceList, 2> queens;
	std::array<int, 2> kings;							 // King is not a list because there can only be one king per color
	std::array<std::array<Bitboard, 64>, 2> pawnAttacks; // Pawn attacks are different for each color
	std::array<Bitboard, 64> knightAttacks;
	std::array<Bitboard, 64> kingAttacks;
	// rookAttacks bishopAttacks, and queenAttacks are not stored because they are calculated using Magic Bitboards

	void initializePieceLists();
	void initializeAttacks(std::string whitePawnsPath = "data/whitePawnAttacks.json", std::string blackPawnsPath = "data/blackPawnAttacks.json", std::string knightsPath = "data/knightAttacks.json", std::string kingsPath = "data/kingAttacks.json");
	void parseFenPosition(std::string fenPosition);
	void loadPieceFromFen(PieceType piece, Color color, int square);
	void parseFenEnPassantTargetSquare(std::string fenEnPassantTargetSquare);
	char pieceToChar(PieceType piece, Color color) const;
	void updatePieceList(PieceType piece, Color color, int from, int to, bool isRemoved);
};

#endif // BOARD_HPP