export module main;

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
import day11;
import day12;
import day13;
import day14;
import day15;
import day16;

import benchmark;
import utils;

import <iostream>;
import <iomanip>;

using LastDay = Day16;

enum class Mode {
    BenchmarkLatest,
    BenchmarkAll,
    TestLatest,
    TestAll
};

//constexpr Mode mode = Mode::BenchmarkLatest;
constexpr Mode mode = Mode::TestLatest;
//constexpr Mode mode = Mode::BenchmarkAll;
//constexpr Mode mode = Mode::TestAll;

export int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

	switch (mode) {
		case Mode::BenchmarkLatest: {
			bench<LastDay>();
			break;
		}
		case Mode::BenchmarkAll: {
			U64 total_microseconds = 0;
			total_microseconds += bench<Day1>();
			total_microseconds += bench<Day2>();
			total_microseconds += bench<Day3>();
			total_microseconds += bench<Day4>();
			total_microseconds += bench<Day5>();
			total_microseconds += bench<Day6>();
			total_microseconds += bench<Day7>();
			total_microseconds += bench<Day8>();
			total_microseconds += bench<Day9>();
			total_microseconds += bench<Day10>();
			total_microseconds += bench<Day11>();
			total_microseconds += bench<Day12>();
			total_microseconds += bench<Day13>();
			total_microseconds += bench<Day14>();
			total_microseconds += bench<Day15>();
			total_microseconds += bench<Day16>();
			print("Total: {} microseconds\n", total_microseconds);
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
			test<Day11>();
			test<Day12>();
			test<Day13>();
			test<Day14>();
			test<Day15>();
			test<Day16>();
			break;
		}
	}

	return 0;
}
