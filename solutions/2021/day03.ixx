module;
#include <cassert>
export module day03;

import types;
import parse;
import bitwise;

import <vector>;

__forceinline S64 bit_balance(const std::vector<U64>& numbers, size_t idx) {
	S64 balance = 0;
	for (U64 n : numbers) {
		balance += check_bit(n, idx) ? 1 : -1;
	}
	return balance;
}

enum class Rating {
	OxygenGenerator,
	CO2Scrubber
};

U64 find_rating(const std::vector<U64>& report, Rating type, U64 max_bits) {
	std::vector<U64> buffer(report);

	S64 idx = max_bits - 1;
	while (buffer.size() > 1 && idx >= 0) {
		const S64 balance = bit_balance(buffer, idx);

		const bool bit_pref =
			type == Rating::OxygenGenerator ? balance >= 0 : balance < 0;

		std::erase_if(buffer, [&](U64 n) -> bool {
			return check_bit(n, idx) == bit_pref;
		});

		idx--;
	}

	assert(buffer.size() == 1);
	return buffer[0];
}

export struct Day3 {
	static constexpr U64 DAY_NUMBER = 3;
	static constexpr std::pair<U64, U64> SOLUTION = { 2261546, 6775520 };
	using InputType = std::pair<std::vector<U64>, U64>;

	static InputType prepare_input() {
		return read_binary_numbers_from_file("2021/3a.txt");
	}

	static std::pair<U64, U64> solve(const InputType& input)
	{
		const auto& [report, max_bits] = input;
		std::pair<U64, U64> answer = {};

		U64 gamma = 0, epsilon = 0;

		for (size_t idx = 0; idx < max_bits; idx++) {
			U64 ones = 0;
			for (const U64 number : report) {
				ones += check_bit(number, idx);
				if (ones > report.size() / 2) {
					gamma = set_bit(gamma, idx);
					break;
				}
			}
		}

		epsilon = ~gamma & ((1ULL << max_bits) - 1ULL);

		answer.first = gamma * epsilon;

		const U64 oxygen_rating = find_rating(report, Rating::OxygenGenerator, max_bits);
		const U64 scrubber_rating = find_rating(report, Rating::CO2Scrubber, max_bits);

		answer.second = oxygen_rating * scrubber_rating;

		return answer;
	}
};
