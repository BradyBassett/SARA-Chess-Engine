#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <string>
#include <unordered_map>

#include "Board.hpp"
#include "Move.hpp"
#include "enums/Color.hpp"
#include "structs/CastleRights.hpp"

class Game
{
public:
	Game(std::string fen);
	Game(std::vector<std::string> fenParts);

	Color getActiveColor() const;
	void setActiveColor(Color activeColor);
	Board &getBoard();
	std::string getFen();
	int getHalfMoveClock() const;
	void setHalfMoveClock(int halfMoveClock);
	int getFullMoveNumber() const;
	void setFullMoveNumber(int fullMoveNumber);
	CastleRights getWhiteCastleRights() const;
	CastleRights getBlackCastleRights() const;
	Move getLastMove() const;

	void makeMove(Position from, Position to, PromotionPiece promotionPiece);
	void unmakeMove();

	std::vector<std::string> getFenTokens(std::string fen);

private:
	Color activeColor;
	Board board;
	std::vector<Move> moveHistory;
	uint8_t halfMoveClock;
	uint16_t fullMoveNumber;
	CastleRights whiteCastleRights;
	CastleRights blackCastleRights;
	bool cachedInCheckValue;
	bool hasCachedInCheckValue;

	void parseActiveColor(std::string color);
	void parsehalfMoveClock(std::string halfMoveClock);
	void parseFullMoveNumber(std::string fullMoveNumber);
	void parseCastlingRights(std::string castlingRights);
	void switchActiveColor();
	void incrementHalfMoveClock();
	void incrementFullMoveNumber();
	void addMoveToHistory(Move move);
	std::optional<PieceType> getCapturedPiece(PieceType piece, Position from, Position to);
	SpecialMove getSpecialMove(PieceType piece, Position from, Position to, std::optional<PieceType> capturedPiece, PromotionPiece promotionPiece);
	std::optional<Position> getEnPassantTargetSquare(PieceType piece, Position from, Position to, SpecialMove specialMove);
	void updateCastlingRights(PieceType piece, Color color, Position from);
	bool isInCheck();
};

#endif // GAME_HPP