#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <format>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "bignum/wideint.h"
using U256 = wideint<256>;

using U64 = uint64_t;
using U32 = uint32_t;
using U16 = uint16_t;
using U8 = uint8_t;
static_assert(sizeof(U64) == 8);
static_assert(sizeof(U32) == 4);
static_assert(sizeof(U16) == 2);
static_assert(sizeof(U8) == 1);

using S64 = int64_t;
using S32 = int32_t;
using S16 = int16_t;
using S8 = int8_t;
static_assert(sizeof(S64) == 8);
static_assert(sizeof(S32) == 4);
static_assert(sizeof(S16) == 2);
static_assert(sizeof(S8) == 1);

using F32 = float;
using F64 = double;
static_assert(sizeof(F64) == 8);
static_assert(sizeof(F32) == 4);

template <typename T, size_t M, size_t N>
using Grid = std::array<std::array<T, N>, M>;


__forceinline std::ifstream open_input_file(const std::string& input_file)
{
	const std::string full_input_path = std::string(_SOLUTIONDIR) + "inputs\\" + input_file;
	return std::ifstream(full_input_path);
}

template <typename T>
std::vector<T> split(const std::string& input, char delim)
{
	std::stringstream ss(input);

	std::vector<T> result;

	T val = {};
	while (ss >> val) {
		result.push_back(val);
		if (ss.peek() == delim) {
			ss.ignore();
		}
	}

	return result;
}

template <size_t N, size_t M>
__forceinline bool parse_grid(std::ifstream& input, Grid<S64, M, N>& grid, char delim = ' ')
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

template <typename T>
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

__forceinline std::pair<std::vector<U64>, U64> read_binary_numbers_from_file(const std::string& input_file)
{
	const std::string input_path = std::string(_SOLUTIONDIR) + "inputs\\" + input_file;
	std::ifstream infile(input_path);

	std::vector<U64> results;
	std::string line;

	U64 max_bits = 0;
	while (std::getline(infile, line)) {
		max_bits = std::max(max_bits, line.size());
		results.push_back(std::stoll(line, nullptr, 2));
	}
	return { results, max_bits };
}

template <typename FmtType, class... Args>
__forceinline void print(const FmtType& fmt, Args&& ...args)
{
	std::cout << std::format(fmt, std::forward<Args>(args)...);
}

template <typename T, typename Callable>
__forceinline void filter(std::vector<T>& vec, Callable&& f)
{
	vec.erase(std::remove_if(vec.begin(), vec.end(), f), vec.end());
}

__forceinline constexpr U64 set_bit(U64 num, U64 idx)
{
	return num | (U64(1) << idx);
}
__forceinline constexpr U64 clear_bit(U64 num, U64 idx)
{
	return num & ~(U64(1) << idx);
}
__forceinline constexpr U64 toggle_bit(U64 num, U64 idx)
{
	return num ^ (U64(1) << idx);
}
__forceinline constexpr bool check_bit(U64 num, U64 idx)
{
	return ((num >> idx) & U64(1)) > 0;
}

template <typename T>
struct V2 {
	T x = {};
	T y = {};

	V2(const std::string& coords) {
		const auto s = split<S64>(coords, ',');
		assert(s.size() == 2);
		this->x = s[0];
		this->y = s[1];
	}

	V2(T _x, T _y) : x(_x), y(_y) {}

	bool operator==(const V2<T>&) const = default;

	__forceinline V2<T>& operator+=(const V2<T>& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
};

// The specialized hash function for `unordered_map` keys
struct v2_hash_fn
{
	template <class T>
	std::size_t operator() (const V2<T>& vec) const
	{
		const std::size_t h1 = std::hash<T>()(vec.x);
		const std::size_t h2 = std::hash<T>()(vec.y);

		return h1 ^ h2;
	}
};




