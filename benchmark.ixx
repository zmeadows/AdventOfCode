module;
#include <chrono>
#include <iostream>
#include <iomanip>
#include <math.h>
export module benchmark;

using namespace std::chrono;

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
    while (chunk_time < 0.1 && in_chunks_of < 1e12) {
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

        const size_t total_iterations = (size_t)count * in_chunks_of;
        // std::cout << '\r' << "iterations completed: " << std::setw(2) << std::setfill('0') << total_iterations << std::flush;
    } while (stddev() / std::abs(mean) > relative_precision && count < 1e7);

    std::cout << std::endl;

	std::cout << "solve time: " << mean << " +/- " << stddev() << " (seconds)" << std::endl;

    std::cout << std::endl;
}

