#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "structs/Position.hpp"

#include <nlohmann/json.hpp>
#include <fstream>
#include <array>
#include <iostream>
#include <string>

namespace Utility {
    int calculateSquareNumber(Position position);
    Position calculatePosition(int squareNumber);
    void validatePosition(Position position);
    void validateSquareNumber(int squareNumber);
    Position convertStringToPosition(std::string str);
    std::string convertPositionToString(Position position);

    using json = nlohmann::json;
    template<typename T, size_t Size>
    static void loadArrayFromJson(std::string filepath, std::array<T, Size>& arr) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filepath << std::endl;
            return;
        }

        json j;
        file >> j;

        for (size_t i = 0; i < Size; ++i) {
            T it = j.at(i).get<T>();
            arr[i] = j.at(i).get<T>();
        }
    }
}

#endif // UTILITY_HPP