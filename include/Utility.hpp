#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "structs/Position.hpp"

#include <string>

namespace Utility {
    int calculateSquareNumber(Position position);
    Position calculatePosition(int squareNumber);
    void validatePosition(Position position);
    void validateSquareNumber(int squareNumber);
    Position convertStringToPosition(std::string str);
}

#endif // UTILITY_HPP