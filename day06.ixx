module;
#include "common.h"
export module day06;

export struct Day6 {
	static constexpr U64 DAY_NUMBER = 6;
	static constexpr std::pair<U64, U64> SOLUTION = { 351188, 1595779846729 };
	using InputType = std::array<U64, 9>;

	static InputType prepare_input() 
	{
		const std::string input = read_lines_as<std::string>("6a.txt")[0];

		std::array<U64, 9> initial_timers = { 0 };
		for (const U64 t : split<U64>(input, ','))
			initial_timers[t]++;

		return initial_timers;
	}

	static std::pair<U64, U64> solve(const InputType& initial_timers)
	{
		auto fish_population = [](std::array<U64, 9> timers, U64 ndays) -> U64
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
		};

		return {
			fish_population(initial_timers, 80),
			fish_population(initial_timers, 256)
		};
	}
};

