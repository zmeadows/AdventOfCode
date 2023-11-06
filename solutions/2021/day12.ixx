module;
#include <cassert>
export module day12;

import bitwise;
import parse;
import types;
import utils;

import <bitset>;
import <iostream>;
import <string>;
import <unordered_map>;
import <utility>;
import <vector>;

__forceinline U32 cache_index(U8 node_id, U64 visited, bool twice) {
	return static_cast<U32>(
		static_cast<U32>(visited << 16) | static_cast<U32>(node_id << 1) | (twice ? 1UL : 0UL)
    );
}

template <bool PART_TWO>
U32 traverse(
	const std::vector<U64>& adjacency_masks,
	const U8 current_id,
	const U64 small_mask,
	const U8 end_id,
	const bool twice,
	const U64 visited,
	std::unordered_map<U32, U32>& cache)
{
	const auto idx = cache_index(current_id, visited, twice);

	if (auto it = cache.find(idx); it != cache.end()) {
		return it->second;
	}

	const U64 new_visited = check_bit(small_mask, current_id) ? set_bit(visited, current_id) : visited;

	U32 npaths = 0;

	serialize(adjacency_masks[current_id], [&](U8 next_id) {
		if (next_id == end_id) {
			npaths++;
			return;
		}

		if constexpr (!PART_TWO) {
			if (!check_bit(new_visited, next_id))
				npaths += traverse<PART_TWO>(adjacency_masks, next_id, small_mask, end_id,
					                         twice, new_visited, cache);
		} else {
			if (check_bit(new_visited, next_id)) {
				if (!twice) {
					npaths += traverse<PART_TWO>(adjacency_masks, next_id, small_mask,
						                         end_id, true, new_visited, cache);
				}
			} else {
				npaths += traverse<PART_TWO>(adjacency_masks, next_id, small_mask,
					                         end_id, twice, new_visited, cache);
			}
		}
	});

	cache[idx] = npaths;

	return npaths;
}

export struct Day12 {
	static constexpr U64 DAY_NUMBER = 12;
	static constexpr std::pair<U64, U64> SOLUTION = { 4691, 140718 };
	using InputType = std::vector<std::string>;

	static InputType prepare_input() {
		return read_lines("2021/12a.txt");
	}

	static std::pair<U64, U64> solve(const InputType& edges)
	{
		U8 next_id = 0;
		U64 small_mask = 0;
		std::unordered_map<std::string, U8> identifiers;

		auto register_identifier = [&](const std::string& node_name) -> U8 {
			if (auto it = identifiers.find(node_name); it == identifiers.end()) {
				const bool is_small = std::islower(node_name[0]);
				auto rit = identifiers.emplace(node_name, next_id++);
				const U8 new_id = (*(rit.first)).second;
				if (is_small) small_mask = set_bit(small_mask, new_id);
				return new_id;
			} else {
				return identifiers[node_name];
			}
		};

		std::vector<U64> adjacency_masks;
		auto register_edge = [&](U8 a, U8 b)-> void {
			const U8 size_required = std::max(a, b) + 1;
			if (adjacency_masks.size() < size_required) adjacency_masks.resize(size_required, 0ULL);
			adjacency_masks[a] = set_bit(adjacency_masks[a], b);
			adjacency_masks[b] = set_bit(adjacency_masks[b], a);
		};

		for (const std::string& e : edges) {
			const U64 dash_loc = e.find('-');
			const U8 idA = register_identifier(std::string(e.begin(), e.begin() + dash_loc));
			const U8 idB = register_identifier(std::string(e.begin() + dash_loc + 1, e.end()));
			register_edge(idA, idB);
		}

		const U8 start_id = identifiers["start"];
		const U8 end_id = identifiers["end"];

		for (U64& mask : adjacency_masks) mask = clear_bit(mask, start_id);
		small_mask = clear_bit(small_mask, start_id);

		std::unordered_map<U32, U32> cache; cache.reserve(1024);

		std::pair<U64, U64> answer = {0ULL, 0ULL};
		answer.first = traverse<false>(adjacency_masks, start_id, small_mask, end_id, false, 0ULL, cache);
		cache.clear();
		answer.second = traverse<true>(adjacency_masks, start_id, small_mask, end_id, false, 0ULL, cache);

		return answer;
	}
};

