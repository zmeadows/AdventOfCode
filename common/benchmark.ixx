export module benchmark;

import <chrono>;
import <cmath>;
import <compare>;
import <iomanip>;
import <iostream>;

using namespace std::chrono;

// https://www.reddit.com/r/adventofcode/comments/rdrewg/2021_my_aim_is_for_all_of_this_years_solutions_to/

/*
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
void bench()
{
    constexpr double relative_precision = 0.5;

    const auto input = Problem::prepare_input();

    // first find appropriate benchmark chunk size
    uint64_t in_chunks_of = 1;
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

        {
            count += 1.0;
            const double delta = mean_in_chunk - mean;
            mean += delta / count;
            const double delta2 = mean_in_chunk - mean;
            M2 += delta * delta2;
        }

        // const size_t total_iterations = (size_t)count * in_chunks_of;
        // std::cout << '\r' << "iterations completed: " << std::setw(2) << std::setfill('0') << total_iterations << std::flush;
    } while (stddev() / std::abs(mean) > relative_precision && count < 1e7);

	std::cout << "Day " << Problem::DAY_NUMBER << " solve time: " << 1e6 * mean << " microseconds" << std::endl;
}

