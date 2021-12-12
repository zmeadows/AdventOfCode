export module day10;

import types;
import parse;

import <algorithm>;
import <compare>;
import <string>;
import <utility>;
import <unordered_map>;
import <vector>;

export struct Day10 {
	static constexpr U64 DAY_NUMBER = 10;
	static constexpr std::pair<U64, U64> SOLUTION = { 288291, 820045242 };
	using InputType = std::vector<std::string>;

	static InputType prepare_input() {
		return read_lines("2021/10a.txt");
	}

	static std::pair<U64, U64> solve(const InputType& input_lines)
	{
		std::pair<U64, U64> answer = { 0,0 };

		char closers[128];
		closers['('] = ')';
		closers['['] = ']';
		closers['{'] = '}';
		closers['<'] = '>';

		const std::unordered_map<char, U64> CORRUPTED_CHAR_SCORE = {
			{')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}
		};

		const std::unordered_map<char, U64> INCOMPLETE_CHAR_SCORE = {
			{')', 1}, {']', 2}, {'}', 3}, {'>', 4}
		};

		auto is_opener = [](const char ch) -> bool {
			return ch == '(' || ch == '[' || ch == '{' || ch == '<';
		};

		auto parens_match = [](char opener, char closer) -> bool {
			return (opener == '(' && closer == ')')
				|| (opener == '[' && closer == ']')
				|| (opener == '{' && closer == '}')
				|| (opener == '<' && closer == '>');
		};

		std::vector<U64> scores; scores.reserve(256);
		std::string paren_history; paren_history.reserve(256);

		for (const std::string& line : input_lines) {
			bool corrupted = false;

			for (const auto& next_paren : line) {
				if (is_opener(next_paren)) {
					paren_history.push_back(closers[next_paren]);
				} else if (next_paren == paren_history.back()) {
					paren_history.pop_back();
				} else {
					answer.first += CORRUPTED_CHAR_SCORE.at(next_paren);
					corrupted = true;
					break;
				}
			}

			if (!corrupted) {
				U64 s = 0;
				while (!paren_history.empty()) {
					s = 5 * s + INCOMPLETE_CHAR_SCORE.at(paren_history.back());
					paren_history.pop_back();
				}
				scores.push_back(s);
			}

			paren_history.clear();
		}

		std::sort(scores.begin(), scores.end());
		answer.second = scores[scores.size() / 2];

		return answer;
	}
};

