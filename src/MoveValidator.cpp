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
		throw std::invalid_argument("Invalid move - The piece cannot move to the to square");
	}

	// Knights do not need a clear path and kings can only move one square
	// Pawns are not excluded because they can move two squares on their first move
	if (piece != PieceType::KNIGHT && piece != PieceType::KING)
	{
		// Check if the path is clear for all pieces except knights and kings
		if (!isPathClear(from, to, piece, color, board))
		{
			throw std::invalid_argument("Invalid move - The path is not clear");
		}
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

Bitboard MoveValidator::generatePotentialMoves(Position position, PieceType piece, Color color, Board &board)
{
	Bitboard moves = board.getAttacks(piece, color, Utility::calculateSquareNumber(position));

	if (piece == PieceType::PAWN)
	{
		return generatePotentialPawnMoves(color, position, moves);
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

Bitboard MoveValidator::generatePotentialPawnMoves(Color color, Position position, Bitboard moves)
{
	int direction = color == Color::WHITE ? -1 : 1;

	Position singleMove{position.row + direction, position.col};
	Position doubleMove{position.row + 2 * direction, position.col};

	if (singleMove.row >= 0 && singleMove.row < 8 && singleMove.col >= 0 && singleMove.col < 8)
	{
		moves.setBit(singleMove);
	}

	if (canDoublePawnPush(color, position) && doubleMove.row >= 0 && doubleMove.row < 8 && doubleMove.col >= 0 && doubleMove.col < 8)
	{
		moves.setBit(doubleMove);
	}

	return moves;
}

bool MoveValidator::canDoublePawnPush(Color color, Position position)
{
	return (color == Color::WHITE && position.row == 6) || (color == Color::BLACK && position.row == 1);
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

bool MoveValidator::isPathClear(Position from, Position to, PieceType piece, Color color, Board &board)
{
	Bitboard path = calculatePath(from, to, piece, color);
	Bitboard occupied = board.getOccupiedBitboard();

	// Check if the path is clear by checking if the intersection of the path and the occupied squares is empty
	return (path & occupied) == Bitboard();
}

Bitboard MoveValidator::calculatePath(Position from, Position to, PieceType piece, Color color)
{
	Bitboard path;

	switch (piece)
	{
		case PieceType::PAWN:
		{
			path = calculatePawnPath(from, to, color);
			break;
		}
		case PieceType::BISHOP:
		{
			path = calculateDiagonalPath(from, to);
			break;
		}
		case PieceType::ROOK:
		{
			path = calculateOrthaogonalPath(from, to);
			break;
		}
		case PieceType::QUEEN:
		{
			if (isDiagonal(from, to))
			{
				path = calculateDiagonalPath(from, to);
			}
			else
			{
				path = calculateOrthaogonalPath(from, to);
			}
			break;
		}
	}

	return path;
}

Bitboard MoveValidator::calculatePawnPath(Position from, Position to, Color color)
{
	Bitboard path;

	// if the pawn is not moving diagonally and is advancing two squares, then calculate the path
	if (!isDiagonal(from, to) && (from.row + 2 == to.row || from.row - 2 == to.row))
	{
		if (color == Color::WHITE)
		{
			if (from.row == 6 && to.row == 4)
			{
				path.setBit(Position{5, from.col});
			}
		}
		else
		{
			if (from.row == 1 && to.row == 3)
			{
				path.setBit(Position{2, from.col});
			}
		}
	}

	return path;
}

Bitboard MoveValidator::calculateDiagonalPath(Position from, Position to)
{
	Bitboard path;

	int rowIncrement = (to.row > from.row) ? 1 : -1;
	int colIncrement = (to.col > from.col) ? 1 : -1;

	// Start at the square after the from square
	int row = from.row + rowIncrement;
	int col = from.col + colIncrement;

	while (row != to.row && col != to.col)
	{
		path.setBit(Position{row, col});
		row += rowIncrement;
		col += colIncrement;
	}

	return path;
}

Bitboard MoveValidator::calculateOrthaogonalPath(Position from, Position to)
{
	Bitboard path;

	if (from.row == to.row)
	{
		int start = std::min(from.col, to.col);
		int end = std::max(from.col, to.col);

		for (int i = start + 1; i < end; i++)
		{
			path.setBit(Position{from.row, i});
		}
	}
	else
	{
		int start = std::min(from.row, to.row);
		int end = std::max(from.row, to.row);

		for (int i = start + 1; i < end; i++)
		{
			path.setBit(Position{i, from.col});
		}
	}

	return path;
}

bool MoveValidator::isDiagonal(Position from, Position to)
{
	return abs(to.row - from.row) == abs(to.col - from.col);
}

bool MoveValidator::isOrthogonal(Position from, Position to)
{
	return to.row == from.row || to.col == from.col;
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
		if (opponentPieces.getBit(Utility::calculatePosition(square)) == 1)
		{
			for (PieceType piece : {PieceType::PAWN, PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK, PieceType::QUEEN, PieceType::KING})
			{
				// Check if the opponent piece is on the square
				if (board.getPieceBitboard(piece, opponentColor).getBit(Utility::calculatePosition(square)) == 0)
				{
					continue;
				}

				// Check if the opponent piece can attack the square
				Bitboard attacks = board.getAttacks(piece, opponentColor, square);
				if (attacks.getBit(position) == true)
				{
					// Check if the path is clear
					if (piece != PieceType::KNIGHT && piece != PieceType::KING)
					{
						Bitboard path = calculatePath(Utility::calculatePosition(square), position, piece, opponentColor);
						Bitboard occupied = board.getOccupiedBitboard();

						if ((path & occupied) == Bitboard())
						{
							return true;
						}
					}
					else
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}