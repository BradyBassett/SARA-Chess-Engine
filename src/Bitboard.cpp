#include "../include/Bitboard.hpp"
#include "../include/Utility.hpp"

Bitboard::Bitboard()
{
	value = 0x0ULL;
}

Bitboard::Bitboard(uint64_t value)
{
	this->value = value;
}

Bitboard::Bitboard(Position position)
{
	Utility::validatePosition(position);

	value = 0x1ULL << Utility::calculateSquareNumber(position);
}

uint64_t Bitboard::getValue() const
{
	return value;
}

void Bitboard::setValue(uint64_t value)
{
	this->value = value;
}

void Bitboard::setBit(Position position)
{
	Utility::validatePosition(position);

	uint64_t bit = 1ULL << Utility::calculateSquareNumber(position);
	value |= bit;
}

void Bitboard::clearBit(Position position)
{
	Utility::validatePosition(position);

	value &= ~(1ULL << Utility::calculateSquareNumber(position));
}

bool Bitboard::getBit(Position position) const
{
	Utility::validatePosition(position);

	return value & (1ULL << Utility::calculateSquareNumber(position));
}

Bitboard Bitboard::operator|(const Bitboard &other) const
{
	Bitboard bitboard = Bitboard(value | other.value);
	return bitboard;
}

Bitboard &Bitboard::operator|=(const Bitboard &other)
{
	value |= other.value;
	return *this;
}

Bitboard Bitboard::operator&(const Bitboard &other) const
{
	Bitboard bitboard = Bitboard(value & other.value);
	return bitboard;
}

Bitboard &Bitboard::operator&=(const Bitboard &other)
{
	value &= other.value;
	return *this;
}

Bitboard Bitboard::operator^(const Bitboard &other) const
{
	Bitboard bitboard = Bitboard(value ^ other.value);
	return bitboard;
}

Bitboard &Bitboard::operator^=(const Bitboard &other)
{
	value ^= other.value;
	return *this;
}

Bitboard Bitboard::operator~() const
{
	Bitboard bitboard = Bitboard(~value);
	return bitboard;
}

Bitboard Bitboard::operator<<(uint64_t shift) const
{
	if (shift >= 64) {
		return Bitboard(0);
	}

	Bitboard bitboard = Bitboard(value << shift);
	return bitboard;
}

Bitboard &Bitboard::operator<<=(uint64_t shift)
{
	if (shift >= 64) {
		value = 0;
		return *this;
	}

	value <<= shift;
	return *this;
}

Bitboard Bitboard::operator>>(uint64_t shift) const
{
	if (shift >= 64) {
		return Bitboard(0);
	}

	Bitboard bitboard = Bitboard(value >> shift);
	return bitboard;
}

Bitboard &Bitboard::operator>>=(uint64_t shift)
{
	if (shift >= 64) {
		value = 0;
		return *this;
	}

	value >>= shift;
	return *this;
}

bool Bitboard::operator==(const Bitboard &other) const
{
	bool isEqual = value == other.value;
	return isEqual;
}

bool Bitboard::operator!=(const Bitboard &other) const
{
	bool notEqual = value != other.value;
	return notEqual;
}