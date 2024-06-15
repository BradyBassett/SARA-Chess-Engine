#include "../include/Game.hpp"

#include <sstream>

Game::Game(std::string fen) : Game(getFenTokens(fen))
{
}

Game::Game(std::vector<std::string> fenParts) : board(fenParts[0], fenParts[3])
{
	parseActiveColor(fenParts[1]);
	parseCastlingRights(fenParts[2]);
	parsehalfMoveClock(fenParts[4]);
	parseFullMoveNumber(fenParts[5]);
}

Color Game::getActiveColor() const
{
	return activeColor;
}

void Game::setActiveColor(Color activeColor)
{
	this->activeColor = activeColor;
}

Board Game::getBoard() const
{
	return board;
}

int Game::getHalfMoveClock() const
{
	return halfMoveClock;
}

void Game::setHalfMoveClock(int halfMoveClock)
{
	this->halfMoveClock = halfMoveClock;
}

int Game::getFullMoveNumber() const
{
	return fullMoveNumber;
}

void Game::setFullMoveNumber(int fullMoveNumber)
{
	this->fullMoveNumber = fullMoveNumber;
}

CastleRights Game::getWhiteCastleRights() const
{
	return whiteCastleRights;
}

CastleRights Game::getBlackCastleRights() const
{
	return blackCastleRights;
}

void Game::switchActiveColor()
{
	activeColor == Color::WHITE ? activeColor = Color::BLACK : activeColor = Color::WHITE;
}

void Game::incrementHalfMoveClock()
{
	halfMoveClock++;
}

void Game::incrementFullMoveNumber()
{
	fullMoveNumber++;
}

std::vector<std::string> Game::getFenTokens(std::string fen)
{
	std::vector<std::string> parts;
	std::istringstream iss(fen);
	std::string part;
	while (std::getline(iss, part, ' '))
	{
		parts.push_back(part);
	}

	return parts;
}

void Game::parseActiveColor(std::string color)
{
	if (color == "w")
	{
		activeColor = Color::WHITE;
	}
	else
	{
		activeColor = Color::BLACK;
	}
}

void Game::parsehalfMoveClock(std::string halfMoveClock)
{
	this->halfMoveClock = std::stoi(halfMoveClock);
}

void Game::parseFullMoveNumber(std::string fullMoveNumber)
{
	this->fullMoveNumber = std::stoi(fullMoveNumber);
}

void Game::parseCastlingRights(std::string castlingRights)
{
    bool whiteCanCastleKingside = castlingRights.find('K') != std::string::npos;
    bool whiteCanCastleQueenside = castlingRights.find('Q') != std::string::npos;
    bool blackCanCastleKingside = castlingRights.find('k') != std::string::npos;
    bool blackCanCastleQueenside = castlingRights.find('q') != std::string::npos;

    whiteCastleRights = CastleRights(whiteCanCastleKingside, whiteCanCastleQueenside);
    blackCastleRights = CastleRights(blackCanCastleKingside, blackCanCastleQueenside);
}