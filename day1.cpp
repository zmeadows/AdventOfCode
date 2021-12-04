#include "common.h"

void day1()
{
	auto in = readfile<I>("1a.txt");

	{ // start part one
		I X = 0;
		FORS(Q, 1, in.size())
			X += in[Q] > in[Q - 1];
		out("1.1) {}\n", X);
	} // end part one

	{ // start part two
		I X = 0;
		I last = sum(in, 0, 2);
		FORS(idx, 3, in.size()) {
			I next = sum(in, idx - 2, idx);
			X += next > last;
			last = next;
		}

		out("1.2) {}\n", X);
	} // end part two

	out("\n");
}

