#include "common.h"

using Board = Grid<S64, 5, 5>;

static __forceinline bool check_row(
	const Board& board, const std::unordered_set<S64>& calls, size_t row_idx)
{
	for (size_t col_idx = 0; col_idx < 5; col_idx++) {
		if (calls.find(board[row_idx][col_idx]) == calls.end()) {
			return false;
		}
	}

	return true;
}

static __forceinline bool check_col(
	const Board& board, const std::unordered_set<S64>& calls, size_t col_idx)
{
	for (size_t row_idx = 0; row_idx < 5; row_idx++) {
		if (calls.find(board[row_idx][col_idx]) == calls.end()) {
			return false;
		}
	}

	return true;
}

static __forceinline bool check_board(const Board& board, const std::unordered_set<S64>& calls)
{
	for (size_t row_idx = 0; row_idx < 5; row_idx++) {
		if (check_row(board, calls, row_idx)) {
			return true;
		}
	}

	for (size_t col_idx = 0; col_idx < 5; col_idx++) {
		if (check_col(board, calls, col_idx)) {
			return true;
		}
	}

	return false;
}

static S64 score_board(
	const Board& board,
	const std::unordered_set<S64>& winning_calls,
	const std::vector<S64>& call_order)
{
	S64 unmarked_sum = 0;
	for (size_t row_idx = 0; row_idx < 5; row_idx++) {
		for (size_t col_idx = 0; col_idx < 5; col_idx++) {
			const S64 entry = board[row_idx][col_idx];
			if (winning_calls.find(entry) == winning_calls.end()) {
				unmarked_sum += entry;
			}
		}
	}

	return unmarked_sum * call_order.at(winning_calls.size() - 1);
}

void day4()
{
	std::ifstream input = open_input_file("4a.txt");

	std::string call_order_string;
	std::getline(input, call_order_string);
	const std::vector<S64> call_order = split<S64>(call_order_string, ',');

	std::vector<Board> boards;
	boards.reserve(256);

	Board b;
	while (parse_grid(input, b)) {
		boards.push_back(b);
	}

	assert(!boards.empty());

	{ // start part one
		std::optional<size_t> winning_board_idx;
		std::unordered_set<S64> first_N_calls;
		S64 winning_score = 0;

		for (size_t N = 5; N < call_order.size(); N++) {
			if (winning_board_idx.has_value()) {
				const Board& winning_board = boards[*winning_board_idx];
				winning_score = score_board(winning_board, first_N_calls, call_order);
				break;
			}

			first_N_calls = std::unordered_set<S64>(call_order.begin(), call_order.begin() + N);
			for (size_t board_idx = 0; board_idx < boards.size(); board_idx++) {
				if (check_board(boards[board_idx], first_N_calls)) {
					winning_board_idx = board_idx;
					break;
				}
			}
		}

		assert(winning_board_idx.has_value());

		print("4.1) {}\n", winning_score);
	} // end part one

	{ // start part two
		std::unordered_set<S64> first_N_calls;
		S64 score = 0;

		for (size_t N = call_order.size()-1; N >= 1; N--) {
			if (score > 0) {
				break;
			}

			first_N_calls = std::unordered_set<S64>(call_order.begin(), call_order.begin() + N);
			for (size_t board_idx = 0; board_idx < boards.size(); board_idx++) {
				if (!check_board(boards[board_idx], first_N_calls)) {
					first_N_calls.insert(call_order[N]);
					score = score_board(boards[board_idx], first_N_calls, call_order);
					break;
				}
			}
		}

		assert(score > 0);

		print("4.2) {}\n", score);
	} // end part two

	print("\n");
}

