#include "common.h"

void day2()
{
	const auto instructions = read_lines_as<std::string>("2a.txt");

	S64 position = 0;
	S64 depth = 0;
	S64 aim2 = 0;
	S64 position2 = 0;
	S64 depth2 = 0;

	for (size_t idx = 0; idx < instructions.size() - 1; idx += 2) {
		const std::string& command = instructions[idx];
		const S64 X = std::stoll(instructions[idx + 1]);

		if (command == "forward") {
			position += X;
			position2 += X;
			depth2 += aim2 * X;
		}
		else if (command == "up") {
			depth -= X;
			aim2 -= X;
		}
		else if (command == "down") {
			depth += X;
			aim2 += X;
		}
	}

	print("2.1) {}\n", depth * position);
	print("2.2) {}\n\n", depth2 * position2);
}