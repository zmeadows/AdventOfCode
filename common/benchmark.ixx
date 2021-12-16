export module benchmark;

import <chrono>;
import <cmath>;
import <compare>;
import <iomanip>;
import <iostream>;

using namespace std::chrono;

import types;


// https://www.reddit.com/r/adventofcode/comments/rdrewg/2021_my_aim_is_for_all_of_this_years_solutions_to/

/*

+-----------+--------------+--------------+--------------+--------------+
| AoC++2021 |   Parsing    |    Part 1    |    Part 2    |    Total     |
+-----------+--------------+--------------+--------------+--------------+
|  Day 01   |    16.874 us |     0.272 us |     0.271 us |    17.416 us |
|  Day 02   |    12.187 us |     2.756 us |     4.519 us |    19.462 us |
|  Day 03   |    17.009 us |     3.512 us |     6.418 us |    26.939 us |
|  Day 04   |    22.009 us |    14.765 us |    44.744 us |    81.517 us |
|  Day 05   |    20.069 us |   152.281 us |   240.249 us |   412.598 us |
|  Day 06   |     8.221 us |     0.002 us |     0.002 us |     8.224 us |
|  Day 07   |    10.203 us |    39.347 us |    82.093 us |   131.643 us |
|  Day 08   |    29.750 us |     0.286 us |     6.405 us |    36.441 us |
|  Day 09   |     7.263 us |    24.009 us |    43.157 us |    74.430 us |
|  Day 10   |    12.711 us |    19.809 us |    10.076 us |    42.596 us |
|  Day 11   |     7.211 us |    12.793 us |    28.501 us |    48.505 us |
|  Day 12   |    10.877 us |     2.947 us |    37.262 us |    51.086 us |
|  Day 13   |    14.234 us |     9.118 us |    15.637 us |    38.990 us |
+-----------+--------------+--------------+--------------+--------------+
|  Summary  |   188.616 us |   281.897 us |   519.333 us |   989.846 us |
+-----------+--------------+--------------+--------------+--------------+

D  PARSE      PART1      PART2
1  7.27µs     790ns      470ns
2  24.54µs    260ns      430ns
3  43.50µs    32.67µs    12.82µs
4  41.91µs    7.60µs     29.56µs
5  12.19µs    728.45µs   991.74µs
6  2.32µs     450ns      300ns
7  11.01µs    160.50µs   380.60µs
8  48.06µs    4.82µs     13.70µs
9  25.00µs    48.76µs    118.10µs
10 116.87µs   14.42µs -
11 200ns      230.28µs -

| Day             	| Mean time        	|
|-----------------	|---------------	|
| 1              	| 0.046 ms       	|
| 2              	| 0.001 ms       	|
| 3              	| 0.032 ms       	|
| 4              	| 0.165 ms       	|
| 5              	| 1.615 ms       	|
| 6              	| 0.001 ms       	|
| 7              	| 0.028 ms       	|
| 8              	| 0.052 ms       	|
| 9              	| 0.162 ms       	|
| 10              	| 0.026 ms       	|
| 11              	| 0.113 ms       	|
https://github.com/MichalMarsalek/Advent-of-code/tree/master/2021/Nim
*/

export template <typename Problem>
U64 bench()
{
    constexpr double relative_precision = 0.25;

    const auto input = Problem::prepare_input();

    // first find appropriate benchmark chunk size
    uint64_t in_chunks_of = 5;
    double chunk_time = 0.0;
    double total_test_time = 0;
    double total_test_iterations = 0.0;
    while (chunk_time < 0.5 && in_chunks_of < 1e12) {
        in_chunks_of *= 2;
        const auto chunk_start_time = high_resolution_clock::now();
        for (size_t i = 0; i < in_chunks_of; i++) {
            (void) Problem::solve(input);
        }
        const auto chunk_end_time = high_resolution_clock::now();
        chunk_time = duration_cast<duration<double>>(chunk_end_time - chunk_start_time).count();
        total_test_time += (double) chunk_time;
        total_test_iterations += (double) in_chunks_of;
    }

    const size_t bench_chunk_size = in_chunks_of;
    double count = 1.0;
    double mean = total_test_time / total_test_iterations;
    double M2 = mean;

    auto stddev = [&](void) {
        return std::sqrt(M2 / ((double)count * (double)bench_chunk_size));
    };

    do { // the actual iteration benchmarking
        const auto iter_start_time = high_resolution_clock::now();

        for (size_t ichunk = 0; ichunk < bench_chunk_size; ichunk++) {
            Problem::solve(input);
        }

        const auto iteration_end_time = high_resolution_clock::now();
        const double chunk_duration = duration_cast<duration<double>>(iteration_end_time - iter_start_time).count();
        const double mean_in_chunk = chunk_duration / (double)bench_chunk_size;

		count += 1.0;
		const double delta = mean_in_chunk - mean;
		mean += delta / count;
		const double delta2 = mean_in_chunk - mean;
		M2 += delta * delta2;

    } while (stddev() / std::abs(mean) > relative_precision && count < 1e7);

    const U64 time_microseconds = std::max(1ULL, static_cast<U64>(round(1e6 * mean)));
	std::cout << "Day " << Problem::DAY_NUMBER << " solve time: " << time_microseconds << " microseconds" << std::endl;

    return time_microseconds;
}

