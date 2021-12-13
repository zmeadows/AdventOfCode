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

// TODO: combine parts one and two, then optimize with cache
// https://gist.github.com/ahans/d6da7b0c4cad441b7fc7b362775e9642

static inline void traverse_one(
	const std::vector<U64>& adjacency_masks,
	const U8 current_id,
	const U64 small_mask,
	const U8 end_id,
	U64 smalls_visited,
	U64& path_count)
{
	if (current_id == end_id) {
		path_count++;
		return;
	}

	if (check_bit(small_mask, current_id)) [[likely]] {
		if (check_bit(smalls_visited, current_id)) return;
		else smalls_visited = set_bit(smalls_visited, current_id);
	}

	serialize(adjacency_masks[current_id], [&](U8 next_id) {
		traverse_one(adjacency_masks, next_id, small_mask, end_id, smalls_visited, path_count);
	});
}

static inline void traverse_two(
	const std::vector<U64>& adjacency_masks,
	const U8 current_id,
	const U64 small_mask,
	const U8 start_id,
	const U8 end_id,
	bool hit_small_cave_twice,
	U64 smalls_visited,
	U64& path_count)
{
	if (current_id == end_id) {
		path_count++;
		return;
	}

	if (check_bit(small_mask, current_id)) [[likely]] {
		if (check_bit(smalls_visited, current_id)) {
			if (current_id == start_id) {
				return;
			} else if (!hit_small_cave_twice) {
				hit_small_cave_twice = true;
				smalls_visited = set_bit(smalls_visited, current_id);
			} else {
				return;
			}
		}
		else {
			smalls_visited = set_bit(smalls_visited, current_id);
		}
	}

	serialize(adjacency_masks[current_id], [&](U8 next_id) {
		traverse_two(adjacency_masks, next_id, small_mask, start_id, end_id, hit_small_cave_twice, smalls_visited, path_count);
	});
}

export struct Day12 {
	static constexpr U64 DAY_NUMBER = 12;
	static constexpr std::pair<U64, U64> SOLUTION = { 4691, 0 };
	using InputType = std::vector<std::string>;

	static InputType prepare_input() {
		return read_lines("2021/12a.txt");
	}

	static std::pair<U64, U64> solve(const InputType& edges)
	{
		U8 next_id = 0;
		std::unordered_map<std::string, U8> identifiers;
		identifiers.reserve(edges.size() / 2 + 1);

		U64 small_mask = 0;

		auto register_identifier = [&](const std::string& node_name) -> U8 {
			if (auto it = identifiers.find(node_name); it == identifiers.end()) {
				bool is_small = true;
				for (char ch : node_name) {
					if (!std::islower(ch)) {
						is_small = false;
						break;
					}
				}
				auto rit = identifiers.emplace(node_name, next_id++);
				const U8 new_id = (*(rit.first)).second;
				if (is_small) small_mask = set_bit(small_mask, new_id);
				return new_id;
			}
			else {
				return identifiers[node_name];
			}
		};

		std::vector<U64> adjacency_masks;
		auto register_edge = [&](U8 a, U8 b)-> void {
			const U8 size_required = std::max(a, b) + 1;
			if (adjacency_masks.size() < size_required) {
				adjacency_masks.resize(size_required, 0ULL);
			}
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

		std::pair<U64, U64> answer = {0ULL, 0ULL};

		traverse_one(
			adjacency_masks,
			start_id,
			small_mask,
			end_id,
			0ULL,
			answer.first
		);

		traverse_two(
			adjacency_masks,
			start_id,
			small_mask,
			start_id,
			end_id,
			false,
			0ULL,
			answer.second
		);

		return answer;
	}
};

