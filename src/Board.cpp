#include "../include/Board.hpp"
#include "../include/enums/PieceType.hpp"

#include <map>

Board::Board(std::string fenPosition, std::string fenEnPassantTargetSquare)
{
	parseFenPosition(fenPosition);
	parseFenEnPassantTargetSquare(fenEnPassantTargetSquare);
}

Bitboard Board::getPieceBitboard(PieceType piece, Color color) const
{
	return pieceBitboards[color][piece];
}

void Board::setPieceBitboard(PieceType piece, Color color, Bitboard bitboard)
{
	pieceBitboards[color][piece] = bitboard;
}

std::optional<Position> Board::getEnPassantTargetSquare() const
{
	return enPassantTargetSquare;
}

void Board::setEnPassantTargetSquare(Position enPassantTargetSquare)
{
	this->enPassantTargetSquare = enPassantTargetSquare;
}

void Board::parseFenPosition(std::string fenPosition)
{
	int rowIndex = 0;
	int colIndex = 0;

	std::map<char, PieceType> charToPiece = {
		{'p', PieceType::PAWN},
		{'n', PieceType::KNIGHT},
		{'b', PieceType::BISHOP},
		{'r', PieceType::ROOK},
		{'q', PieceType::QUEEN},
		{'k', PieceType::KING}
	};

	for (char c : fenPosition)
	{
		if (c == '/')
		{
			rowIndex++;
			colIndex = 0;
		}
		else if (isdigit(c))
		{
			colIndex += c - '0';
		}
		else
		{
			Color color = isupper(c) ? Color::WHITE : Color::BLACK;
			PieceType piece = charToPiece[tolower(c)];

			Position position{rowIndex, colIndex};
			Bitboard temp1 = getPieceBitboard(piece, color);
			Bitboard temp2 = Bitboard(position);
			Bitboard temp3 = temp1 | temp2;
			setPieceBitboard(piece, color, getPieceBitboard(piece, color) | Bitboard(position));
			colIndex++;
		}
	}
}

void Board::parseFenEnPassantTargetSquare(std::string fenEnPassantTargetSquare)
{
	if (fenEnPassantTargetSquare == "-")
	{
		enPassantTargetSquare = std::nullopt;
	}
	else
	{
		enPassantTargetSquare = convertStringToPosition(fenEnPassantTargetSquare);
	}
}

Position Board::convertStringToPosition(std::string position)
{
	return Position{8 - (position[1] - '0'), position[0] - 'a'};
}