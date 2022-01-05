#pragma once

#include <functional>
#include <iterator>
#include <type_traits>

namespace RS::Sci {

    template <typename T>
    constexpr T binomial(T a, T b) noexcept {
        static_assert(std::is_arithmetic_v<T>);
        if (b < 0 || b > a)
            return 0;
        if (b == 0 || b == a)
            return 1;
        if (b > a / 2)
            b = a - b;
        T n = 1;
        T d = 1;
        while (b > 0) {
            n *= a;
            d *= b;
            a -= 1;
            b -= 1;
        }
        return n / d;
    }

    template <typename ForwardRange, typename UnaryFunction, typename Compare>
    auto find_optimum(ForwardRange& range, UnaryFunction f, Compare cmp) {
        using std::begin;
        using std::end;
        auto i = begin(range);
        auto j = end(range);
        if (i == j)
            return j;
        auto opt = i;
        auto current = f(*i);
        for (++i; i != j; ++i) {
            auto value = f(*i);
            if (cmp(value, current)) {
                opt = i;
                current = value;
            }
        }
        return opt;
    }

    template <typename ForwardRange, typename UnaryFunction>
    auto find_optimum(ForwardRange& range, UnaryFunction f) {
        using std::begin;
        using T = std::decay_t<decltype(f(*begin(range)))>;
        return find_optimum(range, f, std::greater<T>());
    }

}
