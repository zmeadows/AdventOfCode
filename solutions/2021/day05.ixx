export module day05;

import types;
import parse;
import v2;

import <utility>;
import <string>;
import <vector>;

struct Vent {
	V2<U64> from;
	V2<U64> to;
};

export struct Day5 {
	static constexpr U64 DAY_NUMBER = 5;
	static constexpr std::pair<U64, U64> SOLUTION = { 5197, 18605 };
	using InputType = std::vector<Vent>;

	static InputType prepare_input()
	{
		const auto lines = read_lines_as<std::string>("2021/5a.txt");

		std::vector<Vent> vents;
		vents.reserve(1 + lines.size() / 3);

		for (U64 i = 0; i < lines.size() - 2; i += 3)
			vents.emplace_back(Vent{ lines[i], lines[i + 2] });

		return vents;
	}

	static std::pair<U64, U64> solve(const InputType& vents)
	{
		auto points_one = new U8[1000][1000];
		auto points_two = new U8[1000][1000];
		std::memset(points_one, 0, 1000 * 1000);
		std::memset(points_two, 0, 1000 * 1000);

		for (const Vent& v : vents) {
			if (v.from.x == v.to.x) { // vertical
				const U64 colx = v.from.x;
				const U64 low = std::min(v.from.y, v.to.y);
				const U64 high = std::max(v.from.y, v.to.y);
				for (U64 y = low; y <= high; y++) {
					points_one[colx][y]++;
					points_two[colx][y]++;
				}
			}
			else if (v.from.y == v.to.y) { // horizontal
				const U64 rowy = v.from.y;
				const U64 low = std::min(v.from.x, v.to.x);
				const U64 high = std::max(v.from.x, v.to.x);
				for (U64 x = low; x <= high; x++) {
					points_one[x][rowy]++;
					points_two[x][rowy]++;
				}
			}
			else { // diagonal
				const V2<U64> delta(v.to.x > v.from.x ? 1 : -1, v.to.y > v.from.y ? 1 : -1);
				for (V2<U64> tmp = v.from; tmp != v.to; tmp += delta)
					points_two[tmp.x][tmp.y]++;
				points_two[v.to.x][v.to.y]++;
			}
		}

		std::pair<U64, U64> answer = { 0,0 };
		for (U64 x = 0; x < 1000; x++) {
			for (U64 y = 0; y < 1000; y++) {
				if (points_one[x][y] > 1) answer.first++;
				if (points_two[x][y] > 1) answer.second++;
			}
		}

		delete[] points_one;
		delete[] points_two;

		return answer;
	}
};

