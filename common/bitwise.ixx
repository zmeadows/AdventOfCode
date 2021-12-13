export module bitwise;

import types;

export __forceinline constexpr U64 set_bit(U64 num, U8 idx) {
	return num | (U64(1) << idx);
}
export __forceinline constexpr U64 clear_bit(U64 num, U8 idx) {
	return num & ~(U64(1) << idx);
}
export __forceinline constexpr U64 toggle_bit(U64 num, U8 idx) {
	return num ^ (U64(1) << idx);
}
export __forceinline constexpr bool check_bit(U64 num, U8 idx) {
	return ((num >> idx) & U64(1)) > 0;
}

export template <typename Callable>
constexpr inline void serialize(U64 bb, Callable&& f)
{
	auto bsf = [](U64 bb) -> U8 {
		unsigned long idx = 0;
		_BitScanForward64(&idx, bb);
		return static_cast<U8>(idx);
	};

	while (bb != 0) {
		f(bsf(bb));
		bb &= (bb - 1);
	}
}
