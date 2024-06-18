#include "Utility.hpp"
#include <stdexcept>

namespace Utility {
    void validatePosition(Position position) {
        if (position.row < 0 || position.row > 7 || position.col < 0 || position.col > 7) {
            throw std::invalid_argument("Position out of bounds");
        }
    }
}