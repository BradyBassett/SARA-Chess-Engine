#ifndef PIECELIST_HPP
#define PIECELIST_HPP

#include <vector>
#include <array>

struct PieceList
{
	std::vector<int> occupiedSquares;
	std::array<int, 64> map;
	int count;

	PieceList(int maxCount = 16)
	{
		occupiedSquares.reserve(maxCount);
		count = 0;
	}

	void addPiece(int square)
	{
		occupiedSquares.push_back(square);
		map[square] = count;
		count++;
	}

	void removePiece(int square)
	{
		int index = map[square];
		occupiedSquares[index] = occupiedSquares[count - 1];
		map[occupiedSquares[index]] = index;
		count--;
	}

	void movePiece(int from, int to)
	{
		int index = map[from];
		occupiedSquares[index] = to;
		map[to] = index;
	}
};

#endif // PIECELIST_HPP