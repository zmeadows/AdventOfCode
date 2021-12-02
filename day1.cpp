#include "common.h"

void day1()
{
	const std::vector<S64> depths = read_file_lines_as<S64>("1a.txt");

	{ // start part one
		U64 answer = 0;

		for (size_t idx = 1; idx < depths.size(); idx++) {
			answer += static_cast<U64>(depths[idx] > depths[idx - 1]);
		}

		std::cout << "1.1) " << answer << std::endl;
	} // end part one

	{ // start part two
		U64 answer = 0;

		S64 last_sum = depths[0] + depths[1] + depths[2];
		for (size_t idx = 3; idx < depths.size(); idx++) {
			const S64 next_sum = depths[idx] + depths[idx - 1] + depths[idx - 2];
			answer += static_cast<U64>(next_sum > last_sum);
			last_sum = next_sum;
		}

		std::cout << "1.2) " << answer << std::endl;
	} // end part two
}

