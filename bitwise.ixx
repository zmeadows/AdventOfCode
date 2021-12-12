export module bitwise;

import types;

export __forceinline constexpr U64 set_bit(U64 num, U64 idx) {
	return num | (U64(1) << idx);
}
export __forceinline constexpr U64 clear_bit(U64 num, U64 idx) {
	return num & ~(U64(1) << idx);
}
export __forceinline constexpr U64 toggle_bit(U64 num, U64 idx) {
	return num ^ (U64(1) << idx);
}
export __forceinline constexpr bool check_bit(U64 num, U64 idx) {
	return ((num >> idx) & U64(1)) > 0;
}

