#pragma once

#include "rs-sci/constants.hpp"
#include "rs-sci/rational.hpp"
#include "rs-sci/traits.hpp"
#include "rs-format/enum.hpp"
#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <limits>
#include <map>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace RS::Sci {

    // Linear congruential generators

    constexpr uint32_t lcg32(uint32_t x) noexcept {
        return uint32_t(32'310'901ul * x + 850'757'001ul);
    }

    constexpr uint64_t lcg64(uint64_t x) noexcept {
        return uint64_t(3'935'559'000'370'003'845ull * x + 8'831'144'850'135'198'739ull);
    }

    class Lcg32 {
    public:
        using result_type = uint32_t;
        constexpr Lcg32() noexcept {}
        explicit constexpr Lcg32(uint32_t s) noexcept: state_(s) {}
        constexpr uint32_t operator()() noexcept { state_ = lcg32(state_); return state_; }
        constexpr bool operator==(const Lcg32& rhs) const noexcept { return state_ == rhs.state_; }
        constexpr bool operator!=(const Lcg32& rhs) const noexcept { return state_ != rhs.state_; }
        constexpr void seed(uint32_t s) noexcept { state_ = s; }
        static constexpr uint32_t min() noexcept { return 0; }
        static constexpr uint32_t max() noexcept { return ~ uint32_t(0); }
    private:
        uint32_t state_ = 0;
    };

    class Lcg64 {
    public:
        using result_type = uint64_t;
        constexpr Lcg64() noexcept {}
        explicit constexpr Lcg64(uint64_t s) noexcept: state_(s) {}
        uint64_t constexpr operator()() noexcept { state_ = lcg64(state_); return state_; }
        bool constexpr operator==(const Lcg64& rhs) const noexcept { return state_ == rhs.state_; }
        bool constexpr operator!=(const Lcg64& rhs) const noexcept { return state_ != rhs.state_; }
        void constexpr seed(uint64_t s) noexcept { state_ = s; }
        static constexpr uint64_t min() noexcept { return 0; }
        static constexpr uint64_t max() noexcept { return ~ uint64_t(0); }
    private:
        uint64_t state_ = 0;
    };

    // PCG generator by Melissa O'Neill
    // http://www.pcg-random.org/

    namespace Detail {

        template <typename T>
        constexpr T rotl(T t, int n) noexcept {
            int size = 8 * sizeof(T);
            n %= size;
            if (n == 0)
                return t;
            return T((t << n) | (t >> (size - n)));
        }

        template <typename T>
        constexpr T rotr(T t, int n) noexcept {
            int size = 8 * sizeof(T);
            n %= size;
            if (n == 0)
                return t;
            return T((t >> n) | (t << (size - n)));
        }

        struct Uint128 {
            uint64_t hi, lo;
            constexpr Uint128(): hi(0), lo(0) {}
            constexpr Uint128(uint64_t u): hi(0), lo(u) {}
            constexpr Uint128(uint64_t h, uint64_t l): hi(h), lo(l) {}
            constexpr void increment() noexcept;
            constexpr void add(Uint128 y) noexcept;
            constexpr void multiply(Uint128 y) noexcept;
            constexpr void shift_left(int y) noexcept;
            constexpr void shift_right(int y) noexcept;
        };

            constexpr void Uint128::increment() noexcept {
                ++lo;
                if (lo == 0)
                    ++hi;
            }

            constexpr void Uint128::add(Uint128 y) noexcept {
                hi += y.hi;
                lo += y.lo;
                if (lo < y.lo)
                    ++hi;
            }

            constexpr void Uint128::multiply(Uint128 y) noexcept {
                uint64_t mask = ~ uint32_t(0);
                uint64_t x0 = lo & mask;
                uint64_t x1 = lo >> 32;
                uint64_t x2 = hi & mask;
                uint64_t x3 = hi >> 32;
                uint64_t y0 = y.lo & mask;
                uint64_t y1 = y.lo >> 32;
                uint64_t y2 = y.hi & mask;
                uint64_t y3 = y.hi >> 32;
                hi = 0;
                lo = x3 * y0;
                add(x2 * y1);
                add(x1 * y2);
                add(x0 * y3);
                shift_left(32);
                add(x2 * y0);
                add(x1 * y1);
                add(x0 * y2);
                shift_left(32);
                add(x1 * y0);
                add(x0 * y1);
                shift_left(32);
                add(x0 * y0);
            }

            constexpr void Uint128::shift_left(int y) noexcept {
                if (y == 0) {
                    // pass
                } else if (y < 64) {
                    hi = (hi << y) | (lo >> (64 - y));
                    lo <<= y;
                } else if (y < 128) {
                    hi = lo << (y - 64);
                    lo = 0;
                } else {
                    hi = lo = 0;
                }
            }

            constexpr void Uint128::shift_right(int y) noexcept {
                if (y == 0) {
                    // pass
                } else if (y < 64) {
                    lo = (lo >> y) | (hi << (64 - y));
                    hi >>= y;
                } else if (y < 128) {
                    lo = hi >> (y - 64);
                    hi = 0;
                } else {
                    hi = lo = 0;
                }
            }

    }

    class Pcg64 {

    public:

        using result_type = uint64_t;

        constexpr Pcg64() noexcept: st_(0, default_seed_) { init(); }
        constexpr explicit Pcg64(uint64_t s) noexcept: st_(0, s) { init(); }
        constexpr explicit Pcg64(uint64_t hi, uint64_t lo) noexcept: st_(hi, lo) { init(); }

        constexpr uint64_t operator()() noexcept;

        constexpr void advance(int64_t offset) noexcept;
        constexpr void seed(uint64_t s) noexcept { st_ = {0, s}; init(); }
        constexpr void seed(uint64_t hi, uint64_t lo) noexcept { st_ = {hi, lo}; init(); }

        static constexpr uint64_t min() noexcept { return 0; }
        static constexpr uint64_t max() noexcept { return ~ uint64_t(0); }

    private:

        using state = Detail::Uint128;

        static constexpr uint64_t default_seed_ = 0xcafe'f00d'd15e'a5e5ull;
        static constexpr state a_ = {0x2360'ed05'1fc6'5da4ull, 0x4385'df64'9fcc'f645ull};
        static constexpr state b_ = {0x5851'f42d'4c95'7f2dull, 0x1405'7b7e'f767'814full};

        state st_;

        constexpr void init() noexcept;

    };

        constexpr uint64_t Pcg64::operator()() noexcept {
            st_.multiply(a_);
            st_.add(b_);
            uint64_t x = st_.hi ^ st_.lo;
            int y = int(st_.hi >> 58) & 63;
            return Detail::rotr(x, y);
        }

        constexpr void Pcg64::advance(int64_t offset) noexcept {
            state u = uint64_t(offset);
            if (offset < 0)
                u.hi = ~ uint64_t(0);
            state add;
            state mul = 1;
            auto c = a_;
            auto d = b_;
            while (u.hi != 0 || u.lo != 0) {
                if ((u.lo & 1) == 1) {
                    mul.multiply(c);
                    add.multiply(c);
                    add.add(d);
                }
                auto c1 = c;
                c1.increment();
                d.multiply(c1);
                c.multiply(c);
                u.shift_right(1);
            }
            st_.multiply(mul);
            st_.add(add);
        }

        constexpr void Pcg64::init() noexcept {
            st_.add(b_);
            st_.multiply(a_);
            st_.add(b_);
        }

    // Replacements for standard distributions
    // (to ensure reproducibility)

    template <typename T>
    class UniformInteger {

    public:

        static_assert(std::is_integral_v<T>);

        using result_type = T;

        constexpr UniformInteger() noexcept: min_(0), range_(2) {} // Defaults to 0-1
        constexpr explicit UniformInteger(T range) noexcept: min_(0), range_(range) {} // 0 to range-1; UB if range<1
        constexpr UniformInteger(T a, T b) noexcept: min_(a), range_(b - a + 1) {} // a to b inclusive; UB if a>b

        template <typename RNG>
        constexpr T operator()(RNG& rng) const noexcept {
            // This is a bit of a hack for efficiency and slightly improved
            // quality. I'm assuming that the actual range of rng() will
            // never be orders of magnitude smaller than its type, and that
            // the required output range will be much smaller.
            auto r = rng();
            if constexpr (sizeof(r) >= sizeof(uint64_t))
                r >>= 16;
            else if constexpr (sizeof(r) >= sizeof(uint32_t))
                r >>= 8;
            return min_ + r % range_;
        }

        constexpr T min() const noexcept { return min_; }
        constexpr T max() const noexcept { return min_ + (range_ - 1); }

    private:

        T min_;
        T range_;

    };

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

    class BernoulliDistribution {

    public:

        using result_type = bool;

        constexpr BernoulliDistribution() noexcept:
        BernoulliDistribution(1, 2) {}

        constexpr explicit BernoulliDistribution(double p) noexcept {
            constexpr long long den = 1ll << 32;
            auto num = std::llround(p * den);
            prob_ = {num, den};
        }

        constexpr explicit BernoulliDistribution(Rational p) noexcept:
        prob_(p) {}

        constexpr BernoulliDistribution(int num, int den) noexcept:
        prob_(Rational(num, den)) {}

        template <typename RNG>
        constexpr bool operator()(RNG& rng) const noexcept {
            return int_dist(prob_.den())(rng) < prob_.num();
        }

        constexpr Rational p() const noexcept { return prob_; }

    private:

        using int_type = Rational::integer_type;
        using int_dist = UniformInteger<int_type>;

        Rational prob_;

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

    RS_DEFINE_ENUM(LogMode, int, 0,
        natural,  // Use base e logs
        decimal   // Use base 10 logs
    )

    template <typename T>
    class LogNormal {

    public:

        static_assert(std::is_floating_point_v<T>);

        using result_type = T;

        LogNormal() noexcept {} // Defaults to (0,1)

        LogNormal(T m, T s, LogMode mode = LogMode::natural) noexcept {
            if (mode == LogMode::decimal) {
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

    template <typename T>
    class IntegerNormal {

    public:

        static_assert(std::is_integral_v<T>);

        using result_type = T;

        IntegerNormal() = default; // Defaults to (0,1)
        IntegerNormal(double mean, double sd) noexcept: norm_(mean, sd) {}

        template <typename RNG>
        T operator()(RNG& rng) const noexcept {
            return T(std::lround(norm_(rng)));
        }

        double mean() const noexcept { return norm_.mean(); }
        double sd() const noexcept { return norm_.sd(); }

    private:

        NormalDistribution<double> norm_;

    };

    // Distribution with resampling from a constrained range

    namespace Detail {

        template <typename T, typename = void> struct HasMinMethod: std::false_type {};
        template <typename T> struct HasMinMethod<T, std::void_t<decltype(std::declval<const T&>().min())>>: std::true_type {};
        template <typename T> constexpr bool has_min_method = HasMinMethod<T>::value;

        template <typename T, typename = void> struct HasMaxMethod: std::false_type {};
        template <typename T> struct HasMaxMethod<T, std::void_t<decltype(std::declval<const T&>().min())>>: std::true_type {};
        template <typename T> constexpr bool has_max_method = HasMaxMethod<T>::value;

    }

    template <typename Base>
    class ConstrainedDistribution {

    public:

        using base_distribution = Base;
        using result_type = typename Base::result_type;

        ConstrainedDistribution(const Base& dist, result_type min, result_type max);
        template <typename... Args> ConstrainedDistribution(result_type min, result_type max, Args&&... args);

        template <typename RNG> result_type operator()(RNG& rng) const;

        result_type min() const noexcept { return min_; }
        result_type max() const noexcept { return max_; }

    private:

        Base dist_;
        result_type min_ = std::numeric_limits<result_type>::lowest();
        result_type max_ = std::numeric_limits<result_type>::max();

        void check_range();

    };

        template <typename Base>
        ConstrainedDistribution<Base>::ConstrainedDistribution(const Base& dist, result_type min, result_type max):
        dist_(dist), min_(min), max_(max) {
            check_range();
        }

        template <typename Base>
        template <typename... Args>
        ConstrainedDistribution<Base>::ConstrainedDistribution(result_type min, result_type max, Args&&... args):
        dist_(std::forward<Args>(args)...), min_(min), max_(max) {
            check_range();
        }

        template <typename Base>
        template <typename RNG>
        typename ConstrainedDistribution<Base>::result_type
        ConstrainedDistribution<Base>::operator()(RNG& rng) const {
            result_type x;
            do x = dist_(rng);
                while (x < min_ || x > max_);
            return x;
        }

        template <typename Base>
        void ConstrainedDistribution<Base>::check_range() {
            if constexpr (Detail::has_min_method<Base>)
                min_ = std::max(min_, dist_.min());
            if constexpr (Detail::has_max_method<Base>)
                max_ = std::min(max_, dist_.max());
            if (min_ > max_)
                throw std::out_of_range("Constrained distribution has no possible values");
        }

    // Selection from a weighted set of discrete values

    template <typename T>
    class WeightedChoice {

    private:

        struct group {
            double weight = 0;
            std::vector<T> values;
            group() = default;
            template <typename... Args> group(double w, const Args&... args): weight(w), values{args...} {}
        };

    public:

        using value_type = T;

        WeightedChoice() = default;
        WeightedChoice(std::initializer_list<group> list);

        template <typename RNG>
        const T& operator()(RNG& rng) const;

        template <typename... Args> WeightedChoice& add(double w, const Args&... args); // Weight split between values
        double total_weight() const noexcept { return dist_.max(); }

    private:

        std::map<double, T> table_; // Cumulative => value
        UniformReal<double> dist_{0};

        void add_group(const group& g);

    };

        template <typename T>
        WeightedChoice<T>::WeightedChoice(std::initializer_list<group> list) {
            for (auto& g: list)
                add_group(g);
        }

        template <typename T>
        template <typename RNG>
        const T& WeightedChoice<T>::operator()(RNG& rng) const {
            double x = dist_(rng);
            auto it = table_.upper_bound(x);
            return it->second;
        }

        template <typename T>
        template <typename... Args>
        WeightedChoice<T>& WeightedChoice<T>::add(double w, const Args&... args) {
            group g(w, args...);
            add_group(g);
            return *this;
        }

        template <typename T>
        void WeightedChoice<T>::add_group(const group& g) {
            if (g.weight <= 0 || g.values.empty())
                return;
            T last = g.values.back();
            auto most = irange(g.values.begin(), g.values.end() - 1);
            double cw = dist_.max();
            double iw = g.weight / g.values.size();
            for (auto& t: most) {
                cw += iw;
                table_[cw] = t;
            }
            cw = dist_.max() + g.weight;
            table_[cw] = last;
            dist_ = UniformReal<double>(cw);
        }

}