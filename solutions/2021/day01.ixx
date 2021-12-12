export module day01;

import types;
import parse;

import <utility>;
import <vector>;

export struct Day1 {
	static constexpr U64 DAY_NUMBER = 1;
	static constexpr std::pair<U64, U64> SOLUTION = { 1527, 1575 };
	using InputType = std::vector<S64>;

	static InputType prepare_input() {
		return read_lines_as<S64>("2021/1a.txt");
	}

	static std::pair<U64, U64> solve(const InputType& depths)
	{
		std::pair<U64, U64> answer = {0, 0};

		for (size_t idx = 1; idx < depths.size(); idx++)
			answer.first += depths[idx] > depths[idx - 1];

		S64 last = depths[0] + depths[1] + depths[2];
		for (size_t idx = 3; idx < depths.size(); idx++) {
			const S64 next = depths[idx - 2] + depths[idx - 1] + depths[idx];
			answer.second += next > last;
			last = next;
		}

		return answer;
	}
};

