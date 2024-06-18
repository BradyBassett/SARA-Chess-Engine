#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "structs/Position.hpp"

#include <stdexcept>

namespace Utility
{
	void validatePosition(Position position)
	{
		if (position.row < 0 || position.row > 7 || position.col < 0 || position.col > 7)
		{
			throw std::invalid_argument("Invalid position");
		}
	}
}

#endif // UTILITY_HPP