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

#define FOR(X,Y) for (size_t X = 0; X < Y; X++)
#define FORS(X,S,Y) for (size_t X = S; X < Y; X++)
#define FORN(X,Y,N) for (size_t X = 0; X < Y; X += N)

using I = int64_t;
using R = double;

using S = std::string;

template <typename T>
using V = std::vector<T>;

template <typename T>
std::vector<T> readfile(const std::string& input_file) {
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

template <typename FmtType, class... Args>
void out(const FmtType& fmt, Args&& ...args)
{
	std::cout << std::format(fmt, std::forward<Args>(args)...);
}

template <typename T>
T sum(const std::vector<T>& vec, size_t start, size_t end) {
	T res = {};
	for (size_t i = start; i <= end; i++) {
		res += vec[i];
	}
	return res;
}

__forceinline I toint(const std::string& str) {
	return std::stoll(str);
}

