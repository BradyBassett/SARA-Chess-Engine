#include "../include/MoveValidator.hpp"
#include "../include/Utility.hpp"

#include <stdexcept>

void MoveValidator::validateMove(Position from, Position to, PieceType piece, Color friendlyColor, Game &game)
{
	Board &board = game.getBoard();

	// Check if the piece is of the correct color
	if (friendlyColor != game.getActiveColor())
	{
		throw std::invalid_argument("Invalid move - You can only move your own pieces");
	}

	// Check if the from and to squares are different
	if (from == to)
	{
		throw std::invalid_argument("Invalid move - You must move the piece to a different square");
	}

	// Check if the from square is occupied by a piece of the correct color
	if (board.getColorBitboard(friendlyColor).getBit(from) == 0)
	{
		throw std::invalid_argument("Invalid move - There is no piece on this square");
	}

	// Check if the to square is occupied by a piece of the same color
	if (board.getColorBitboard(friendlyColor).getBit(to) != 0)
	{
		throw std::invalid_argument("Invalid move - You cannot capture your own piece");
	}

	// Check if the to square is in the list of potential moves
	if (generatePotentialMoves(from, piece, friendlyColor, board).getBit(to) == 0)
	{
		throw std::invalid_argument("Invalid move - The piece cannot move to the square");
	}

	if (piece == PieceType::PAWN)
	{
		// Pawn-specific move validation
		validatePawnMove(from, to, piece, friendlyColor, board);
	}
	else if (piece == PieceType::KING)
	{
		// King-specific move validation
		validateKingMove(from, to, piece, friendlyColor, game);
	}

	if (!isValidPinnedPieceMove(from, to, friendlyColor, board))
	{
		throw std::invalid_argument("Invalid move - The piece is pinned and cannot move in that direction");
	}
}

// todo: implement
bool MoveValidator::calculateInCheck()
{
	return true;
}

Bitboard MoveValidator::findAbsolutePins(Board &board, Color friendlyColor)
{
	Bitboard pinned;
	Bitboard occupied = board.getOccupiedBitboard();
	Bitboard friendlyPieces = board.getColorBitboard(friendlyColor);
	Color opponentColor = (friendlyColor == Color::WHITE) ? Color::BLACK : Color::WHITE;
	Bitboard opponentRQ = board.getPieceBitboard(PieceType::ROOK, opponentColor) | board.getPieceBitboard(PieceType::QUEEN, opponentColor);
	Bitboard opponentBQ = board.getPieceBitboard(PieceType::BISHOP, opponentColor) | board.getPieceBitboard(PieceType::QUEEN, opponentColor);
	int kingSquare = board.getKing(friendlyColor);

	Bitboard pinner = xrayAttacks(occupied, friendlyPieces, kingSquare, PieceType::ROOK) & opponentRQ;
	while (pinner.getValue())
	{
		int pinnerSquare = pinner.bitScanForward();
		pinned |= getObstructedRay(board.getRay(pinnerSquare, kingSquare), occupied) & friendlyPieces;
		pinner &= (pinner.getValue() - 1);
	}

	pinner = xrayAttacks(occupied, friendlyPieces, kingSquare, PieceType::BISHOP) & opponentBQ;
	while (pinner.getValue())
	{
		int pinnerSquare = pinner.bitScanForward();
		pinned |= getObstructedRay(board.getRay(pinnerSquare, kingSquare), occupied) & friendlyPieces;
		pinner &= (pinner.getValue() - 1);
	}

	return pinned;
}

Bitboard MoveValidator::generatePotentialMoves(Position position, PieceType piece, Color friendlyColor, Board &board)
{
	Bitboard moves = board.getAttacks(piece, friendlyColor, Utility::calculateSquareNumber(position));

	if (piece == PieceType::PAWN)
	{
		return generatePotentialPawnMoves(friendlyColor, position, moves, board.getOccupiedBitboard());
	}
	else if (piece == PieceType::KING)
	{
		return generatePotentialKingMoves(friendlyColor, position, moves);
	}
	else
	{
		return moves;
	}
}

Bitboard MoveValidator::generatePotentialPawnMoves(Color friendlyColor, Position position, Bitboard moves, Bitboard occupied)
{
	int direction = friendlyColor == Color::WHITE ? -1 : 1;

	Position singleMove{position.row + direction, position.col};
	Position doubleMove{position.row + 2 * direction, position.col};

	if (singleMove.row >= 0 && singleMove.row < 8 && singleMove.col >= 0 && singleMove.col < 8)
	{
		moves.setBit(singleMove);
	}

	if (canDoublePawnPush(friendlyColor, position, occupied) && doubleMove.row >= 0 && doubleMove.row < 8 && doubleMove.col >= 0 && doubleMove.col < 8)
	{
		moves.setBit(doubleMove);
	}

	return moves;
}

bool MoveValidator::canDoublePawnPush(Color friendlyColor, Position position, Bitboard occupied)
{
	bool isCorrectRow = (friendlyColor == Color::WHITE && position.row == 6) || (friendlyColor == Color::BLACK && position.row == 1);
	bool isClearPath;

	if (friendlyColor == Color::WHITE)
	{
		isClearPath = occupied.getBit(Position{5, position.col}) == 0;
	}
	else
	{
		isClearPath = occupied.getBit(Position{2, position.col}) == 0;
	}

	return isCorrectRow && isClearPath;
}

Bitboard MoveValidator::generatePotentialKingMoves(Color friendlyColor, Position position, Bitboard moves)
{
	// Doesnt consider board state, only theoretically potential moves
	if (position.col == 4)
	{
		if (position.row == 0 && friendlyColor == Color::BLACK)
		{
			moves.setBit(Position{0, 6});
			moves.setBit(Position{0, 2});
		}
		else if (position.row == 7 && friendlyColor == Color::WHITE)
		{
			moves.setBit(Position{7, 6});
			moves.setBit(Position{7, 2});
		}
	}

	return moves;
}

bool MoveValidator::isDiagonal(Position from, Position to)
{
	return abs(to.row - from.row) == abs(to.col - from.col);
}

void MoveValidator::validatePawnMove(Position from, Position to, PieceType piece, Color friendlyColor, Board &board)
{
	Color opponentColor = (friendlyColor == Color::WHITE) ? Color::BLACK : Color::WHITE;

	// Check direction
	if (isDiagonal(from, to))
	{
		// Check if the pawn is capturing a piece
		if (board.getColorBitboard(opponentColor).getBit(to) != 1)
		{
			// Check if the pawn is on an en passant eligible square
			int enPassantEligibleRow = (friendlyColor == Color::WHITE) ? 3 : 4;
			if (from.row == enPassantEligibleRow)
			{
				// if the en passant target square is not set or is not the to square, then the move is invalid
				if (!board.getEnPassantTargetSquare().has_value() || board.getEnPassantTargetSquare().value() != to)
				{
					throw std::invalid_argument("Invalid move - Pawn can only capture en passant if the opponents previous move was a pawn push of 2 squares that landed adjacent to this pawn");
				}
			}
			// if not, then the move is invalid
			else
			{
				throw std::invalid_argument("Invalid move - To move diagonally, you must capture an enemy piece");
			}
		}
	}
	else
	{
		if (board.getColorBitboard(opponentColor).getBit(to) != 0)
		{
			throw std::invalid_argument("Invalid move - You cannot capture a piece by moving forward");
		}
	}
}

void MoveValidator::validateKingMove(Position from, Position to, PieceType piece, Color friendlyColor, Game &game)
{
	int castlingRow = (friendlyColor == Color::WHITE) ? 7 : 0;
	Position correctKingPosition = (friendlyColor == Color::WHITE) ? Position{7, 4} : Position{0, 4};

	// if king is on correct square, and target square is left or right two squares and on the correct castling row, then the move is a castling move
	if (from == correctKingPosition && abs(from.col - to.col) == 2 && (from.row == castlingRow && to.row == castlingRow))
	{
		validateCastlingMove(from, to, friendlyColor, game);
	}
	else
	{
		// Check if the king is moving into check
		if (isSquareAttacked(to, friendlyColor, game.getBoard()))
		{
			throw std::invalid_argument("Invalid move - The king cannot move into check");
		}
	}
}

void MoveValidator::validateCastlingMove(Position from, Position to, Color friendlyColor, Game &game)
{
	CastleRights castleRights = (friendlyColor == Color::WHITE) ? game.getWhiteCastleRights() : game.getBlackCastleRights();
	bool kingSide = castleRights.canCastleKingSide();
	bool queenSide = castleRights.canCastleQueenSide();

	if (castleRights.canCastleKingSide() || castleRights.canCastleQueenSide())
	{
		if (!castleRights.canCastleKingSide() && to.col == 6)
		{
			throw std::invalid_argument("Invalid move - The king cannot castle kingside");
		}
		if (!castleRights.canCastleQueenSide() && to.col == 2)
		{
			throw std::invalid_argument("Invalid move - The king cannot castle queenside");
		}

		Board &board = game.getBoard();

		// Check if the squares between the king and the rook are empty
		int direction = (to.col > from.col) ? 1 : -1;
		for (int i = from.col + direction; (i < 7 && i > 0); i += direction)
		{
			if (board.getOccupiedBitboard().getBit(Position{from.row, i}) != 0)
			{
				throw std::invalid_argument("Invalid move - The path between the king and the rook is not clear");
			}

			// Check if the king is castling through check
			if (isSquareAttacked(Position{from.row, i}, friendlyColor, board))
			{
				throw std::invalid_argument("Invalid move - The king cannot castle through check");
			}
		}
	}
	else
	{
		throw std::invalid_argument("Invalid move - You cannot castle");
	}
}

bool MoveValidator::isValidPinnedPieceMove(Position from, Position to, Color friendlyColor, Board &board)
{
	Bitboard pinned = findAbsolutePins(board, friendlyColor);

	// Check if the piece being moved is pinned
	if (pinned.getBit(from) == 1)
	{
		int kingSquare = board.getKing(friendlyColor);
		Position kingPosition = Utility::calculatePosition(kingSquare);
		Position pinDirection{(from.row < kingPosition.row) ? -1 : (from.row > kingPosition.row) ? 1 : 0,
							  (from.col < kingPosition.col) ? -1 : (from.col > kingPosition.col) ? 1 : 0};
		int pinningPieceSquare = -1;
		Position current = from;
		Color opponentColor = (friendlyColor == Color::WHITE) ? Color::BLACK : Color::WHITE;

		// Find the piece that is pinning the pinned piece
		while (Utility::isValidPosition(current))
		{
			current.row += pinDirection.row;
			current.col += pinDirection.col;

			if (board.getColorBitboard(opponentColor).getBit(current) == 1)
			{
				pinningPieceSquare = Utility::calculateSquareNumber(current);
				break;
			}
		}

		// Check if the move is along the pinning ray or to the pinning piece
		Bitboard ray = board.getRay(pinningPieceSquare, kingSquare);
		if (ray.getBit(to) || to == Utility::calculatePosition(pinningPieceSquare))
		{
			return true;
		}

		return false;
	}

	return true;
}

bool MoveValidator::isSquareAttacked(Position position, Color friendlyColor, Board &board)
{
    Color opponentColor = (friendlyColor == Color::WHITE) ? Color::BLACK : Color::WHITE;

    // Combine all opponent piece bitboards into a single bitboard
    Bitboard opponentPieces = board.getColorBitboard(opponentColor);

    // Iterate over all squares occupied by the opponent's pieces
    while (opponentPieces.getValue())
    {
        int square = opponentPieces.bitScanForward();
        opponentPieces.clearBit(square);

        // Iterate over all piece types
        for (PieceType piece : {PieceType::PAWN, PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK, PieceType::QUEEN, PieceType::KING})
        {
            // Check if the opponent piece is on the square
            if (board.getPieceBitboard(piece, opponentColor).getBit(square))
            {
                // Check if the opponent piece can attack the square
                Bitboard attacks = board.getAttacks(piece, opponentColor, square);
                if (attacks.getBit(position))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

Bitboard MoveValidator::xrayAttacks(Bitboard occupied, Bitboard friendlyPieces, int kingSquare, PieceType piece)
{
	Bitboard attacks = MagicBitboards::getSliderAttacks(kingSquare, occupied, piece);

	friendlyPieces &= attacks;

	return attacks ^ MagicBitboards::getSliderAttacks(kingSquare, occupied ^ friendlyPieces, piece);
}

Bitboard MoveValidator::getObstructedRay(Bitboard ray, Bitboard occupied)
{
	Bitboard obstructedRay;

	while (ray.getValue())
	{
		int square = ray.bitScanForward();
		obstructedRay.setBit(square);
		ray &= (ray.getValue() - 1);

		if (occupied.getBit(square))
		{
			break;
		}
	}

	return obstructedRay;
}