#include "common.h"

void day1()
{
	const auto depths = read_lines_as<S64>("1a.txt");

	{ // start part one
		S64 X = 0;
		for (size_t idx = 1; idx < depths.size(); idx++) {
			X += depths[idx] > depths[idx - 1];
		}
		print("1.1) {}\n", X);
	} // end part one

	{ // start part two
		S64 X = 0;
		S64 last = depths[0] + depths[1] + depths[2];
		for (size_t idx = 3; idx < depths.size(); idx++) {
			S64 next = depths[idx - 2] + depths[idx - 1] + depths[idx];
			X += next > last;
			last = next;
		}

		print("1.2) {}\n", X);
	} // end part two

	print("\n");
}

