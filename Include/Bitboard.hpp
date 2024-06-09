#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <cstdint>

#include "../Include/Structs/Position.hpp"
class Bitboard
{
public:
	Bitboard();
	Bitboard(uint64_t value);
	Bitboard(Position position);

	uint64_t getValue() const;
	void setValue(uint64_t value);

	void setBit(int index);
	void clearBit(int index);
	bool getBit(int index) const;

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

private:
	uint64_t value = 0x0;
};

#endif // BITBOARD_HPP