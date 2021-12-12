module;
#include <algorithm>
#include <cassert>
#include <format>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>
#include <utility>
export module utils;

import types;

export inline S64 average(const std::vector<S64>& vec)
{
	assert(!vec.empty());
	const double sum = static_cast<double>(std::reduce(vec.begin(), vec.end()));
	const double entries = static_cast<double>(vec.size());
	return static_cast<S64>(round(sum / entries));
}

export template <typename Container>
inline void intersect(const Container& in1, const Container& in2, Container& out)
{
	out.clear();
	std::set_intersection(
		in1.begin(), in1.end(),
		in2.begin(), in2.end(),
		std::back_inserter(out));
}

export template <typename Container>
inline void combine(const Container& in1, const Container& in2, Container& out)
{
	out.clear();
	std::set_union(
		in1.begin(), in1.end(),
		in2.begin(), in2.end(),
		std::back_inserter(out));
}

export template <typename T>
inline T clamp(T val, T low, T high) {
	if (val < low) return low;
	else if (val > high) return high;
	else return val;
}

export template <typename FmtType, class... Args>
void __forceinline print(const FmtType& fmt, Args&& ...args)
{
	std::cout << std::format(fmt, std::forward<Args>(args)...);
}

export template <typename Problem>
void test() {
	const std::pair<U64, U64> answer = Problem::solve(Problem::prepare_input());
	const bool success = (answer.first == Problem::SOLUTION.first) && (answer.second == Problem::SOLUTION.second);
	const std::string result = success ? "SUCCESS" : "FAILURE";
	print("Day {:<2} => [ {:^15} , {:^17} ] {:>8}!\n",
		  Problem::DAY_NUMBER, answer.first, answer.second, result);
}
