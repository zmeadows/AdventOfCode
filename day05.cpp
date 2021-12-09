#include "common.h"

struct Vent {
	V2<S64> from;
	V2<S64> to;
};

void day5()
{
	const auto vent_input = read_lines_as<std::string>("5a.txt");

	std::vector<Vent> vents; vents.reserve(1 + vent_input.size() / 3);
	for (U64 i = 0; i < vent_input.size() - 2; i += 3)
		vents.emplace_back( Vent{vent_input[i], vent_input[i + 2]} );

	std::unordered_map< V2<S64>, U64, v2_hash_fn > counts_part_one, counts_part_two;

	for (const Vent& v : vents) {
		if (v.from.x == v.to.x) { // vertical
			for (S64 y = std::min(v.from.y, v.to.y); y <= std::max(v.from.y, v.to.y); y++) {
				counts_part_one[V2<S64>(v.from.x, y)]++;
				counts_part_two[V2<S64>(v.from.x, y)]++;
			}
		}
		else if (v.from.y == v.to.y) { // horizontal
			for (S64 x = std::min(v.from.x, v.to.x); x <= std::max(v.from.x, v.to.x); x++) {
				counts_part_one[V2<S64>(x, v.from.y)]++;
				counts_part_two[V2<S64>(x, v.from.y)]++;
			}
		}
		else { // diagonal
			V2<S64> delta( v.to.x > v.from.x ? 1 : -1, v.to.y > v.from.y ? 1 : -1 );
			for (V2<S64> tmp = v.from; tmp != v.to; tmp += delta)
				counts_part_two[tmp]++;
			counts_part_two[v.to]++;
		}
	}

	S64 overlap_part_one = 0, overlap_part_two = 0;
	for (const auto& [_, c] : counts_part_one)
		if (c > 1) overlap_part_one++;
	for (const auto& [_, c] : counts_part_two)
		if (c > 1) overlap_part_two++;

	print("5.1) {}\n", overlap_part_one);
	print("5.2) {}\n\n", overlap_part_two);
}
