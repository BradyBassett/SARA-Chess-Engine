#include "../include/Board.hpp"
#include "../include/Utility.hpp"
#include "../include/PrecomputedData.hpp"

#include <map>

Board::Board(std::string fenPosition, std::string fenEnPassantTargetSquare)
{
	initializePieceLists();
	initializeAttacks();
	initializeRays();
	MagicBitboards::init();
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
			return MagicBitboards::getSliderAttacks(square, getOccupiedBitboard(), PieceType::BISHOP);
		case PieceType::ROOK:
			return MagicBitboards::getSliderAttacks(square, getOccupiedBitboard(), PieceType::ROOK);
		case PieceType::QUEEN:
			return MagicBitboards::getSliderAttacks(square, getOccupiedBitboard(), PieceType::QUEEN);
		case PieceType::KING:
			return kingAttacks[square];
		default:
			return Bitboard(0);
	}
}

Bitboard Board::getRay(int from, int to) const
{
	return rays[from][to];
}

std::optional<PieceType> Board::getPiece(Position position, Color color) const
{
	for (PieceType piece : {PieceType::PAWN, PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK, PieceType::QUEEN, PieceType::KING})
	{
		if (getPieceBitboard(piece, color).getBit(position))
		{
			return piece;
		}
	}

	return std::nullopt;
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

	// Set the piece bitboard on the to square and update the piece list
	Position to = move.getTo();
	SpecialMove specialMove = move.getSpecialMove();
	if (specialMove == SpecialMove::PROMOTION)
	{
		// Remove the pawn from the piece list
		pawns[static_cast<int>(color)].removePiece(Utility::calculateSquareNumber(from));

		// Add the promoted piece to the piece list and set the piece bitboard
		switch (move.getPromotionPiece())
		{
			case PromotionPiece::QUEEN:
			{
				queens[static_cast<int>(color)].addPiece(Utility::calculateSquareNumber(to));
				setPieceBitboard(PieceType::QUEEN, color, getPieceBitboard(PieceType::QUEEN, color) | Bitboard(to));
				break;
			}
			case PromotionPiece::ROOK:
			{
				rooks[static_cast<int>(color)].addPiece(Utility::calculateSquareNumber(to));
				setPieceBitboard(PieceType::ROOK, color, getPieceBitboard(PieceType::ROOK, color) | Bitboard(to));
				break;
			}
			case PromotionPiece::BISHOP:
			{
				bishops[static_cast<int>(color)].addPiece(Utility::calculateSquareNumber(to));
				setPieceBitboard(PieceType::BISHOP, color, getPieceBitboard(PieceType::BISHOP, color) | Bitboard(to));
				break;
			}
			case PromotionPiece::KNIGHT:
			{
				knights[static_cast<int>(color)].addPiece(Utility::calculateSquareNumber(to));
				setPieceBitboard(PieceType::KNIGHT, color, getPieceBitboard(PieceType::KNIGHT, color) | Bitboard(to));
				break;
			}
			default:
				break;
		}
	}
	else
	{
		setPieceBitboard(piece, color, getPieceBitboard(piece, color) | Bitboard(to));
		updatePieceList(piece, color, Utility::calculateSquareNumber(from), Utility::calculateSquareNumber(to), false);
	}

	// Update the captured piece bitboard and remove the piece from the piece lists
	std::optional<PieceType> capturedPiece = move.getCapturedPiece();
	if (capturedPiece.has_value())
	{
		Color capturedPieceColor = color == Color::WHITE ? Color::BLACK : Color::WHITE;
		Bitboard capturedPieceBitboard = getPieceBitboard(capturedPiece.value(), capturedPieceColor);

		if (specialMove == SpecialMove::EN_PASSANT)
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

	// Update the en passant target square on double pawn pushes
	if (specialMove == SpecialMove::DOUBLE_PAWN_PUSH)
	{
		Position enPassantTargetSquare = Position{(to.row) + (color == Color::WHITE ? 1 : -1), from.col};
		setEnPassantTargetSquare(enPassantTargetSquare);
	}

	// Handle castling
	if (specialMove == SpecialMove::KINGSIDE_CASTLE || specialMove == SpecialMove::QUEENSIDE_CASTLE)
	{
		// Move the appropriate rook
		Position rookFrom = Position{from.row, (specialMove == SpecialMove::KINGSIDE_CASTLE ? 7 : 0)};
		Position rookTo = Position{from.row, (specialMove == SpecialMove::KINGSIDE_CASTLE ? 5 : 3)};

		// Clear the rook bitboard from the from square and set it on the to square
		setPieceBitboard(PieceType::ROOK, color, getPieceBitboard(PieceType::ROOK, color) & ~Bitboard(rookFrom));
		setPieceBitboard(PieceType::ROOK, color, getPieceBitboard(PieceType::ROOK, color) | Bitboard(rookTo));

		// Update the rook piece list
		updatePieceList(PieceType::ROOK, color, Utility::calculateSquareNumber(rookFrom), Utility::calculateSquareNumber(rookTo), false);
	}
}

void Board::unmovePiece(Move move)
{
	Position from = move.getFrom();
	PieceType piece = move.getPieceType();
	Color color = move.getColor();
	Position to = move.getTo();
	SpecialMove specialMove = move.getSpecialMove();

	// Clear the piece bitboard from the to square and update the piece list
	if (specialMove == SpecialMove::PROMOTION)
	{
		// Add the pawn back to the piece list
		pawns[static_cast<int>(color)].addPiece(Utility::calculateSquareNumber(from));

		// Remove the promoted piece from the piece list and clear the piece bitboard
		switch (move.getPromotionPiece())
		{
			case PromotionPiece::QUEEN:
			{
				queens[static_cast<int>(color)].removePiece(Utility::calculateSquareNumber(to));
				setPieceBitboard(PieceType::QUEEN, color, getPieceBitboard(PieceType::QUEEN, color) & ~Bitboard(to));
				break;
			}
			case PromotionPiece::ROOK:
			{
				rooks[static_cast<int>(color)].removePiece(Utility::calculateSquareNumber(to));
				setPieceBitboard(PieceType::ROOK, color, getPieceBitboard(PieceType::ROOK, color) & ~Bitboard(to));
				break;
			}
			case PromotionPiece::BISHOP:
			{
				bishops[static_cast<int>(color)].removePiece(Utility::calculateSquareNumber(to));
				setPieceBitboard(PieceType::BISHOP, color, getPieceBitboard(PieceType::BISHOP, color) & ~Bitboard(to));
				break;
			}
			case PromotionPiece::KNIGHT:
			{
				knights[static_cast<int>(color)].removePiece(Utility::calculateSquareNumber(to));
				setPieceBitboard(PieceType::KNIGHT, color, getPieceBitboard(PieceType::KNIGHT, color) & ~Bitboard(to));
				break;
			}
			default:
				break;
		}
	}
	else
	{
		// Clear the piece bitboard from the to square and update the piece list
		setPieceBitboard(piece, color, getPieceBitboard(piece, color) & ~Bitboard(to));
		updatePieceList(piece, color, Utility::calculateSquareNumber(to), Utility::calculateSquareNumber(from), false);
	}

	// Set the piece bitboard back on the from square
	setPieceBitboard(piece, color, getPieceBitboard(piece, color) | Bitboard(from));

	// If a piece was captured, set the captured piece bitboard and update the piece list
	std::optional<PieceType> capturedPiece = move.getCapturedPiece();
	if (capturedPiece.has_value())
	{
		Color capturedPieceColor = color == Color::WHITE ? Color::BLACK : Color::WHITE;
		Bitboard capturedPieceBitboard = getPieceBitboard(capturedPiece.value(), capturedPieceColor);
		Position capturedPiecePosition = to;

		if (specialMove == SpecialMove::EN_PASSANT)
		{
			capturedPiecePosition = Position{to.row + (color == Color::WHITE ? 1 : -1), to.col};
		}

		setPieceBitboard(capturedPiece.value(), capturedPieceColor, capturedPieceBitboard | Bitboard(capturedPiecePosition));
		PieceList capturedPieceList = getPieceList(capturedPiece.value(), capturedPieceColor);
		capturedPieceList.addPiece(Utility::calculateSquareNumber(capturedPiecePosition));
		setPieceList(capturedPiece.value(), capturedPieceColor, capturedPieceList);
	}

	// Set the en passant target square back
	setEnPassantTargetSquare(move.getEnPassantTargetSquare());

	if (specialMove == SpecialMove::KINGSIDE_CASTLE || specialMove == SpecialMove::QUEENSIDE_CASTLE)
	{
		// Move the appropriate rook
		Position rookFrom = Position{from.row, (specialMove == SpecialMove::KINGSIDE_CASTLE ? 5 : 3)};
		Position rookTo = Position{from.row, (specialMove == SpecialMove::KINGSIDE_CASTLE ? 7 : 0)};

		// Clear the rook bitboard from the to square and set it on the to square
		setPieceBitboard(PieceType::ROOK, color, getPieceBitboard(PieceType::ROOK, color) & ~Bitboard(rookFrom));
		setPieceBitboard(PieceType::ROOK, color, getPieceBitboard(PieceType::ROOK, color) | Bitboard(rookTo));

		// Update the rook piece list
		updatePieceList(PieceType::ROOK, color, Utility::calculateSquareNumber(rookFrom), Utility::calculateSquareNumber(rookTo), false);
	}
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
	pawnAttacks = PrecomputedData::pawnAttacks;
	knightAttacks = PrecomputedData::knightAttacks;
	kingAttacks = PrecomputedData::kingAttacks;
}

// ? Might be worth in the future to remove duplicate ray calculations, ex a1-h1 and h1-a1
void Board::initializeRays()
{
	for (int from = 0; from < 64; from++)
	{
		for (int to = 0; to < 64; to++)
		{
			if (isValidRay(from, to))
			{
				rays[from][to] = calculateRay(from, to);
			}
			else
			{
				rays[from][to] = Bitboard(0);
			}
		}
	}
}

bool Board::isValidRay(int from, int to)
{
	Position fromPosition = Utility::calculatePosition(from);
	Position toPosition = Utility::calculatePosition(to);

	if (fromPosition.row == toPosition.row || fromPosition.col == toPosition.col)
	{
		return true;
	}

	int rowDiff = abs(fromPosition.row - toPosition.row);
	int colDiff = abs(fromPosition.col - toPosition.col);

	return rowDiff == colDiff;
}

Bitboard Board::calculateRay(int from, int to)
{
    Position fromPosition = Utility::calculatePosition(from);
    Position toPosition = Utility::calculatePosition(to);

    int rowDiff = toPosition.row - fromPosition.row;
    int colDiff = toPosition.col - fromPosition.col;

    int rowDirection = rowDiff > 0 ? 1 : (rowDiff < 0 ? -1 : 0);
    int colDirection = colDiff > 0 ? 1 : (colDiff < 0 ? -1 : 0);

    Bitboard ray = Bitboard(0);
    Position current = {fromPosition.row + rowDirection, fromPosition.col + colDirection};
    while (current.row != toPosition.row || current.col != toPosition.col)
    {
        ray |= Bitboard(current);
        current = {current.row + rowDirection, current.col + colDirection};
    }

    return ray;
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

void Board::updatePieceList(PieceType piece, Color color, int from, int to, bool isRemoved)
{
	if (piece == PieceType::KING) // The king is not in the piece lists and therefore cannot be updated
	{
		// King cannot be removed so we only need to update the king's position
		if (!isRemoved)
			{
				setKing(color, to);
			}

		return;
	}

	PieceList pieceList = getPieceList(piece, color);

	if (isRemoved)
	{
		pieceList.removePiece(to);
	}
	else
	{
		pieceList.movePiece(from, to);
	}

	setPieceList(piece, color, pieceList);
}