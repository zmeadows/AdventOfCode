module;
#include <cassert>
export module day17;

import types;

import <string>;
import <utility>;

export struct Day17 {
	static constexpr U64 DAY_NUMBER = 17;
	static constexpr std::pair<U64, U64> SOLUTION = { 6786, 2313 };

	using InputType = U64;

	static constexpr S64 XMIN_TARGET = 155;
	static constexpr S64 XMAX_TARGET = 182;
	static constexpr S64 YMIN_TARGET = -117;
	static constexpr S64 YMAX_TARGET = -67;

	static InputType prepare_input() { return 0ULL; }

	static std::pair<U64, U64> solve(U64)
	{
		const S64 VX_MAX = XMAX_TARGET;
		const S64 VX_MIN = 1;
		const S64 VY_MIN = YMIN_TARGET;
		const S64 VY_MAX = std::abs(YMIN_TARGET) - 1;

		auto step_y = [](S64 v0, S64 n) -> S64 {
			return n * v0 - n * (n - 1) / 2;
		};

		auto step_x = [&](S64 v0, S64 n) -> S64 {
			return n > v0 ? step_y(v0, v0) : step_y(v0, n);
		};

		auto target_status = [&](S64 x, S64 y) -> S64 {
			if (x > XMAX_TARGET || y < YMIN_TARGET) {
				return -1;
			} else if (x >= XMIN_TARGET && y <= YMAX_TARGET) {
				return 1;
			} else {
				return 0;
			}
		};

		U64 count = 0;
		for (S64 vx = VX_MIN; vx <= VX_MAX; vx++) {
			for (S64 vy = VY_MIN; vy <= VY_MAX; vy++) {
				S64 n = 0;
				while (++n) {
					const S64 s = target_status(step_x(vx, n), step_y(vy, n));
					if (s == -1) {
						break;
					} else if (s == 1) {
						count++;
						break;
					}
				}
			}
		}

		return { step_y(VY_MAX, VY_MAX), count };
	}
};

