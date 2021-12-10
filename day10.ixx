module;
#include "common.h"
export module day10;

export struct Day10 {
	static constexpr U64 DAY_NUMBER = 10;
	static constexpr std::pair<U64, U64> SOLUTION = { 288291, 820045242 };
	using InputType = std::vector<std::string>;

	static InputType prepare_input() {
		return read_lines("10a.txt");
	}

	static std::pair<U64, U64> solve(const InputType& input_lines)
	{
		std::pair<U64, U64> answer = { 0,0 };

		const std::unordered_map<char, U64> CORRUPTED_CHAR_SCORE = {
			{')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}
		};

		const std::unordered_map<char, U64> INCOMPLETE_CHAR_SCORE = {
			{'(', 1}, {'[', 2}, {'{', 3}, {'<', 4}
		};

		auto is_opener = [](const char ch) -> bool {
			constexpr std::string_view OPENERS = "{[(<";
			return OPENERS.find(ch) != OPENERS.npos;
		};

		auto parens_match = [](char opener, char closer) -> bool {
			return (opener == '(' && closer == ')')
				|| (opener == '[' && closer == ']')
				|| (opener == '{' && closer == '}')
				|| (opener == '<' && closer == '>');
		};

		std::vector<U64> scores = {};
		std::stack<char> paren_history = {};

		for (const std::string& line : input_lines) {
			bool corrupted = false;

			for (char next_paren : line) {
				if (is_opener(next_paren)) {
					paren_history.push(next_paren);
				} else if (parens_match(paren_history.top(), next_paren)) {
					paren_history.pop();
				} else {
					answer.first += CORRUPTED_CHAR_SCORE.at(next_paren);
					corrupted = true;
					break;
				}
			}

			if (!corrupted) {
				U64 s = 0;
				while (!paren_history.empty()) {
					s = 5 * s + INCOMPLETE_CHAR_SCORE.at(paren_history.top());
					paren_history.pop();
				}
				scores.push_back(s);
			}

			paren_history = {};
		}

		std::sort(scores.begin(), scores.end());
		answer.second = scores[scores.size() / 2];

		return answer;
	}
};

