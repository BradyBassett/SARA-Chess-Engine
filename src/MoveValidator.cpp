#include "../include/MoveValidator.hpp"
#include "../include/Utility.hpp"

#include <stdexcept>

void MoveValidator::validateMove(Position from, Position to, PieceType piece, Color color, Game &game)
{
	Board &board = game.getBoard();

	// Check if the piece is of the correct color
	if (color != game.getActiveColor())
	{
		throw std::invalid_argument("Invalid move - You can only move your own pieces");
	}

	// Check if the from and to squares are different
	if (from == to)
	{
		throw std::invalid_argument("Invalid move - You must move the piece to a different square");
	}

	// Check if the from square is occupied by a piece of the correct color
	if (board.getColorBitboard(color).getBit(from) == 0)
	{
		throw std::invalid_argument("Invalid move - There is no piece on this square");
	}

	// Check if the to square is occupied by a piece of the same color
	if (board.getColorBitboard(color).getBit(to) != 0)
	{
		throw std::invalid_argument("Invalid move - You cannot capture your own piece");
	}

	// Check if the to square is in the list of potential moves
	if (generatePotentialMoves(from, piece, color, board).getBit(to) == 0)
	{
		throw std::invalid_argument("Invalid move - The piece cannot move to the square");
	}

	if (piece == PieceType::PAWN)
	{
		// Pawn-specific move validation
		validatePawnMove(from, to, piece, color, board);
	}
	else if (piece == PieceType::KING)
	{
		// King-specific move validation
		validateKingMove(from, to, piece, color, game);
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

Bitboard MoveValidator::generatePotentialMoves(Position position, PieceType piece, Color color, Board &board)
{
	Bitboard moves = board.getAttacks(piece, color, Utility::calculateSquareNumber(position));

	if (piece == PieceType::PAWN)
	{
		return generatePotentialPawnMoves(color, position, moves, board.getOccupiedBitboard());
	}
	else if (piece == PieceType::KING)
	{
		return generatePotentialKingMoves(color, position, moves);
	}
	else
	{
		return moves;
	}
}

Bitboard MoveValidator::generatePotentialPawnMoves(Color color, Position position, Bitboard moves, Bitboard occupied)
{
	int direction = color == Color::WHITE ? -1 : 1;

	Position singleMove{position.row + direction, position.col};
	Position doubleMove{position.row + 2 * direction, position.col};

	if (singleMove.row >= 0 && singleMove.row < 8 && singleMove.col >= 0 && singleMove.col < 8)
	{
		moves.setBit(singleMove);
	}

	if (canDoublePawnPush(color, position, occupied) && doubleMove.row >= 0 && doubleMove.row < 8 && doubleMove.col >= 0 && doubleMove.col < 8)
	{
		moves.setBit(doubleMove);
	}

	return moves;
}

bool MoveValidator::canDoublePawnPush(Color color, Position position, Bitboard occupied)
{
	bool isCorrectRow = (color == Color::WHITE && position.row == 6) || (color == Color::BLACK && position.row == 1);
	bool isClearPath;

	if (color == Color::WHITE)
	{
		isClearPath = occupied.getBit(Position{5, position.col}) == 0;
	}
	else
	{
		isClearPath = occupied.getBit(Position{2, position.col}) == 0;
	}

	return isCorrectRow && isClearPath;
}

Bitboard MoveValidator::generatePotentialKingMoves(Color color, Position position, Bitboard moves)
{
	// Doesnt consider board state, only theoretically potential moves
	if (position.col == 4)
	{
		if (position.row == 0 && color == Color::BLACK)
		{
			moves.setBit(Position{0, 6});
			moves.setBit(Position{0, 2});
		}
		else if (position.row == 7 && color == Color::WHITE)
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

void MoveValidator::validatePawnMove(Position from, Position to, PieceType piece, Color color, Board &board)
{
	Color opponentColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;

	// Check direction
	if (isDiagonal(from, to))
	{
		// Check if the pawn is capturing a piece
		if (board.getColorBitboard(opponentColor).getBit(to) != 1)
		{
			// Check if the pawn is on an en passant eligible square
			int enPassantEligibleRow = (color == Color::WHITE) ? 3 : 4;
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

void MoveValidator::validateKingMove(Position from, Position to, PieceType piece, Color color, Game &game)
{
	int castlingRow = (color == Color::WHITE) ? 7 : 0;
	Position correctKingPosition = (color == Color::WHITE) ? Position{7, 4} : Position{0, 4};

	// if king is on correct square, and target square is left or right two squares and on the correct castling row, then the move is a castling move
	if (from == correctKingPosition && abs(from.col - to.col) == 2 && (from.row == castlingRow && to.row == castlingRow))
	{
		validateCastlingMove(from, to, color, game);
	}
	else
	{
		// Check if the king is moving into check
		if (isSquareAttacked(to, color, game.getBoard()))
		{
			throw std::invalid_argument("Invalid move - The king cannot move into check");
		}
	}
}

void MoveValidator::validateCastlingMove(Position from, Position to, Color color, Game &game)
{
	CastleRights castleRights = (color == Color::WHITE) ? game.getWhiteCastleRights() : game.getBlackCastleRights();
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
		for(int i = from.col + direction; (i < 7 && i > 0); i += direction)
		{
			if (board.getOccupiedBitboard().getBit(Position{from.row, i}) != 0)
			{
				throw std::invalid_argument("Invalid move - The path between the king and the rook is not clear");
			}

			// Check if the king is castling through check
			if (isSquareAttacked(Position{from.row, i}, color, board))
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

bool MoveValidator::isSquareAttacked(Position position, Color color, Board &board)
{
	Color opponentColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
	Bitboard opponentPieces = board.getColorBitboard(opponentColor);

	for (int square = 0; square < 64; square++)
	{
		// Check if the square is occupied by an opponent piece
		if (opponentPieces.getBit(square) == 1)
		{
			for (PieceType piece : {PieceType::PAWN, PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK, PieceType::QUEEN, PieceType::KING})
			{
				// Check if the opponent piece is on the square
				if (board.getPieceBitboard(piece, opponentColor).getBit(square) == 0)
				{
					continue;
				}

				// Check if the opponent piece can attack the square
				Bitboard attacks = board.getAttacks(piece, opponentColor, square);
				if (attacks.getBit(position) == true)
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