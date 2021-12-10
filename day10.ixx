module;
#include "common.h"
export module day10;

export struct Day10 {
	static constexpr U64 DAY_NUMBER = 10;
	static constexpr std::pair<U64, U64> SOLUTION = { 0, 0 };

	using InputType = std::vector<S64>;
	static InputType prepare_input()
	{
		return read_lines_as<S64>("10a.txt");
	}

	static std::pair<U64, U64> solve(const InputType& input)
	{
		std::pair<U64, U64> answer = { 0,0 };
		return answer;
	}
};

