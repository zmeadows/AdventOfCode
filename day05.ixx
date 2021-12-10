module;
#include "common.h"
export module day05;

struct Vent {
	V2<S64> from;
	V2<S64> to;
};

export struct Day5 {
	static constexpr U64 DAY_NUMBER = 5;
	static constexpr std::pair<U64, U64> SOLUTION = { 5197, 18605 };
	using InputType = std::vector<Vent>;

	static InputType prepare_input()
	{
		const auto lines = read_lines_as<std::string>("5a.txt");

		std::vector<Vent> vents;
		vents.reserve(1 + lines.size() / 3);

		for (U64 i = 0; i < lines.size() - 2; i += 3)
			vents.emplace_back(Vent{ lines[i], lines[i + 2] });

		return vents;
	}

	static std::pair<U64, U64> solve(const InputType& vents)
	{
		std::unordered_map< V2<S64>, U64, v2_hash_fn > counts_part_one, counts_part_two;

		for (const Vent& v : vents) {
			if (v.from.x == v.to.x) { // vertical
				for (S64 y = std::min(v.from.y, v.to.y); y <= std::max(v.from.y, v.to.y); y++) {
					counts_part_one[V2<S64>(v.from.x, y)]++;
					counts_part_two[V2<S64>(v.from.x, y)]++;
				}
			}
			else if (v.from.y == v.to.y) { // horizontal
				for (S64 x = std::min(v.from.x, v.to.x); x <= std::max(v.from.x, v.to.x); x++) {
					counts_part_one[V2<S64>(x, v.from.y)]++;
					counts_part_two[V2<S64>(x, v.from.y)]++;
				}
			}
			else { // diagonal
				V2<S64> delta(v.to.x > v.from.x ? 1 : -1, v.to.y > v.from.y ? 1 : -1);
				for (V2<S64> tmp = v.from; tmp != v.to; tmp += delta)
					counts_part_two[tmp]++;
				counts_part_two[v.to]++;
			}
		}

		std::pair<U64, U64> answer = { 0,0 };

		for (const auto& [_, c] : counts_part_one)
			if (c > 1) answer.first++;
		for (const auto& [_, c] : counts_part_two)
			if (c > 1) answer.second++;

		return answer;
	}
};

