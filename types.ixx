module;
#include <cstdint>
export module types;

import <array>;

export using U64 = uint64_t;
export using U32 = uint32_t;
export using U16 = uint16_t;
export using U8 = uint8_t;
static_assert(sizeof(U64) == 8);
static_assert(sizeof(U32) == 4);
static_assert(sizeof(U16) == 2);
static_assert(sizeof(U8) == 1);

export using S64 = int64_t;
export using S32 = int32_t;
export using S16 = int16_t;
export using S8 = int8_t;
static_assert(sizeof(S64) == 8);
static_assert(sizeof(S32) == 4);
static_assert(sizeof(S16) == 2);
static_assert(sizeof(S8) == 1);

export using F32 = float;
export using F64 = double;
static_assert(sizeof(F64) == 8);
static_assert(sizeof(F32) == 4);

export template <typename T, size_t M, size_t N>
using Grid = std::array<std::array<T, N>, M>;

