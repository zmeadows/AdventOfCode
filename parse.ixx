module;
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
export module parse;

import types;

export std::ifstream open_input_file(const std::string& input_file)
{
	const std::string full_input_path = std::string(_SOLUTIONDIR) + "inputs\\" + input_file;
	return std::ifstream(full_input_path);
}

// TODO: use std::string delims convert to unordered_set, and skip *all*
export template <typename T = std::string>
std::vector<T> split(const std::string& input, char delim = ' ')
{
	std::stringstream ss(input);

	std::vector<T> result;

	T val = {};
	while (ss >> val) {
		result.push_back(val);
		while (ss.peek() == delim) {
			ss.ignore();
		}
	}

	return result;
}

export template <typename T>
std::vector<T> read_lines_as(const std::string& input_file)
{
	std::ifstream infile = open_input_file(input_file);

	std::vector<T> results;

	std::string line;
	T val = {};
	while (std::getline(infile, line)) {
		std::stringstream ss(line);
		while (ss >> val) {
			results.push_back(val);
		}
	}

	return results;
}

export std::vector<std::string> read_lines(const std::string& input_file)
{
	std::ifstream infile = open_input_file(input_file);

	std::vector<std::string> results;

	std::string line;
	while (std::getline(infile, line)) {
		results.push_back(line);
	}

	return results;
}

export std::pair<std::vector<U64>, U64> read_binary_numbers_from_file(const std::string& input_file)
{
	std::ifstream infile = open_input_file(input_file);

	std::vector<U64> results;

	std::string line; U64 max_bits = 0;
	while (std::getline(infile, line)) {
		max_bits = std::max(max_bits, line.size());
		results.push_back(std::stoll(line, nullptr, 2));
	}

	return { std::move(results), max_bits };
}

export template <U64 N, U64 M>
bool parse_grid(std::ifstream& input, Grid<S64, M, N>& grid, char delim = ' ')
{
	std::string row_string;

	// skip all initial empty lines
	while (std::getline(input, row_string)) {
		if (!row_string.empty()) {
			break;
		}
	}

	size_t row_idx = 0;

	do {
		if (row_string.empty()) {
			break;
		}

		std::stringstream row_stream(row_string);
		while (row_stream.peek() == delim) {
			row_stream.ignore();
		}

		size_t col_idx = 0;

		std::string entry;
		while (std::getline(row_stream, entry, delim)) {
			grid[row_idx][col_idx] = std::stoll(entry);
			col_idx++;
			while (row_stream.peek() == delim) {
				row_stream.ignore();
			}
		}
		if (col_idx != N) {
			return false;
		}

		row_idx++;

	} while (std::getline(input, row_string));

	if (row_idx != M) {
		return false;
	}
	
	return true;
}

