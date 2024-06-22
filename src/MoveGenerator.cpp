#include "../include/MoveGenerator.hpp"

#include <array>
#include <vector>

Bitboard MoveGenerator::generateAttacks(PieceType piece, Color color, Position position)
{
	switch (piece)
	{
	case PieceType::PAWN:
		return generatePawnAttacks(color, position);
	case PieceType::KNIGHT:
		return generateKnightAttacks(position);
	case PieceType::BISHOP:
	case PieceType::ROOK:
	case PieceType::QUEEN:
		return generateSlidingAttacks(piece, position);
	case PieceType::KING:
		return generateKingAttacks(position);
	default:
		return 0;
	}
}

Bitboard MoveGenerator::generatePotentialMoves(PieceType piece, Color color, Position position)
{
	switch (piece)
	{
	case PieceType::PAWN:
		return generatePotentialPawnMoves(color, position);
	case PieceType::KNIGHT:
		return generateKnightAttacks(position);
	case PieceType::BISHOP:
	case PieceType::ROOK:
	case PieceType::QUEEN:
		return generateSlidingAttacks(piece, position);
	case PieceType::KING:
		return generatePotentialKingMoves(color, position);
	default:
		return 0;
	}
}

Bitboard MoveGenerator::generatePotentialPawnMoves(Color color, Position position)
{
	Bitboard moves = generatePawnAttacks(color, position);

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

Bitboard MoveGenerator::generatePotentialKingMoves(Color color, Position position)
{
	Bitboard moves = generateKingAttacks(position);

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

Bitboard MoveGenerator::generatePawnAttacks(Color color, Position position)
{
	Bitboard attacks{0x0ULL};

	int direction = color == Color::WHITE ? -1 : 1;

	const std::array<Position, 2> targetPositions = {
		Position{position.row + direction, position.col - 1},
		Position{position.row + direction, position.col + 1}
	};

	for (const Position &targetPosition : targetPositions)
	{
		if (targetPosition.row >= 0 && targetPosition.row < 8 && targetPosition.col >= 0 && targetPosition.col < 8)
		{
			attacks.setBit(targetPosition);
		}
	}

	return attacks;
}

Bitboard MoveGenerator::generateKnightAttacks(Position position)
{
	Bitboard attacks{0x0ULL};
	const std::array<Position, 8> moves = {
		Position{2, 1}, Position{1, 2}, Position{-1, 2}, Position{-2, 1}, Position{-2, -1}, Position{-1, -2}, Position{1, -2}, Position{2, -1}
	};

	for (const Position &move : moves)
	{
		Position targetPosition{position.row + move.row, position.col + move.col};

		if (targetPosition.row >= 0 && targetPosition.row < 8 && targetPosition.col >= 0 && targetPosition.col < 8)
		{
			attacks.setBit(targetPosition);
		}
	}

	return attacks;
}

Bitboard MoveGenerator::generateSlidingAttacks(PieceType piece, Position position)
{
	Bitboard attacks{0x0ULL};

	std::array<Position, 4> diagonalDirections = {
		Position{1, 1}, Position{1, -1}, Position{-1, 1}, Position{-1, -1},
	};
	std::array<Position, 4> orthagonalDirections = {
		Position{1, 0}, Position{0, 1}, Position{-1, 0}, Position{0, -1}
	};

	std::vector<Position> directions;

	// Add the correct directions based on the piece type
	switch (piece)
	{
		case PieceType::BISHOP:
			directions.insert(directions.end(), diagonalDirections.begin(), diagonalDirections.end());
			break;
		case PieceType::ROOK:
			directions.insert(directions.end(), orthagonalDirections.begin(), orthagonalDirections.end());
			break;
		case PieceType::QUEEN:
			directions.insert(directions.end(), diagonalDirections.begin(), diagonalDirections.end());
			directions.insert(directions.end(), orthagonalDirections.begin(), orthagonalDirections.end());
			break;
	}

	for (const Position &direction : directions)
	{
		Position targetPosition{position.row + direction.row, position.col + direction.col};

		while (targetPosition.row >= 0 && targetPosition.row < 8 && targetPosition.col >= 0 && targetPosition.col < 8)
		{
			attacks.setBit(targetPosition);
			targetPosition.row += direction.row;
			targetPosition.col += direction.col;
		}
	}

	return attacks;
}

Bitboard MoveGenerator::generateKingAttacks(Position position)
{
	Bitboard attacks{0x0ULL};
	const std::array<Position, 8> directions = {
		Position{1, 0}, Position{1, 1}, Position{0, 1}, Position{-1, 1}, Position{-1, 0}, Position{-1, -1}, Position{0, -1}, Position{1, -1}
	};

	for (const Position &direction : directions)
	{
		Position targetPosition{position.row + direction.row, position.col + direction.col};

		if (targetPosition.row >= 0 && targetPosition.row < 8 && targetPosition.col >= 0 && targetPosition.col < 8)
		{
			attacks.setBit(targetPosition);
		}
	}

	return attacks;
}

bool MoveGenerator::canDoublePawnPush(Color color, Position position)
{
	return (color == Color::WHITE && position.row == 6) || (color == Color::BLACK && position.row == 1);
}