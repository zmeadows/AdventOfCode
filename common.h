#pragma once

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <format>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

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

template <typename T>
std::vector<T> read_lines_as(const std::string& input_file)
{
	const std::string input_path = std::string(_SOLUTIONDIR) + "inputs\\" + input_file;
	std::ifstream infile(input_path);

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
