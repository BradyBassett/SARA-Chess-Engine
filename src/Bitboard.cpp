#include "../include/Bitboard.hpp"
#include <stdexcept>

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
	value = 0x1ULL << (position.row * 8 + position.col);
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
	validatePosition(position);

	uint64_t bit = 1ULL << calculateSquareNumber(position);
	value |= bit;
}

void Bitboard::clearBit(Position position)
{
	validatePosition(position);

	value &= ~(1ULL << calculateSquareNumber(position));
}

bool Bitboard::getBit(Position position) const
{
	validatePosition(position);

	return value & (1ULL << calculateSquareNumber(position));
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

int Bitboard::calculateSquareNumber(Position position) const
{
	return position.row * 8 + position.col;
}

void Bitboard::validatePosition(Position position) const
{
	if (position.row < 0 || position.row > 7 || position.col < 0 || position.col > 7)
	{
		throw std::invalid_argument("Position out of bounds");
	}
}