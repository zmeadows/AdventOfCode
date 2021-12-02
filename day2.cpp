#include "common.h"

void day2()
{
	auto in = readfile<S>("2a.txt");

	{ // start part one
		I pos = 0;
		I dep = 0;

		FORN(idx, in.size() - 1, 2) {
			S& Q = in[idx];
			I X = std::stoll(in[idx + 1]);

			if (Q == "forward") pos += X;
			else if (Q == "up") dep -= X;
			else if (Q == "down") dep += X;
			else throw;
		}

		out("2.1) {}\n", dep * pos);
	} // end part one

	{ // start part two
		I aim = 0;
		I pos = 0;
		I dep = 0;

		FORN(idx, in.size() - 1, 2) {
			S& Q = in[idx];
			I X = std::stoll(in[idx + 1]);

			if (Q == "forward") {
				pos += X;
				dep += aim * X;
			}
			else if (Q == "up") aim -= X;
			else if (Q == "down") aim += X;
			else throw;
		}

		out("2.2) {}\n", dep * pos);
	} // end part two
}