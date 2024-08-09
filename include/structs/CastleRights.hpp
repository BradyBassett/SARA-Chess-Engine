#ifndef CASTLERIGHTS_HPP
#define CASTLERIGHTS_HPP

struct CastleRights
{
	bool kingSide;
	bool queenSide;

	CastleRights(bool kingSide = true, bool queenSide = true) : kingSide(kingSide), queenSide(queenSide) {};

	bool canCastleKingSide() const
	{
		return kingSide;
	}

	bool canCastleQueenSide() const
	{
		return queenSide;
	}

	bool canCastle() const
	{
		return kingSide || queenSide;
	}

	void disable()
	{
		kingSide = false;
		queenSide = false;
	}

	void disableKingSide()
	{
		kingSide = false;
	}

	void disableQueenSide()
	{
		queenSide = false;
	}

	bool operator==(const CastleRights &other) const
	{
		return kingSide == other.kingSide && queenSide == other.queenSide;
	}
};

#endif // CASTLERIGHTS_HPP