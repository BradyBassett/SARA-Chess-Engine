#include "../include/Utility.hpp"
#include <stdexcept>

namespace Utility {
    int calculateSquareNumber(Position position) {
        validatePosition(position);
        return position.row * 8 + position.col;
    }

    Position calculatePosition(int squareNumber) {
        validateSquareNumber(squareNumber);

        return Position{squareNumber / 8, squareNumber % 8};
    }

    void validatePosition(Position position) {
        if (position.row < 0 || position.row > 7 || position.col < 0 || position.col > 7) {
            throw std::invalid_argument("Position out of bounds");
        }
    }

    void validateSquareNumber(int squareNumber) {
        if (squareNumber < 0 || squareNumber > 63) {
            throw std::invalid_argument("Square number out of bounds");
        }
    }

    Position convertStringToPosition(std::string str) {
        if (str.length() != 2) {
            throw std::invalid_argument("Invalid string length");
        }

        Position pos{8 - (str[1] - '0'), str[0] - 'a'};

        validatePosition(pos);

        return pos;
    }
}