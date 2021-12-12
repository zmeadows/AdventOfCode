export module day02;

import types;
import parse;

import <utility>;
import <string>;
import <vector>;

export struct Day2 {
	static constexpr U64 DAY_NUMBER = 2;
	static constexpr std::pair<U64, U64> SOLUTION = { 1947824, 1813062561 };
	using InputType = std::vector<std::string>;

	static InputType prepare_input() {
		return read_lines_as<std::string>("2021/2a.txt");
	}

	static std::pair<U64, U64> solve(const InputType& instructions)
	{
		S64 position = 0, depth = 0, aim2 = 0, position2 = 0, depth2 = 0;

		for (size_t idx = 0; idx < instructions.size() - 1; idx += 2) {
			const std::string& command = instructions[idx];
			const S64 X = static_cast<S64>(instructions[idx + 1][0] - '0');

			if (command == "forward") {
				position += X;
				position2 += X;
				depth2 += aim2 * X;
			} else if (command == "up") {
				depth -= X;
				aim2 -= X;
			} else if (command == "down") {
				depth += X;
				aim2 += X;
			}
		}

		return { depth * position, depth2 * position2 };
	}
};

