#include <iostream>

#include "common.h"

import benchmark;
import day01;
import day02;
import day03;
import day04;
import day05;
import day06;
import day07;
import day08;
import day09;
import day10;

using LastDay = Day10;

enum class Mode {
    BenchmarkLatest,
    BenchmarkAll,
    TestLatest,
    TestAll
};

//constexpr Mode mode = Mode::BenchmarkLatest;
//constexpr Mode mode = Mode::TestLatest;
//constexpr Mode mode = Mode::BenchmarkAll;
constexpr Mode mode = Mode::TestAll;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

	switch (mode) {
		case Mode::BenchmarkLatest: {
			bench<LastDay>();
			break;
		}
		case Mode::BenchmarkAll: {
			bench<Day1>();
			bench<Day2>();
			bench<Day3>();
			bench<Day4>();
			bench<Day5>();
			bench<Day6>();
			bench<Day7>();
			bench<Day8>();
			bench<Day9>();
			bench<Day10>();
			break;
		}
		case Mode::TestLatest: {
			test<LastDay>();
			break;

		}
		case Mode::TestAll: {
			test<Day1>();
			test<Day2>();
			test<Day3>();
			test<Day4>();
			test<Day5>();
			test<Day6>();
			test<Day7>();
			test<Day8>();
			test<Day9>();
			test<Day10>();
			break;
		}
	}

	return EXIT_SUCCESS;
}
