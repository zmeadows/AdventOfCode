#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using U64 = uint64_t;
using U32 = uint32_t;
using U16 = uint16_t;
using U8 = uint8_t;

using S64 = int64_t;
using S32 = int32_t;
using S16 = int16_t;
using S8 = int8_t;

using F32 = float;
using F64 = double;

template <typename T>
std::vector<T> read_file_lines_as(const std::string& input_file) {
	const std::string input_path = std::string(_SOLUTIONDIR) + "inputs\\" + input_file;
	std::ifstream infile(input_path);

	std::vector<T> results; results.reserve(2048);

	std::string line; T val;
	while (std::getline(infile, line)) {
		std::stringstream ss(line);
		while (ss >> val) {
			results.push_back(val);
		}
	}

	return results;
}
