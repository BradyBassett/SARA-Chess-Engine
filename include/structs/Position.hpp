#ifndef POSITION_HPP
#define POSITION_HPP

struct Position
{
	int row;
	int col;

	bool operator==(const Position &other) const
	{
		return row == other.row && col == other.col;
	}

	bool operator!=(const Position &other) const
	{
		return !(*this == other);
	}
};

#endif // POSITION_HPP