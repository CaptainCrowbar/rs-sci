// This header is not part of the public interface;
// include "rs-sci/random.hpp" instead.

#pragma once

#include "rs-sci/constants.hpp"
#include "rs-format/enum.hpp"
#include <cmath>
#include <type_traits>

namespace RS::Sci {

    RS_DEFINE_ENUM_CLASS(LogMode, int, 0,
        natural,  // Use base e logs
        common    // Use base 10 logs
    )

    template <typename T>
    class UniformReal {

    public:

        static_assert(std::is_floating_point_v<T>);

        using result_type = T;

        constexpr UniformReal() noexcept: UniformReal(0, 1) {} // Defaults to 0-1
        constexpr explicit UniformReal(T range) noexcept: UniformReal(0, range) {} // 0 to range; UB if range<0
        constexpr UniformReal(T a, T b) noexcept: min_(a), range_(b - a) {} // a to b; UB if a>b

        template <typename RNG>
        constexpr T operator()(RNG& rng) const noexcept {
            return min_ + (range_ / (T(rng.max()) + 1)) * rng();
        }

        constexpr T min() const noexcept { return min_; }
        constexpr T max() const noexcept { return min_ + range_; }

    private:

        T min_;
        T range_;

    };

    template <typename T>
    class NormalDistribution {

    public:

        static_assert(std::is_floating_point_v<T>);

        using result_type = T;

        NormalDistribution() noexcept {} // Defaults to (0,1)
        NormalDistribution(T mean, T sd) noexcept: unit_(), mean_(mean), sd_(sd) {}

        template <typename RNG>
        T operator()(RNG& rng) const noexcept {
            T u = 1 - unit_(rng); // to ensure log(u) doesn't fail
            T v = unit_(rng);
            T a = std::sqrt(-2 * std::log(u));
            T b = std::cos(2 * pi_c<T> * v);
            return a * b * sd_ + mean_;
        }

        T mean() const noexcept { return mean_; }
        T sd() const noexcept { return sd_; }

    private:

        UniformReal<T> unit_;
        T mean_ = 0;
        T sd_ = 1;

    };

    template <typename T>
    class LogNormal {

    public:

        static_assert(std::is_floating_point_v<T>);

        using result_type = T;

        LogNormal() noexcept {} // Defaults to (0,1)

        LogNormal(T m, T s, LogMode mode = LogMode::natural) noexcept {
            if (mode == LogMode::common) {
                m *= ln10_c<T>;
                s *= ln10_c<T>;
            }
            norm_ = NormalDistribution<T>(m, s);
        }

        template <typename RNG>
        T operator()(RNG& rng) const noexcept {
            return std::exp(norm_(rng));
        }

        T m() const noexcept { return norm_.mean(); }
        T s() const noexcept { return norm_.sd(); }
        T median() const noexcept { return std::exp(m()); }
        T s_factor() const noexcept { return std::exp(s()); }

    private:

        NormalDistribution<T> norm_;

    };

    template <typename T>
    class LogUniform {

    public:

        static_assert(std::is_floating_point_v<T>);

        using result_type = T;

        constexpr LogUniform() noexcept: base_(1) {} // Defaults to 1-e
        constexpr LogUniform(T min, T max) noexcept: base_(std::log(min), std::log(max)) {} // UB if min<=0 or max<=0 or min>max

        template <typename RNG>
        constexpr T operator()(RNG& rng) const noexcept {
            return std::exp(base_(rng));
        }

        constexpr T min() const noexcept { return std::exp(base_.min()); }
        constexpr T max() const noexcept { return std::exp(base_.max()); }

    private:

        UniformReal<T> base_;

    };

}
