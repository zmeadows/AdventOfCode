module;
#include <cassert>
export module day13;

import parse;
import types;
import utils;

import <algorithm>;
import <string>;
import <tuple>;
import <unordered_set>;
import <utility>;
import <vector>;
import <iostream>;

constexpr U16 VERTICAL = 1;
constexpr U16 HORIZONTAL = 2;

constexpr bool PRINT_PART_TWO = false;

export struct Day13 {
	static constexpr U64 DAY_NUMBER = 13;
	static constexpr std::pair<U64, U64> SOLUTION = { 850, 0 };

	struct Fold {
		U16 pos;
		U16 dir;
	};

	using InputType = std::tuple<
		std::vector<std::pair<U16,U16>>, // initial dot locations
		std::vector<Fold> // ordered fold instructions
	>;

	static InputType prepare_input() {
		const std::vector<std::string> input = read_lines("2021/13a.txt");

		std::vector<std::pair<U16, U16>> dots; dots.reserve(input.size());
		std::vector<Fold> folds; folds.reserve(16);

		U64 idx = 0;
		while (true) {
			const std::string& line = input[idx];
			if (line.empty()) break;
			const auto s = split<U16>(line, ',');
			dots.emplace_back(s[0], s[1]);
			const auto& [x, y] = dots.back();
			idx++;
		}

		idx++;

		while (idx < input.size()) {
			const std::string& line = input[idx];
			const auto words = split(line);
			folds.emplace_back(Fold{
				static_cast<U16>(std::stoi(std::string(words[2].begin() + 2, words[2].end()))),
                words[2][0] == 'x' ? VERTICAL : HORIZONTAL
			});
			idx++;
		}

		return { std::move(dots), std::move(folds) };
	}

	static std::pair<U64, U64> solve(const InputType& input)
	{
		std::pair<U64, U64> answer = { 0ULL, 0ULL };

		const auto& [INIT_DOTS, FOLDS] = input;

		std::vector<std::pair<U16, U16>> dots(INIT_DOTS);

		U64 fold_idx = 0;
		auto perform_next_fold = [&]() -> void {
			const Fold& f = FOLDS[fold_idx];
			if (f.dir == VERTICAL) {
				for (auto& [x, _] : dots) {
					if (x > f.pos) x = 2 * f.pos - x;
				}
			} else { // f.dir == HORIZONTAL
				for (auto& [_, y] : dots) {
					if (y > f.pos) y = 2 * f.pos - y;
				}
			}
			fold_idx++;
		};

		{ // part one
			perform_next_fold();
			std::sort(dots.begin(), dots.end());
			auto it = std::unique(dots.begin(), dots.end());
			dots.resize(std::distance(dots.begin(), it));
			answer.first = dots.size();
		}

		while (fold_idx < FOLDS.size()) perform_next_fold();

		if constexpr (PRINT_PART_TWO) {
			U16 paper_height = 0;
			U16 paper_width = 0;
			for (const auto& [x, y] : dots) {
				paper_width = std::max(paper_width, x);
				paper_height = std::max(paper_height, y);
			}

			for (U16 y = 0; y <= paper_height; y++) {
				for (U16 x = 0; x <= paper_width; x++) {
					auto it = std::find(dots.begin(), dots.end(), std::make_pair(x,y));
					std::cout << (it != dots.end() ? "#" : " ");
				}
				std::cout << "\n";
			}
		}

		return answer;
	}
};

