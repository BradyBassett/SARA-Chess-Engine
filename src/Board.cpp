#include "../include/Board.hpp"
#include "../include/MoveGenerator.hpp"
#include "../include/enums/PieceType.hpp"
#include "../include/Utility.hpp"

#include <map>

Board::Board(std::string fenPosition, std::string fenEnPassantTargetSquare)
{
	initializePieceLists();
	initializeAttacks();
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

Bitboard Board::getColorBitboard(Color color) const
{
	Bitboard colorBitboard = Bitboard(0);
	for (PieceType piece : {PieceType::PAWN, PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK, PieceType::QUEEN, PieceType::KING})
	{
		colorBitboard |= getPieceBitboard(piece, color);
	}
	return colorBitboard;
}

Bitboard Board::getOccupiedBitboard() const
{
	return getColorBitboard(Color::WHITE) | getColorBitboard(Color::BLACK);
}

std::optional<Position> Board::getEnPassantTargetSquare() const
{
	return enPassantTargetSquare;
}

void Board::setEnPassantTargetSquare(std::optional<Position> enPassantTargetSquare)
{
	this->enPassantTargetSquare = enPassantTargetSquare;
}

PieceList Board::getPieceList(PieceType piece, Color color) const
{
	switch (piece)
	{
		case PieceType::PAWN:
			return getPawns(color);
		case PieceType::KNIGHT:
			return getKnights(color);
		case PieceType::BISHOP:
			return getBishops(color);
		case PieceType::ROOK:
			return getRooks(color);
		case PieceType::QUEEN:
			return getQueens(color);
		default:
			return PieceList(0);
	}
}

void Board::setPieceList(PieceType piece, Color color, PieceList pieceList)
{
	switch (piece)
	{
		case PieceType::PAWN:
			setPawns(color, pieceList);
			break;
		case PieceType::KNIGHT:
			setKnights(color, pieceList);
			break;
		case PieceType::BISHOP:
			setBishops(color, pieceList);
			break;
		case PieceType::ROOK:
			setRooks(color, pieceList);
			break;
		case PieceType::QUEEN:
			setQueens(color, pieceList);
			break;
		default:
			break;
	}
}

PieceList Board::getPawns(Color color) const
{
	return pawns[static_cast<int>(color)];
}

void Board::setPawns(Color color, PieceList pawns)
{
	this->pawns[static_cast<int>(color)] = pawns;
}

PieceList Board::getKnights(Color color) const
{
	return knights[static_cast<int>(color)];
}

void Board::setKnights(Color color, PieceList knights)
{
	this->knights[static_cast<int>(color)] = knights;
}

PieceList Board::getBishops(Color color) const
{
	return bishops[static_cast<int>(color)];
}

void Board::setBishops(Color color, PieceList bishops)
{
	this->bishops[static_cast<int>(color)] = bishops;
}

PieceList Board::getRooks(Color color) const
{
	return rooks[static_cast<int>(color)];
}

void Board::setRooks(Color color, PieceList rooks)
{
	this->rooks[static_cast<int>(color)] = rooks;
}

PieceList Board::getQueens(Color color) const
{
	return queens[static_cast<int>(color)];
}

void Board::setQueens(Color color, PieceList queens)
{
	this->queens[static_cast<int>(color)] = queens;
}

int Board::getKing(Color color) const
{
	return kings[static_cast<int>(color)];
}

void Board::setKing(Color color, int king)
{
	kings[static_cast<int>(color)] = king;
}

Bitboard Board::getAttacks(PieceType piece, Color color, int square) const
{
	switch (piece)
	{
		case PieceType::PAWN:
			return pawnAttacks[static_cast<int>(color)][square];
		case PieceType::KNIGHT:
			return knightAttacks[square];
		case PieceType::BISHOP:
			return bishopAttacks[square];
		case PieceType::ROOK:
			return rookAttacks[square];
		case PieceType::QUEEN:
			return queenAttacks[square];
		case PieceType::KING:
			return kingAttacks[square];
		default:
			return Bitboard(0);
	}
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

std::string Board::getFenPosition() const
{
	std::string fenPosition = "";

	for (int row = 0; row < 8; row++)
	{
		int emptySquares = 0;

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

			if (pieceChar == ' ')
			{
				emptySquares++;
			}
			else
			{
				if (emptySquares > 0)
				{
					fenPosition += std::to_string(emptySquares);
					emptySquares = 0;
				}
				fenPosition += pieceChar;
			}
		}

		if (emptySquares > 0)
		{
			fenPosition += std::to_string(emptySquares);
		}

		if (row < 7)
		{
			fenPosition += "/";
		}
	}

	return fenPosition;
}

std::string Board::getFenEnPassantTargetSquare() const
{
	if (enPassantTargetSquare.has_value())
	{
		return Utility::convertPositionToString(enPassantTargetSquare.value());
	}
	else
	{
		return "-";
	}
}

void Board::movePiece(Move move)
{
	// Clear the piece bitboard from the from square
	Position from = move.getFrom();
	PieceType piece = move.getPieceType();
	Color color = move.getColor();
	setPieceBitboard(piece, color, getPieceBitboard(piece, color) & ~Bitboard(from));

	// Set the piece bitboard on the to square
	Position to = move.getTo();
	if (move.getSpecialMove() == SpecialMove::PROMOTION)
	{
		switch (move.getPromotionPiece())
		{
			case PromotionPiece::QUEEN:
				setPieceBitboard(PieceType::QUEEN, color, getPieceBitboard(PieceType::QUEEN, color) | Bitboard(to));
				break;
			case PromotionPiece::ROOK:
				setPieceBitboard(PieceType::ROOK, color, getPieceBitboard(PieceType::ROOK, color) | Bitboard(to));
				break;
			case PromotionPiece::BISHOP:
				setPieceBitboard(PieceType::BISHOP, color, getPieceBitboard(PieceType::BISHOP, color) | Bitboard(to));
				break;
			case PromotionPiece::KNIGHT:
				setPieceBitboard(PieceType::KNIGHT, color, getPieceBitboard(PieceType::KNIGHT, color) | Bitboard(to));
				break;
			default:
				break;
		}
	}
	else
	{
		setPieceBitboard(piece, color, getPieceBitboard(piece, color) | Bitboard(to));
	}

	// Update the piece list
	updatePieceList(piece, color, Utility::calculateSquareNumber(from), Utility::calculateSquareNumber(to), false);

	// Update the captured piece bitboard and remove the piece from the piece lists
	std::optional<PieceType> capturedPiece = move.getCapturedPiece();
	if (capturedPiece.has_value())
	{
		Color capturedPieceColor = color == Color::WHITE ? Color::BLACK : Color::WHITE;
		Bitboard capturedPieceBitboard = getPieceBitboard(capturedPiece.value(), capturedPieceColor);

		if (move.getSpecialMove() == SpecialMove::EN_PASSANT)
		{
			Position enPassantTargetSquare = move.getEnPassantTargetSquare().value();
			Position capturedPiecePosition = Position{enPassantTargetSquare.row + (color == Color::WHITE ? 1 : -1), enPassantTargetSquare.col};
			setPieceBitboard(capturedPiece.value(), capturedPieceColor, capturedPieceBitboard & ~Bitboard(capturedPiecePosition));
			updatePieceList(capturedPiece.value(), capturedPieceColor, Utility::calculateSquareNumber(from), Utility::calculateSquareNumber(capturedPiecePosition), true);

			// Clear the en passant target square
			setEnPassantTargetSquare(std::nullopt);
		}
		else
		{
			setPieceBitboard(capturedPiece.value(), capturedPieceColor, capturedPieceBitboard & ~Bitboard(to));
			updatePieceList(capturedPiece.value(), capturedPieceColor, Utility::calculateSquareNumber(from), Utility::calculateSquareNumber(to), true);
		}
	}

	// TODO - Update the en passant target square on double pawn pushes

	// TODO: Handle castling
}

void Board::initializePieceLists()
{
	for (int color = 0; color < 2; color++)
	{
		pawns[color] = PieceList(8);
		knights[color] = PieceList(10);
		bishops[color] = PieceList(10);
		rooks[color] = PieceList(10);
		queens[color] = PieceList(9);
	}
}

void Board::initializeAttacks()
{
	for (int square = 0; square < 64; square++)
	{
		Position position = Utility::calculatePosition(square);

		pawnAttacks[0][square] = MoveGenerator::generateAttacks(PieceType::PAWN, Color::WHITE, position);
		pawnAttacks[1][square] = MoveGenerator::generateAttacks(PieceType::PAWN, Color::BLACK, position);

		knightAttacks[square] = MoveGenerator::generateAttacks(PieceType::KNIGHT, Color::WHITE, position);
		bishopAttacks[square] = MoveGenerator::generateAttacks(PieceType::BISHOP, Color::WHITE, position);
		rookAttacks[square] = MoveGenerator::generateAttacks(PieceType::ROOK, Color::WHITE, position);
		queenAttacks[square] = MoveGenerator::generateAttacks(PieceType::QUEEN, Color::WHITE, position);
		kingAttacks[square] = MoveGenerator::generateAttacks(PieceType::KING, Color::WHITE, position);
	}
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
			int square = Utility::calculateSquareNumber(position);
			setPieceBitboard(piece, color, getPieceBitboard(piece, color) | Bitboard(position));
			loadPieceFromFen(piece, color, square);
			colIndex++;
		}
	}
}

void Board::loadPieceFromFen(PieceType piece, Color color, int square)
{
	switch (piece)
		{
			case PieceType::PAWN:
				pawns[static_cast<int>(color)].addPiece(square);
				break;
			case PieceType::KNIGHT:
				knights[static_cast<int>(color)].addPiece(square);
				break;
			case PieceType::BISHOP:
				bishops[static_cast<int>(color)].addPiece(square);
				break;
			case PieceType::ROOK:
				rooks[static_cast<int>(color)].addPiece(square);
				break;
			case PieceType::QUEEN:
				queens[static_cast<int>(color)].addPiece(square);
				break;
			case PieceType::KING:
				kings[static_cast<int>(color)] = square;
				break;
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
		enPassantTargetSquare = Utility::convertStringToPosition(fenEnPassantTargetSquare);
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

void Board::updatePieceList(PieceType piece, Color color, int from, int to, bool isCapture)
{
	if (piece == PieceType::KING) // The king is not in the piece lists and therefore cannot be updated
	{
		if (!isCapture)
			{
				setKing(color, to);
			}
	}

	PieceList pieceList = getPieceList(piece, color);

	if (isCapture)
	{
		pieceList.removePiece(to);
	}
	else
	{
		pieceList.movePiece(from, to);
	}

	setPieceList(piece, color, pieceList);
}