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