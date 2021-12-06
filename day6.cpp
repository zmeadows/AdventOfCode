#include "common.h"

static U64 fish_population(std::array<U64, 9> timers, U64 ndays)
{
	while (ndays > 0) {
		const U64 birth_count = timers[0];
		timers[0] = 0;

		for (U64 idx = 1; idx < 9; idx++)
			timers[idx - 1] = timers[idx];

		timers[8] = birth_count;
		timers[6] += birth_count;

		ndays--;
	}

	return std::accumulate(timers.begin(), timers.end(), 0ULL);
}

void day6()
{
	const std::vector<std::string> input = read_lines_as<std::string>("6a.txt");

	std::array<U64, 9> initial_timers = { 0 };
	for (const U64 t : split<U64>(input[0], ','))
		initial_timers[t]++;

	print("6.1) {}\n", fish_population(initial_timers, 80));
	print("6.2) {}\n\n", fish_population(initial_timers, 256));
}
