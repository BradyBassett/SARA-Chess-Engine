#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <string>
#include <unordered_map>

#include "Board.hpp"
#include "enums/Color.hpp"
#include "structs/Move.hpp"
#include "structs/CastleRights.hpp"

class Game
{
public:
	Game(std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	Game(std::vector<std::string> fenParts);

	Color getActiveColor() const;
	void setActiveColor(Color activeColor);
	Board getBoard() const;
	int getHalfMoveClock() const;
	void setHalfMoveClock(int halfMoveClock);
	int getFullMoveNumber() const;
	void setFullMoveNumber(int fullMoveNumber);
	CastleRights getWhiteCastleRights() const;
	CastleRights getBlackCastleRights() const;

	void switchActiveColor();
	void incrementHalfMoveClock();
	void incrementFullMoveNumber();

private:
	Color activeColor;
	Board board;
	std::vector<Move> moveHistory;
	std::unordered_map<std::string, int> gameStateHistory;
	int halfMoveClock;
	int fullMoveNumber;
	CastleRights whiteCastleRights;
	CastleRights blackCastleRights;

	std::vector<std::string> getFenTokens(std::string fen);
	void parseActiveColor(std::string color);
	void parsehalfMoveClock(std::string halfMoveClock);
	void parseFullMoveNumber(std::string fullMoveNumber);
	void parseCastlingRights(std::string castlingRights);
};

#endif // GAME_HPP