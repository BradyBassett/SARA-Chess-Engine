#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "structs/Position.hpp"

#include <array>
#include <string>

namespace Utility
{
    int calculateSquareNumber(Position position);
    Position calculatePosition(int squareNumber);
    bool isValidPosition(Position position);
    void validatePosition(Position position);
    void validateSquareNumber(int squareNumber);
    int convertStringToSquareNumber(std::string str);
    Position convertStringToPosition(std::string str);
    std::string convertPositionToString(Position position);
}

#endif // UTILITY_HPP