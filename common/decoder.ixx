module;
#include <cassert>
export module decoder;

import bitwise;
import types;

import <string>;
import <utility>;
import <vector>;

enum class TypeID {
	Sum,
	Product,
	Minimum,
	Maximum,
	Literal,
	GreaterThan,
	LessThan,
	EqualTo
};

struct PacketDecoder {
	std::vector<U8> bits = {};
	U64 tip = 0;
	U64 version_sum = 0;
	U64 result = 0;

	static PacketDecoder build(const std::string& transmission) {
		PacketDecoder decoder; decoder.bits.reserve(transmission.size() * 4);

		for (const char hex : transmission) {
			const U64 bitrep = static_cast<U64>(std::isdigit(hex) ? hex - '0' : 10 + hex - 'A');
			for (auto i = 3; i >= 0; i--) decoder.bits.push_back(check_bit(bitrep, i) ? 1 : 0);
		}

		return decoder;
	}
};

U64 parse_packet(PacketDecoder&);

export bool finished(PacketDecoder& decoder) {
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

static inline U64 read_bits(PacketDecoder& decoder, U64 nbits) {
	U64 result = 0;
	while (nbits > 0) result += decoder.bits[decoder.tip++] << --nbits;
	assert(decoder.tip <= decoder.bits.size());
	return result;
}

static inline std::pair<U64, U64> read_packet_header(PacketDecoder& decoder) {
	return { read_bits(decoder, 3), read_bits(decoder, 3) };
}

static std::vector<U64> parse_subpackets(PacketDecoder& decoder)
{
	std::vector<U64> results;

	const U64 length_type_id = read_bits(decoder, 1);
	assert(length_type_id == 0 || length_type_id == 1);

	if (length_type_id == 1) {
		const U64 subpacket_count = read_bits(decoder, 11);
		for (U64 ipacket = 0; ipacket < subpacket_count; ipacket++)
			results.push_back(parse_packet(decoder));
	} else {
		const U64 total_subpacket_bits = read_bits(decoder, 15);
		const U64 packet_end = decoder.tip + total_subpacket_bits;
		while (decoder.tip < packet_end)
			results.push_back(parse_packet(decoder));
	}

	return results;
}

static U64 parse_literal(PacketDecoder& decoder) {
	U64 result = 0;

	while (true) {
		const U64 five_bits = read_bits(decoder, 5);
		result = (result << 4) | (five_bits & 0b1111);
		if (!check_bit(five_bits, 4)) break;
	}

	return result;
}

template <TypeID type_id>
U64 parse_expression(PacketDecoder& decoder) {
	throw;
}

template <>
U64 parse_expression<TypeID::Sum>(PacketDecoder& decoder) {
	U64 result = 0;
	for (const U64 val : parse_subpackets(decoder)) result += val;
	return result;
}

template <>
U64 parse_expression<TypeID::Product>(PacketDecoder& decoder) {
	U64 result = 1;
	for (const U64 val : parse_subpackets(decoder))
		result *= val;
	return result;
}

template <>
U64 parse_expression<TypeID::Minimum>(PacketDecoder& decoder) {
	U64 result = std::numeric_limits<U64>::max();
	for (const U64 val : parse_subpackets(decoder))
		result = std::min(result, val);
	return result;
}

template <>
U64 parse_expression<TypeID::Maximum>(PacketDecoder& decoder) {
	U64 result = 0;
	for (const U64 val : parse_subpackets(decoder))
		result = std::max(result, val);
	return result;
}

template <>
U64 parse_expression<TypeID::GreaterThan>(PacketDecoder& decoder) {
	const auto& subpackets = parse_subpackets(decoder);
	assert(subpackets.size() == 2);
	return subpackets[0] > subpackets[1];
}

template <>
U64 parse_expression<TypeID::LessThan>(PacketDecoder& decoder) {
	const auto& subpackets = parse_subpackets(decoder);
	assert(subpackets.size() == 2);
	return subpackets[0] < subpackets[1];
}

template <>
U64 parse_expression<TypeID::EqualTo>(PacketDecoder& decoder) {
	const auto& subpackets = parse_subpackets(decoder);
	assert(subpackets.size() == 2);
	return subpackets[0] == subpackets[1];
}

U64 parse_packet(PacketDecoder& decoder) {
	const auto [version, type_id] = read_packet_header(decoder);

	decoder.version_sum += version;

	switch (type_id) {
	    case 0: { return parse_expression<TypeID::Sum>(decoder);         break; }
	    case 1: { return parse_expression<TypeID::Product>(decoder);     break; }
	    case 2: { return parse_expression<TypeID::Minimum>(decoder);     break; }
	    case 3: { return parse_expression<TypeID::Maximum>(decoder);     break; }
	    case 4: { return parse_literal(decoder);                         break; }
	    case 5: { return parse_expression<TypeID::GreaterThan>(decoder); break; }
	    case 6: { return parse_expression<TypeID::LessThan>(decoder);    break; }
	    case 7: { return parse_expression<TypeID::EqualTo>(decoder);     break; }
	    default: { throw; }
	}

}

export struct DecodedTransmission {
	U64 value = 0;
	U64 version_sum = 0;
};

export DecodedTransmission decode(const std::string& transmission) {
	auto decoder = PacketDecoder::build(transmission);

	while (!finished(decoder))
		decoder.result = parse_packet(decoder);

	return DecodedTransmission{ decoder.result, decoder.version_sum };
}
