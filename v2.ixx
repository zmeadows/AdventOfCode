module;
#include <cassert>
#include <cmath>
#include <string>
export module v2;

import types;
import parse;

export template <typename T>
struct V2 {
    T x = static_cast<T>(0ULL);
    T y = static_cast<T>(0ULL);

    V2(T _x, T _y) : x(_x), y(_y) {}

    V2(const std::string& coords) {
        const auto s = split<T>(coords, ',');
        assert(s.size() == 2);
        this->x = s[0];
        this->y = s[1];
    }

    inline V2<T>& operator+=(const V2<T>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    inline V2<T>& operator-=(const V2<T>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    inline V2<T>& operator/=(T sf)
    {
        x /= sf;
        y /= sf;
        return *this;
    }

    inline F64 magnitude(void) const {
        return std::sqrt(static_cast<F64>(x * x) + static_cast<F64>(y * y));
    }
};

export template <typename T>
inline V2<T> operator*(const V2<T>& v, T sf) { 
    return { v.x * sf, v.y * sf };
}

export template <typename T>
inline V2<T> operator*(T sf, const V2<T>& v) {
    return { v.x * sf, v.y * sf };
}

export template <typename T>
inline V2<T> operator-(const V2<T>& lhs, const V2<T>& rhs) {
    return { lhs.x - rhs.x, lhs.y - rhs.y };
}

export template <typename T>
inline V2<T> operator/(const V2<T>& v, T sf) {
    return { v.x / sf, v.y / sf };
}

export template <typename T>
inline bool operator==(const V2<T>& lhs, const V2<T>& rhs) {
    return lhs.x == rhs.x && lhs.y == lhs.y;
}

export template <typename T>
inline bool operator!=(const V2<T>& lhs, const V2<T>& rhs) {
    return !(lhs == rhs);
}

export template <typename T>
inline F64 distance_sq(const V2<T>& a, const V2<T>& b)
{
    const T dx = a.x - b.x;
    const T dy = a.y - b.y;
    return static_cast<F64>(dx * dx) + static_cast<F64>(dy * dy);
}