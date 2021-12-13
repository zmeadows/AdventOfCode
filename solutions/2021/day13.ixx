module;
#include <cassert>
#include "radix_sort.hpp"
export module day13;

import parse;
import types;
import utils;

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
		std::vector<Fold>, // ordered fold instructions
		U16, // max height
		U16 // max width
	>;

	static InputType prepare_input() {
		const std::vector<std::string> input = read_lines("2021/13a.txt");

		U16 xmax = 0;
		U16 ymax = 0;
		std::vector<std::pair<U16, U16>> dots; dots.reserve(input.size());
		std::vector<Fold> folds; folds.reserve(16);

		U64 idx = 0;
		while (true) {
			const std::string& line = input[idx];
			if (line.empty()) break;
			const auto s = split<U16>(line, ',');
			dots.emplace_back(s[0], s[1]);
			const auto& [x, y] = dots.back();
			xmax = std::max(xmax, x);
			ymax = std::max(xmax, y);
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

		return {
			std::move(dots),
			std::move(folds),
			xmax + 1,
			ymax + 1
		};
	}

	static std::pair<U64, U64> solve(const InputType& input)
	{
		std::pair<U64, U64> answer = { 0ULL, 0ULL };

		const auto& [INIT_DOTS, FOLDS, XMAX, YMAX] = input;

		U32* dots_old = static_cast<U32*>(alloca(sizeof(U32) * INIT_DOTS.size()));
		U32* dots_new = static_cast<U32*>(alloca(sizeof(U32) * INIT_DOTS.size()));

		{
			U64 idx = 0;
			for (const auto& [x, y] : INIT_DOTS) {
				dots_old[idx] = y * XMAX + x;
				idx++;
			}
		}

		U16 paper_width = XMAX;
		U16 paper_height = YMAX;

		U64 old_dot_count = INIT_DOTS.size();
		U64 new_dot_count = 0;

		for (U16 fold_idx = 0; fold_idx < FOLDS.size(); fold_idx++) {
			const Fold& f = FOLDS[fold_idx];

			switch (f.dir) {
			    case HORIZONTAL: {
			    	paper_height = f.pos;

			    	for (U64 old_dot_idx = 0; old_dot_idx < old_dot_count; old_dot_idx++) {
			    		const U32 p = dots_old[old_dot_idx];
			    		const U16 y = static_cast<U16>(p / XMAX);
			    		if (y < paper_height) {
			    			dots_new[new_dot_count] = p;
			    			new_dot_count++;
			    		} else {
			    			const U16 x = static_cast<U16>(p % XMAX);
			    			const U16 ynew = 2 * paper_height - y;
			    			dots_new[new_dot_count] = ynew * XMAX + x;
			    			new_dot_count++;
			    		}
			    	}

			    	break;
			    }

			    case VERTICAL: {
			    	paper_width = f.pos;

			    	for (U64 old_dot_idx = 0; old_dot_idx < old_dot_count; old_dot_idx++) {
			    		const U32 p = dots_old[old_dot_idx];
			    		const U16 x = static_cast<U16>(p % XMAX);
			    		if (x < paper_width) {
			    			dots_new[new_dot_count] = p;
			    			new_dot_count++;
			    		} else {
			    			const U16 y = static_cast<U16>(p / XMAX);
			    			const U16 xnew = 2 * paper_width - x;
			    			dots_new[new_dot_count] = y * XMAX + xnew;
			    			new_dot_count++;
			    		}
			    	}

			    	break;
			    }

			}

			if (fold_idx == 0) [[unlikely]] {
				std::vector<U32> v(new_dot_count, 0);
				radix_sort(dots_new, dots_new + new_dot_count, v.begin());
				for (U64 i = 0; i < new_dot_count; i++) {
					while (i < new_dot_count - 1 && v[i] == v[i + 1]) {
						i++;
					}
					answer.first++;
				}
			}

			std::memcpy(dots_old, dots_new, sizeof(U32) * new_dot_count);
			old_dot_count = new_dot_count;
			new_dot_count = 0;
		}

		if constexpr (PRINT_PART_TWO) {
			std::unordered_set<U32> dots(dots_old, dots_old + old_dot_count);
			for (U64 y = 0; y < paper_height; y++) {
				for (U64 x = 0; x < paper_width; x++) {
					if (auto it = dots.find(y * XMAX + x); it != dots.end()) {
						print("#");
					} else {
						print(" ");
					}
				}
				print("\n");
			}
		}

		return answer;
	}

};

