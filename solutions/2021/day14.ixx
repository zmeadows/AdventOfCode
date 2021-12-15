module;
#include "flat_hash_map.hpp"
export module day14;

import parse;
import types;
import utils;

import <algorithm>;
import <limits>;
import <string>;
import <tuple>;
import <unordered_map>;
import <unordered_set>;
import <vector>;

// TODO: Could be optimized by converting base pairs to contiguous integers
// and using small stack-allocated 2d arrays intead of unordered_map.

export struct Day14 {
	static constexpr U64 DAY_NUMBER = 14;
	static constexpr std::pair<U64, U64> SOLUTION = { 3048, 3288891573057 };

	using InputType = std::tuple<
		std::string, // polymer template
		ska::flat_hash_map<std::pair<char, char>, char, hash_pair>, // pair insertion rules
		U64 // number of unique letters
	>;

	static InputType prepare_input() 
	{
		const std::vector<std::string> input = read_lines("2021/14a.txt");

		std::unordered_set<char> elements;
		elements.reserve(input[0].size());

		ska::flat_hash_map<std::pair<char, char>, char, hash_pair> rules;
		rules.reserve(input.size() - 2);

		for (U64 idx = 2; idx < input.size(); idx++) {
			const std::string& s = input[idx];
			rules[{s[0], s[1]}] = s[6];
			for (char ch : {s[0], s[1], s[6]}) elements.insert(ch);
		}

		return { input[0], std::move(rules), elements.size() };
	}

	static std::pair<U64, U64> solve(const InputType& input)
	{
		const auto& [start, insertion_rules, nletters] = input;

		ska::flat_hash_map<std::pair<char, char>, std::pair<S64, S64>, hash_pair> counts;
		counts.reserve(insertion_rules.size());

		for (U64 i = 0; i < start.size() - 1; i++)
			counts[{start[i], start[i + 1]}].first++;

		auto iterate = [&]() -> void {
			for (const auto& [base_pair, new_middle] : insertion_rules) {
				auto& [current, next_iter] = counts[base_pair];
				next_iter -= current;
				counts[{base_pair.first, new_middle}].second += current;
				counts[{new_middle, base_pair.second}].second += current;
			}

			for (auto& [_, v] : counts) v = { v.first + v.second, 0 };
		};

		ska::flat_hash_map<char, U64> char_counts; char_counts.reserve(nletters);

		auto compute_answer = [&]() -> U64 {
			char_counts = { {start.front(), 1}, {start.back(), 1} };

			for (const auto& [k, v] : counts) {
				char_counts[k.first] += v.first;
				char_counts[k.second] += v.first;
			}

			std::pair<U64, U64> bounds = { std::numeric_limits<U64>::max(), 0 };
			for (const auto& [_, c] : char_counts)
				bounds = { std::min(bounds.first, c/2), std::max(bounds.second, c/2) };
			return bounds.second - bounds.first;
		};

		std::pair<U64, U64> answer = { 0ULL, 0ULL };

		for (U64 nsteps = 1; nsteps <= 10; nsteps++) iterate();
		answer.first = compute_answer();
		for (U64 nsteps = 11; nsteps <= 40; nsteps++) iterate();
		answer.second = compute_answer();

		return answer;
	}
};

