#include "common.h"

void day2()
{
	const std::vector<std::string> input = read_file_lines_as<std::string>("2a.txt");

	{ // start part one
		S64 position = 0;
		S64 depth = 0;

		for (size_t idx = 0; idx < input.size() - 1; idx += 2) {
			const std::string& instruction = input[idx];
			const S64 X = std::stoll(input[idx+1]);

			if (instruction == "forward") {
				position += X;
			}
			else if (input[idx] == "up") {
				depth -= X;
			}
			else if (input[idx] == "down") {
				depth += X;
			}
			else {
				throw;
			}
		}

		std::cout << "2.1) " << depth * position << std::endl;
	} // end part one

	{ // start part two
		S64 aim = 0;
		S64 position = 0;
		S64 depth = 0;

		for (size_t idx = 0; idx < input.size() - 1; idx += 2) {
			const std::string& instruction = input[idx];
			const S64 X = std::stoll(input[idx+1]);

			if (instruction == "forward") {
				position += X;
				depth += aim * X;
			}
			else if (instruction == "up") {
				aim -= X;
			}
			else if (instruction == "down") {
				aim += X;
			}
		}

		std::cout << "2.2) " << depth * position << std::endl;
	} // end part two
}