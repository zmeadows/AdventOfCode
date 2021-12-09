#include "common.h"

static void climb_basin(std::vector<std::vector<S64>>& heights, S64 init_row, S64 init_col, S64& size)
{
	// Trying to make a CPU/memory efficient flood fill here, rather than the typical naive algorithms
	const S64 COL_COUNT = heights[0].size();

	auto on_map = [&](const S64 irow, const S64 icol) -> bool {
		return !(irow < 0 || irow >= (S64)heights.size() || icol < 0 || icol >= (S64)heights[irow].size());
	};

	if (!on_map(init_row, init_col)) return;

	std::queue<std::pair<S64, S64>> Q;

	auto push_location = [&](const std::pair<S64, S64>& loc) {
		Q.push(loc);
		heights[loc.first][loc.second] = 9;
	};

	auto queue_above_below = [&](const S64 irow, const S64 icol) {
		if (on_map(irow + 1, icol) && heights[irow + 1][icol] != 9)
			push_location({ irow + 1, icol });
		if (on_map(irow - 1, icol) && heights[irow - 1][icol] != 9)
			push_location({ irow - 1, icol });
	};

	auto scan_row = [&](const S64 irow, const S64 icol, const S64 delta) {
		const S64 bound = delta > 0 ? COL_COUNT : -1;
		for (S64 jcol = icol + delta; jcol != bound; jcol += delta) {
			if (heights[irow][jcol] == 9) break;
			size++;
			heights[irow][jcol] = 9;
			queue_above_below(irow, jcol);
		}
	};

	push_location({ init_row, init_col });

	while (!Q.empty()) {
		const auto [irow, icol] = Q.front(); Q.pop();
		size++;

		queue_above_below(irow, icol);

		// Perform east/west span filling for better CPU cache access patterns and smaller queue size.
	    // With this method the maximum queue size is around 20, for this problem.
		scan_row(irow, icol, 1);
		scan_row(irow, icol, -1);
	}
}

static __forceinline S64 compute_basin_size(std::vector<std::vector<S64>>& heights, S64 irow, S64 icol)
{
	S64 size = 0;
	climb_basin(heights,  irow, icol, size);
	return size;
}

void day9()
{
	std::vector<std::vector<S64>> heights;
	for (const std::string& rowstr : read_lines("9a.txt")) {
		std::vector<S64> row; row.reserve(rowstr.size());
		for (char c : rowstr)
			row.push_back(static_cast<S64>(c - '0'));
		heights.emplace_back(std::move(row));
	}

	const S64 ROW_COUNT = heights.size();
	const S64 COL_COUNT = heights[0].size();

	S64 risk_level_sum = 0;

	std::set<S64> basins;
	for (S64 irow = 0; irow < ROW_COUNT; irow++) {
		for (S64 icol = 0; icol < COL_COUNT; icol++) {
			const S64 h = heights[irow][icol];

			const bool is_low_point =
				   (irow == ROW_COUNT - 1 || h < heights[irow + 1][icol])
				&& (irow == 0             || h < heights[irow - 1][icol])
				&& (icol == COL_COUNT - 1 || h < heights[irow][icol + 1])
				&& (icol == 0             || h < heights[irow][icol - 1]);

			if (is_low_point) {
				risk_level_sum += h + 1;
				basins.insert(compute_basin_size(heights, irow, icol));
				// Only keep the three largest basins at any given moment, to save memory.
				if (basins.size() > 3) basins.erase(basins.begin());
			}
		}
	}

	print("9.1) {}\n", risk_level_sum);
	print("9.2) {}\n\n", std::accumulate(basins.begin(), basins.end(), 1LL, std::multiplies<S64>()));
}
