#ifndef JSONLOADER_HPP
#define JSONLOADER_HPP

#include <nlohmann/json.hpp>
#include <fstream>
#include <array>
#include <iostream>
#include <string>

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
		arr[i] = j.at(i).get<T>();
	}
}

#endif // JSONLOADER_HPP