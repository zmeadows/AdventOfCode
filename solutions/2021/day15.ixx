export module day15;

import parse;
import types;
import utils;

import <algorithm>;
import <limits>;
import <queue>;
import <deque>;
import <string>;
import <utility>;
import <vector>;

// OPTIMIZE: Replace STL priority_queue with fibonacci heap based min-priority-queue.

export struct Day15 {
	static constexpr U64 DAY_NUMBER = 15;
	static constexpr std::pair<U64, U64> SOLUTION = { 652, 2938 };

	using InputType = std::vector<std::vector<U8>>;

	static InputType prepare_input() 
	{
		const auto lines = read_lines("2021/15a.txt");
		const U64 ROWS = lines.size();
		const U64 COLS = lines[0].size();

		std::vector<std::vector<U8>> matrix; matrix.reserve(ROWS);

		for (const std::string& rowstr : lines) {
			matrix.push_back({}); matrix.back().reserve(COLS);
			for (const char ch : rowstr) {
				matrix.back().push_back(static_cast<U8>(ch - '0'));
			}
		}

		return matrix;
	}

	struct Cell {
		U16 x;
		U16 y;
		U32 distance;

		inline bool operator>(const Cell& other) const {
			return this->distance > other.distance;
		}
	};

	static U64 find_lowest_risk(const std::vector<std::vector<U8>>& matrix)
	{
		const U16 ROWS = matrix.size();
		const U16 COLS = matrix[0].size();

		std::vector<std::pair<U16, U16>> neighbors; neighbors.reserve(4);
		auto compute_neighbors = [&](U16 x, U16 y) {
			neighbors.clear();
			if (x > 0) neighbors.emplace_back(x - 1, y);
			if (x < COLS - 1) neighbors.emplace_back(x + 1, y);
			if (y > 0) neighbors.emplace_back(x, y - 1);
			if (y < ROWS - 1) neighbors.emplace_back(x, y + 1);
		};

		std::priority_queue<Cell, std::vector<Cell>, std::greater<Cell>> Q;
		std::vector<std::vector<U32>> distances(ROWS, std::vector<U32>(COLS, std::numeric_limits<U32>::max()));
		std::vector<std::vector<bool>> unvisited(ROWS, std::vector<bool>(COLS, true));

		distances[0][0] = 0;
		unvisited[0][0] = false;
		Q.emplace(Cell{ 0,0,0 });

		U16 x_current = 0, y_current = 0;
		while (unvisited.back().back()) {
			const U64 distance_current = distances[y_current][x_current];

			compute_neighbors(x_current, y_current);
			for (const auto& [nx, ny] : neighbors) {
				const U32 new_distance = distance_current + matrix[ny][nx];
				if (new_distance < distances[ny][nx]) {
					distances[ny][nx] = new_distance;
					Q.emplace(Cell{ nx, ny, new_distance });
				}
			}

			unvisited[y_current][x_current] = false;

			while (!Q.empty()) {
				Cell next = Q.top();
				Q.pop();
				if (unvisited[next.y][next.x] && next.distance == distances[next.y][next.x]) {
					x_current = next.x;
					y_current = next.y;
					break;
				}
			}
		}

		return static_cast<U64>(distances.back().back());
	}

	static std::pair<U64, U64> solve(const InputType& input)
	{
		std::vector<std::vector<U8>> matrix(input);

		const U64 ROWS = matrix.size();
		const U64 COLS = matrix[0].size();
		const U64 BIG_ROWS = 5 * ROWS;
		const U64 BIG_COLS = 5 * COLS;

		std::vector<std::vector<U8>> big_matrix;
		big_matrix.reserve(BIG_ROWS);

		for (U64 irow = 0; irow < BIG_ROWS; irow++)
			big_matrix.emplace_back(BIG_COLS, 0ULL);

		for (U64 j = 0; j < 5; j++) {
			for (U64 i = 0; i < 5; i++) {
				for (U64 y = 0; y < ROWS; y++) {
					for (U64 x = 0; x < COLS; x++) {
						U8 new_val = input[y][x] + i + j;
						if (new_val > 9) new_val -= 9;
						big_matrix[j * ROWS + y][i * COLS + x] = new_val;
					}
				}
			}
		}

		return { find_lowest_risk(matrix), find_lowest_risk(big_matrix) };
	}
};

