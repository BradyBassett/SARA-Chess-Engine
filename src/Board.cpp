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
	return pieceBitboards[static_cast<int>(color)][static_cast<int>(piece)];
}

void Board::setPieceBitboard(PieceType piece, Color color, Bitboard bitboard)
{
	pieceBitboards[static_cast<int>(color)][static_cast<int>(piece)] = bitboard;
}

std::optional<Position> Board::getEnPassantTargetSquare() const
{
	return enPassantTargetSquare;
}

void Board::setEnPassantTargetSquare(Position enPassantTargetSquare)
{
	this->enPassantTargetSquare = enPassantTargetSquare;
}

std::string Board::boardToAscii() const
{
    std::string asciiBoard = "";
	const std::string PURPLE = "\033[34m";
    const std::string RESET = "\033[0m";

    for (int row = 0; row < 8; row++)
    {
        asciiBoard += "--+---+---+---+---+---+---+---+---+\n";
        asciiBoard += PURPLE + std::to_string(8 - row) + RESET + " | ";
        for (int col = 0; col < 8; col++)
        {
            char pieceChar = ' ';
            for (Color color : {Color::WHITE, Color::BLACK})
            {
                for (PieceType piece : {PieceType::PAWN, PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK, PieceType::QUEEN, PieceType::KING})
                {
                    if (getPieceBitboard(piece, color).getBit(Position{row, col}))
                    {
                        pieceChar = pieceToChar(piece, color);
                        break;
                    }
                }
                if (pieceChar != ' ') break;
            }
            asciiBoard += pieceChar;
            asciiBoard += " | ";
        }
        asciiBoard += "\n";
    }
    asciiBoard += "--+---+---+---+---+---+---+---+---+\n ";

	for (char file = 'a'; file <= 'h'; file++)
	{
		asciiBoard += " | " + PURPLE + file + RESET;
	}

    return asciiBoard;
}

Position Board::convertStringToPosition(std::string position)
{
	return Position{8 - (position[1] - '0'), position[0] - 'a'};
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

char Board::pieceToChar(PieceType piece, Color color) const
{
	switch (piece)
	{
	case PieceType::PAWN:
		return color == Color::WHITE ? 'P' : 'p';
	case PieceType::KNIGHT:
		return color == Color::WHITE ? 'N' : 'n';
	case PieceType::BISHOP:
		return color == Color::WHITE ? 'B' : 'b';
	case PieceType::ROOK:
		return color == Color::WHITE ? 'R' : 'r';
	case PieceType::QUEEN:
		return color == Color::WHITE ? 'Q' : 'q';
	case PieceType::KING:
		return color == Color::WHITE ? 'K' : 'k';
	default:
		return ' ';
	}
}