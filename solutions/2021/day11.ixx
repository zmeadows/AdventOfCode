export module day11;

import types;
import parse;

import <array>;
import <string>;
import <utility>;

export struct Day11 {
	static constexpr U64 DAY_NUMBER = 11;
	static constexpr std::pair<U64, U64> SOLUTION = { 1634, 210 };
	using InputType = Grid<U8, 10, 10>;

	static InputType prepare_input() {
		InputType energy = {};

		U64 irow = 0;
		for (const std::string& rowstr : read_lines("2021/11a.txt")) {
			std::array<U8, 10> row = {};
			U64 icol = 0;
			for (char c : rowstr) {
				row[icol] = static_cast<U8>(c - '0');
				icol++;
			}
			energy[irow] = std::move(row);
			irow++;
		}

		return energy;
	}

	static std::pair<U64, U64> solve(const InputType& initial_energy)
	{
		std::pair<U64, U64> answer = { 0,0 };

		// Create an over-size grid with junk space on the borders so that
		// we don't have to do costly boolean checks on every operation to ensure
		// we are in-bounds of the grid.
		U8 energy[12][12] = { 0 };

		for (U64 irow = 0; irow < 10; irow++) {
			for (U64 icol = 0; icol < 10; icol++) {
				energy[irow + 1][icol + 1] = initial_energy[irow][icol];
			}
		}

		std::vector<std::pair<U64, U64>> flashing; flashing.reserve(10 * 10);

		U64 nstep = 1;

		while (nstep <= 100 || answer.second == 0) {
			// do the initial +1 increment each step
			for (U64 irow = 1; irow < 11; irow++) {
				for (U64 icol = 1; icol < 11; icol++) {
					if (++energy[irow][icol] == 10) [[unlikely]] {
						flashing.push_back({ irow, icol });
					}
				}
			}

			// cascade the flashing until finished
			while (!flashing.empty()) {
				const auto [irow, icol] = flashing.back(); flashing.pop_back();

				// ignore junk borders
				if (irow == 0 || icol == 0 || irow == 11 || icol == 11) [[unlikely]] continue;

				for (U64 jrow = irow - 1; jrow <= irow + 1; jrow++) {
					for (U64 jcol = icol - 1; jcol <= icol + 1; jcol++) {
						if (++energy[jrow][jcol] == 10) [[unlikely]] {
							flashing.push_back({ jrow, jcol });
						}
					}
				}
			}

			// flashing is complete, now add up and reset all flashers to zero energy
			U64 flashes_this_turn = 0;
			for (U64 irow = 1; irow < 11; irow++) {
				for (U64 icol = 1; icol < 11; icol++) {
					U8& e = energy[irow][icol];
					if (e > 9) [[unlikely]] {
						e = 0;
						flashes_this_turn++;
					}
				}
			}

			if (nstep <= 100) answer.first += flashes_this_turn;
			if (flashes_this_turn == 10 * 10) [[unlikely]] answer.second = nstep;

			nstep++;
		}

		return answer;
	}
};

