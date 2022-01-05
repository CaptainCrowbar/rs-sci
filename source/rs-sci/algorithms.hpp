#pragma once

#include "rs-sci/traits.hpp"
#include "rs-graphics-core/vector.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <deque>
#include <functional>
#include <iterator>
#include <numeric>
#include <type_traits>
#include <utility>
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

    // Numerical algorithms

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

    namespace Detail {

        template <typename T>
        class LineIntegralIterator:
        public ForwardIterator<LineIntegralIterator<T>, const T> {

        public:

            LineIntegralIterator() = default;

            template <typename F> LineIntegralIterator(T x1, T x2, int k, F f):
            function_(f), start_x_(x1), delta_x_((x2 - x1) / k), prev_y_(f(x1)), area_element_(0), index_(0) {
                ++*this;
            }

            explicit LineIntegralIterator(int k) { index_ = k + 1; }

            const T& operator*() const noexcept { return area_element_; }
            LineIntegralIterator& operator++() {
                ++index_;
                T x = start_x_ + delta_x_ * index_;
                T y = function_(x);
                area_element_ = delta_x_ * (prev_y_ + y) / 2;
                prev_y_ = y;
                return *this;
            }
            bool operator==(const LineIntegralIterator& rhs) const noexcept { return index_ == rhs.index_; }

        private:

            std::function<T(T)> function_;
            T start_x_ = 0;
            T delta_x_ = 0;
            T prev_y_ = 0;
            T area_element_ = 0;
            int index_ = 0;

        };

        template <typename T, int N>
        class VolumeIntegralIterator:
        public ForwardIterator<VolumeIntegralIterator<T, N>, const T> {

        public:

            using vector_type = Graphics::Core::Vector<T, N>;

            VolumeIntegralIterator() = default;

            template <typename F> VolumeIntegralIterator(vector_type x1, vector_type x2, int k, F f):
                function_(f),
                cache_(),
                start_x_(x1),
                delta_x_((x2 - x1) / T(k)),
                index_(vector_type::null()),
                n_edge_(k),
                volume_factor_(std::ldexp(std::accumulate(delta_x_.begin(), delta_x_.end(), T(1), std::multiplies<T>()), - N)),
                volume_element_(get_volume()),
                done_(false) {}

            const T& operator*() const noexcept {
                return volume_element_;
            }

            VolumeIntegralIterator& operator++() {
                next_index();
                volume_element_ = get_volume();
                return *this;
            }

            bool operator==(const VolumeIntegralIterator& rhs) const noexcept {
                return done_ == rhs.done_ && (done_ || index_ == rhs.index_);
            }

        private:

            static constexpr int points = 1 << N;

            std::function<T(vector_type)> function_;
            std::deque<std::pair<vector_type, T>> cache_;
            vector_type start_x_;
            vector_type delta_x_;
            vector_type index_;
            int n_edge_;
            T volume_factor_;
            T volume_element_;
            bool done_ = true;

            void next_index() {
                int i = 0;
                while (i < N) {
                    index_[i] += 1;
                    if (index_[i] < n_edge_)
                        break;
                    index_[i++] = 0;
                }
                done_ = i == N;
            }

            T get_value(vector_type x) {
                static constexpr int cache_size = 2 * points;
                for (auto& xy: cache_)
                    if (xy.first == x)
                        return xy.second;
                T y = function_(x);
                cache_.push_back({x, y});
                if (cache_.size() > cache_size)
                    cache_.pop_front();
                return y;
            }

            T get_volume() {
                auto corner = start_x_ + delta_x_ * index_;
                std::array<T, points> y;
                for (int i = 0; i < points; ++i) {
                    auto x = corner;
                    for (int j = 0; j < N; ++j)
                        x[j] += delta_x_[j] * ((i >> j) & 1);
                    y[i] = get_value(x);
                }
                return volume_factor_ * precision_sum(y);
            }

        };

    }

    template <typename T, typename F>
    T line_integral(T x1, T x2, int k, F f) {
        static_assert(std::is_floating_point_v<T>);
        static_assert(std::is_convertible_v<F, std::function<T(T)>>);
        using iterator = Detail::LineIntegralIterator<T>;
        iterator i(x1, x2, k, f);
        iterator j(k);
        return precision_sum(irange(i, j));
    }

    template <typename T, int N, typename F>
    T volume_integral(Graphics::Core::Vector<T, N> x1, Graphics::Core::Vector<T, N> x2, int k, F f) {
        static_assert(std::is_floating_point_v<T>);
        static_assert(N > 0);
        static_assert(std::is_convertible_v<F, std::function<T(Graphics::Core::Vector<T, N>)>>);
        using iterator = Detail::VolumeIntegralIterator<T, N>;
        iterator i(x1, x2, k, f);
        iterator j;
        return precision_sum(irange(i, j));
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

}
