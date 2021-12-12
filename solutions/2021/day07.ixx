export module day07;

import types;
import parse;
import utils;

import <algorithm>;
import <utility>;
import <vector>;
import <string>;

export struct Day7 {
	static constexpr U64 DAY_NUMBER = 7;
	static constexpr std::pair<U64, U64> SOLUTION = { 355521, 100148777 };
	using InputType = std::vector<S64>;

	static InputType prepare_input()
	{
		const std::string input = read_lines_as<std::string>("2021/7a.txt")[0];
		return split<S64>(input, ',');
	}

	static std::pair<U64, U64> solve(const InputType& positions)
	{
		const S64 max_pos = *std::max_element(positions.begin(), positions.end());
		const S64 min_pos = *std::min_element(positions.begin(), positions.end());
		const S64 mean_pos = average(positions);

		auto optimize = [&](auto&& fuel_cost_fn) -> S64 {
			// Both fuel functions are monotonically increasing or decreasing on either side
			// of the global minimum. So we just pick the right direction and move.
			const S64 delta = fuel_cost_fn(mean_pos + 1) < fuel_cost_fn(mean_pos) ? 1 : -1;
			const S64 bound = (delta > 0 ? max_pos : min_pos) + delta;

			S64 last_fuel_cost = std::numeric_limits<S64>::max();
			for (S64 align = mean_pos; align != bound; align += delta) {
				const S64 align_cost = fuel_cost_fn(align);
				if (align_cost > last_fuel_cost) break;
				last_fuel_cost = align_cost;
			}
			return last_fuel_cost;
		};

		std::pair<U64, U64> answer = {};

		answer.first = optimize([&](S64 align) {
			S64 cost = 0;
			for (const S64 p : positions) {
				cost += std::abs(p - align);
			}
			return cost;
		});

		answer.second = optimize([&](S64 align) {
			S64 cost = 0;
			for (const S64 p : positions) {
				const S64 delta = std::abs(p - align);
				cost += delta * (delta + 1) / 2;
			}
			return cost;
		});

		return answer;
	}
};

