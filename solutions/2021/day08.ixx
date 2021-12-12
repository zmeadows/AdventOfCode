export module day08;

import types;
import parse;
import utils;

import <algorithm>;
import <compare>;
import <string>;
import <unordered_map>;
import <unordered_set>;
import <vector>;

struct Entry {
	std::vector<std::string> patterns;
	std::vector<std::string> output;
};

static const std::unordered_map<U64, U64> UNIQUE_DIGITS = { {2, 1}, {3, 7}, {4, 4}, {7, 8} };

static U64 decode(const Entry& entry)
{
	std::unordered_set<std::string> patterns_remaining(
		entry.patterns.begin(), entry.patterns.end()
	);

	std::unordered_map<U64, std::string> segment_matches;

	for (const std::string& pattern : entry.patterns) {
		if (auto it = UNIQUE_DIGITS.find(pattern.size()); it != UNIQUE_DIGITS.end()) {
			segment_matches[it->second] = pattern;
			patterns_remaining.erase(pattern);
		}
	}

	auto decode_digit = [&](U64 digit, U64 nsegs, auto&& setop, U64 compdig, U64 compres) {
		for (const std::string& pattern : patterns_remaining) {
			if (pattern.size() == nsegs) {
				const std::string& segments = segment_matches.at(compdig);
				std::string buffer;
				setop(pattern, segments, buffer);
				if (buffer.size() == compres) {
					segment_matches[digit] = pattern;
					// NOTE: calling 'erase' invalidates future iteration, but we're returning immediately.
					patterns_remaining.erase(pattern);
					return;
				}
			}
		}
	};

	decode_digit(9, 6, intersect<std::string>, 4, 4);
	decode_digit(2, 5, combine<std::string>  , 4, 7);
	decode_digit(0, 6, intersect<std::string>, 7, 3);
	decode_digit(6, 6, intersect<std::string>, 7, 2);
	decode_digit(5, 5, intersect<std::string>, 6, 5);
	decode_digit(3, 5, intersect<std::string>, 6, 4);

	std::unordered_map<std::string, U64> decoded_segments;
	for (const auto& [k, v] : segment_matches)
		decoded_segments.emplace(v, k);

	U64 answer = 0;
	for (const std::string& digit_pattern : entry.output)
		answer = 10 * answer + decoded_segments.at(digit_pattern);

	return answer;
}

export struct Day8 {
	static constexpr U64 DAY_NUMBER = 8;
	static constexpr std::pair<U64, U64> SOLUTION = { 476, 1011823 };

	using InputType = std::vector<Entry>;
	static InputType prepare_input()
	{
		auto lines = read_lines("2021/8a.txt");

		std::vector<Entry> entries;
		entries.reserve(lines.size());

		for (const std::string& line : lines) {
			Entry entry;

			bool hit_delimiter = false;
			for (std::string& word : split(line)) {
				if (word == "|") {
					hit_delimiter = true;
					continue;
				}

				std::sort(word.begin(), word.end());
				if (hit_delimiter)
					entry.output.emplace_back(std::move(word));
				else
					entry.patterns.emplace_back(std::move(word));
			}

			entries.emplace_back(std::move(entry));
		}

		return entries;
	}

	static std::pair<U64, U64> solve(const InputType& entries)
	{
		std::pair<U64, U64> answer = { 0,0 };

		for (const Entry& entry : entries) {
			for (const std::string& pattern : entry.output) {
				if (auto it = UNIQUE_DIGITS.find(pattern.size()); it != UNIQUE_DIGITS.end()) {
					answer.first++;
				}
			}
		}

		for (const Entry& entry : entries)
			answer.second += decode(entry);

		return answer;
	}
};

