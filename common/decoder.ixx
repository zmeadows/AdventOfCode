module;
#include <cassert>
export module decoder;

import bitwise;
import types;

import <algorithm>;
import <functional>;
import <numeric>;
import <string>;
import <utility>;
import <vector>;

struct PacketDecoder {
	std::vector<U8> bits = {};
	std::vector<U64> operand_buffer = {};
	U64 tip = 0;
	U64 version_sum = 0;
	U64 result = 0;

	static PacketDecoder initialize(const std::string& transmission) {
		PacketDecoder decoder;
		decoder.operand_buffer.reserve(static_cast<U64>(0.1 * transmission.size()));
		decoder.bits.reserve(transmission.size() * 4);

		for (const char hex : transmission) {
			const U64 bitrep = static_cast<U64>(std::isdigit(hex) ? hex - '0' : 10 + hex - 'A');
			for (auto i = 3; i >= 0; i--) decoder.bits.push_back(check_bit(bitrep, i) ? 1 : 0);
		}

		return decoder;
	}
};

U64 parse_packet(PacketDecoder&);

static __forceinline U64 read_bits(PacketDecoder& decoder, U64 nbits) {
	U64 result = 0;
	while (nbits > 0) result += decoder.bits[decoder.tip++] << --nbits;
	assert(decoder.tip <= decoder.bits.size());
	return result;
}

static __forceinline std::pair<U64, U64> read_packet_header(PacketDecoder& decoder) {
	return { read_bits(decoder, 3), read_bits(decoder, 3) };
}

static __forceinline auto parse_subpackets(PacketDecoder& decoder)
{
	const U64 start_idx = decoder.operand_buffer.size();

	const U64 length_type_id = read_bits(decoder, 1);
	assert(length_type_id == 0 || length_type_id == 1);

	if (length_type_id == 1) {
		const U64 subpacket_count = read_bits(decoder, 11);
		for (U64 ipacket = 0; ipacket < subpacket_count; ipacket++)
			decoder.operand_buffer.push_back(parse_packet(decoder));
	} else {
		const U64 total_subpacket_bits = read_bits(decoder, 15);
		const U64 packet_end = decoder.tip + total_subpacket_bits;
		while (decoder.tip < packet_end)
			decoder.operand_buffer.push_back(parse_packet(decoder));
	}

	const U64 end_idx = decoder.operand_buffer.size();

	return std::make_pair(
		decoder.operand_buffer.begin() + start_idx,
		decoder.operand_buffer.begin() + end_idx
	);
}

static __forceinline U64 parse_literal(PacketDecoder& decoder) {
	U64 result = 0;

	while (true) {
		const U64 five_bits = read_bits(decoder, 5);
		result = (result << 4) | (five_bits & 0b1111);
		if (!check_bit(five_bits, 4)) break;
	}

	return result;
}

U64 parse_packet(PacketDecoder& decoder) {
	const auto [version, type_id] = read_packet_header(decoder);

	assert(type_id >= 0 && type_id <= 7);

	decoder.version_sum += version;

	if (type_id == 4) {
		return parse_literal(decoder);
	}

	const auto& [opiter_start, opiter_end] = parse_subpackets(decoder);

	const U64 result = [&]() {
		switch (type_id) {
		    case 0: return std::reduce(opiter_start, opiter_end, 0ULL, std::plus<U64>());
		    case 1: return std::reduce(opiter_start, opiter_end, 1ULL, std::multiplies<U64>());
		    case 2: return *std::min_element(opiter_start, opiter_end);
		    case 3: return *std::max_element(opiter_start, opiter_end);
		    case 5: return static_cast<U64>(*opiter_start > *(opiter_start + 1));
		    case 6: return static_cast<U64>(*opiter_start < *(opiter_start + 1));
		    case 7: return static_cast<U64>(*opiter_start == *(opiter_start + 1));
		}
	}();

	decoder.operand_buffer.resize(std::distance(decoder.operand_buffer.begin(), opiter_start));

	return result;
}

export __forceinline bool finished(PacketDecoder& decoder) {
	if (decoder.tip >= decoder.bits.size()) return true;

	bool finished = true;
	for (U64 i = decoder.tip; i < decoder.bits.size(); i++) {
		if (decoder.bits[i] == 1) {
			finished = false;
			break;
		}
	}

	return finished;
}

export struct DecodedTransmission {
	U64 value = 0;
	U64 version_sum = 0;
};

export DecodedTransmission decode(const std::string& transmission) {
	auto decoder = PacketDecoder::initialize(transmission);

	while (!finished(decoder))
		decoder.result = parse_packet(decoder);

	assert(decoder.operand_buffer.empty());

	return DecodedTransmission{ decoder.result, decoder.version_sum };
}
