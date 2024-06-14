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
};

#endif // CASTLERIGHTS_HPP