#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <cstdint>

#include "../include/structs/Position.hpp"
class Bitboard
{
public:
	Bitboard();
	Bitboard(uint64_t value);
	Bitboard(Position position);

	uint64_t getValue() const;
	void setValue(uint64_t value);

	void setBit(Position position);
	void clearBit(Position position);
	bool getBit(Position position) const;

	Bitboard operator|(const Bitboard &other) const;
	Bitboard &operator|=(const Bitboard &other);
	Bitboard operator&(const Bitboard &other) const;
	Bitboard &operator&=(const Bitboard &other);
	Bitboard operator^(const Bitboard &other) const;
	Bitboard &operator^=(const Bitboard &other);
	Bitboard operator~() const;
	Bitboard operator<<(uint64_t shift) const;
	Bitboard &operator<<=(uint64_t shift);
	Bitboard operator>>(uint64_t shift) const;
	Bitboard &operator>>=(uint64_t shift);
	bool operator==(const Bitboard &other) const;
	bool operator!=(const Bitboard &other) const;
	bool operator!=(uint64_t other) const;

private:
	uint64_t value = 0x0;

	int calculateSquareNumber(Position position) const;
};

#endif // BITBOARD_HPP