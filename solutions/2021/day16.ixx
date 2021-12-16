module;
#include <cassert>
export module day16;

import bitwise;
import parse;
import types;
import utils;

import decoder;

import <string>;
import <utility>;
import <vector>;
import <stdexcept>;

export struct Day16 {
	static constexpr U64 DAY_NUMBER = 16;
	static constexpr std::pair<U64, U64> SOLUTION = { 967, 12883091136209 };

	using InputType = std::string;

	static InputType prepare_input() { return read_line("2021/16a.txt", 1400); }

	static std::pair<U64, U64> solve(const std::string& transmission)
	{
		DecodedTransmission result = decode(transmission);
		return { result.version_sum, result.value };
	}
};

