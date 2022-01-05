#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <iterator>
#include <numeric>
#include <type_traits>
#include <vector>

namespace RS::Sci {

    // Integer algorithms

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

    // Range algorithms

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

    template <typename T>
    class PrecisionSum {

    public:

        static_assert(std::is_floating_point_v<T>);

        using value_type = T;

        PrecisionSum& add(T t) {
            size_t i = 0;
            for (T p: partials_) {
                if (std::abs(t) < std::abs(p))
                    std::swap(t, p);
                T sum = t + p;
                p -= sum - t;
                t = sum;
                if (p != T())
                    partials_[i++] = p;
            }
            partials_.erase(partials_.begin() + i, partials_.end());
            partials_.push_back(t);
            return *this;
        }

        PrecisionSum& operator()(T t) { return add(t); }
        void clear() noexcept { partials_.clear(); }
        T get() const { return std::accumulate(partials_.begin(), partials_.end(), T()); }
        operator T() const { return get(); }

    private:

        std::vector<T> partials_;

    };

    template <typename SinglePassRange>
    auto precision_sum(const SinglePassRange& range) {
        using std::begin;
        using T = std::decay_t<decltype(*begin(range))>;
        static_assert(std::is_floating_point_v<T>);
        PrecisionSum<T> sum;
        for (auto x: range)
            sum(x);
        return sum;
    }

}
