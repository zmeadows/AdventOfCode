#include "common.h"

void day2()
{
	const auto instructions = read_lines_as<std::string>("2a.txt");

	{ // start part one
		S64 position = 0;
		S64 depth = 0;

		for (size_t idx = 0; idx < instructions.size() - 1; idx += 2) {
			const std::string& command = instructions[idx];
			S64 X = std::stoll(instructions[idx + 1]);

			if (command == "forward") {
				position += X;
			}
			else if (command == "up") {
				depth -= X;
			}
			else if (command == "down") {
				depth += X;
			}
		}

		print("2.1) {}\n", depth * position);
	} // end part one

	{ // start part two
		S64 aim = 0;
		S64 position = 0;
		S64 depth = 0;

		for (size_t idx = 0; idx < instructions.size(); idx += 2) {
			const std::string& command = instructions[idx];
			S64 X = std::stoll(instructions[idx + 1]);

			if (command == "forward") {
				position += X;
				depth += aim * X;
			}
			else if (command == "up") {
				aim -= X;
			}
			else if (command == "down") {
				aim += X;
			}
		}

		print("2.2) {}\n", depth * position);
	} // end part two

	print("\n");
}