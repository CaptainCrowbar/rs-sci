#pragma once

#include "rs-sci/constants.hpp"
#include "rs-sci/rational.hpp"
#include "rs-format/enum.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-tl/fixed-binary.hpp"
#include "rs-tl/iterator.hpp"
#include "rs-tl/types.hpp"
#include "rs-tl/uuid.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <map>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace RS::Sci {

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

    }

    // Supporting types

    RS_DEFINE_ENUM_CLASS(LogMode, int, 0,
        natural,  // Use base e logs
        common   // Use base 10 logs
    )

    // Hash functions

    namespace Detail {

        constexpr size_t mix_hashes(size_t h1, size_t h2) noexcept {
            return h1 ^ ((h1 << 6) + (h1 >> 2) + h2 + 0x9e3779b9);
        }

    }

    template <typename T, typename... Args>
    size_t hash_mix(const T& t, const Args&... args) {
        size_t h = std::hash<T>()(t);
        if constexpr (sizeof...(Args) > 0)
            h = Detail::mix_hashes(h, hash_mix(args...));
        return h;
    }

    template <typename Range>
    size_t hash_mix(const Range& r) {
        using std::begin;
        using T = std::decay_t<decltype(*begin(r))>;
        std::hash<T> t_hash;
        size_t h = 0;
        for (auto& x: r)
            h = Detail::mix_hashes(h, t_hash(x));
        return h;
    }

    inline uint32_t bernstein_hash(const void* ptr, size_t len) noexcept {
        auto bptr = static_cast<const uint8_t*>(ptr);
        uint32_t h = 5381;
        for (size_t i = 0; i < len; ++i)
            h = 33 * h + bptr[i];
        return h;
    }

    // SipHash-2-4-64 by Jean-Philippe Aumasson and Daniel J. Bernstein
    // https://github.com/veorq/SipHash

    class SipHash {

    public:

        using result_type = uint64_t;

        SipHash() noexcept {}
        SipHash(uint64_t key0, uint64_t key1) noexcept: key0_(key0), key1_(key1) {}

        uint64_t operator()(const void* ptr, size_t len) const noexcept {

            uint64_t v0 = 0x736f'6d65'7073'6575ull;
            uint64_t v1 = 0x646f'7261'6e64'6f6dull;
            uint64_t v2 = 0x6c79'6765'6e65'7261ull;
            uint64_t v3 = 0x7465'6462'7974'6573ull;

            auto bptr = static_cast<const uint8_t*>(ptr);
            auto end = bptr + (len - len % sizeof(uint64_t));
            int left = int(len & 7);
            uint64_t b = uint64_t(len) << 56;
            uint64_t m = 0;

            v0 ^= key0_;
            v1 ^= key1_;
            v2 ^= key0_;
            v3 ^= key1_;

            for (; bptr != end; bptr += 8) {
                std::memcpy(&m, bptr, 8);
                v3 ^= m;
                siprounds(2, v0, v1, v2, v3);
                v0 ^= m;
            }

            for (int i = left - 1; i >= 0; --i)
                b |= uint64_t(bptr[i]) << 8 * i;

            v3 ^= b;
            siprounds(2, v0, v1, v2, v3);
            v0 ^= b;
            v2 ^= 0xff;
            siprounds(4, v0, v1, v2, v3);

            return v0 ^ v1 ^ v2 ^ v3;

        }

    private:

        uint64_t key0_ = 0;
        uint64_t key1_ = 0;

        static void siprounds(int n, uint64_t& v0, uint64_t& v1, uint64_t& v2, uint64_t& v3) noexcept {
            for (int i = 0; i < n; ++i) {
                v0 += v1;
                v1 = Detail::rotl(v1, 13);
                v1 ^= v0;
                v0 = Detail::rotl(v0, 32);
                v2 += v3;
                v3 = Detail::rotl(v3, 16);
                v3 ^= v2;
                v0 += v3;
                v3 = Detail::rotl(v3, 21);
                v3 ^= v0;
                v2 += v1;
                v1 = Detail::rotl(v1, 17);
                v1 ^= v2;
                v2 = Detail::rotl(v2, 32);
            }
        }

    };

    // Good LCG transformations for 32 and 64 bit integers
    // Pierre L'Ecuyer (1999), "Tables of Linear Congruential Generators of Different Sizes and Good Lattice Structure"
    // http://www.ams.org/journals/mcom/1999-68-225/S0025-5718-99-00996-5/S0025-5718-99-00996-5.pdf

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

    class Pcg64 {

    public:

        using result_type = uint64_t;

        constexpr Pcg64() noexcept: state_(default_seed_) { init(); }
        constexpr explicit Pcg64(uint64_t s) noexcept: state_(s) { init(); }
        constexpr explicit Pcg64(uint64_t hi, uint64_t lo) noexcept: state_{hi, lo} { init(); }

        constexpr uint64_t operator()() noexcept {
            state_ *= a_;
            state_ += b_;
            auto x = uint64_t((state_ >> 64) ^ state_);
            auto y = int(state_ >> 122) & 63;
            return Detail::rotr(x, y);
        }

        constexpr void advance(int64_t offset) noexcept {
            state_type u = uint64_t(offset);
            if (offset < 0)
                u |= ~ TL::Uint128(0) << 64;
            state_type add;
            state_type mul = 1;
            auto c = a_;
            auto d = b_;
            while (u) {
                if (u & 1) {
                    mul *= c;
                    add *= c;
                    add += d;
                }
                auto c1 = c;
                ++c1;
                d *= c1;
                c *= c;
                u >>= 1;
            }
            state_ *= mul;
            state_ += add;
        }

        constexpr void seed(uint64_t s) noexcept { state_ = {0, s}; init(); }
        constexpr void seed(uint64_t hi, uint64_t lo) noexcept { state_ = {hi, lo}; init(); }

        static constexpr uint64_t min() noexcept { return 0; }
        static constexpr uint64_t max() noexcept { return ~ uint64_t(0); }

    private:

        using state_type = TL::Uint128;

        static constexpr uint64_t default_seed_ = 0xcafe'f00d'd15e'a5e5ull;
        static constexpr state_type a_ = {0x2360'ed05'1fc6'5da4ull, 0x4385'df64'9fcc'f645ull};
        static constexpr state_type b_ = {0x5851'f42d'4c95'7f2dull, 0x1405'7b7e'f767'814full};

        state_type state_;

        constexpr void init() noexcept {
            state_ += b_;
            state_ *= a_;
            state_ += b_;
        }

    };

    // Xoshiro256** generator by David Blackman and Sebastiano Vigna
    // http://xoshiro.di.unimi.it/

    namespace Detail {

        class SplitMix64 {
        public:
            using result_type = uint64_t;
            constexpr SplitMix64() noexcept: state_(0) {}
            constexpr explicit SplitMix64(uint64_t s) noexcept: state_ (s) {}
            constexpr uint64_t operator()() noexcept {
                constexpr uint64_t a = 0x9e37'79b9'7f4a'7c15ull;
                constexpr uint64_t b = 0xbf58'476d'1ce4'e5b9ull;
                constexpr uint64_t c = 0x94d0'49bb'1331'11ebull;
                state_ += a;
                uint64_t x = (state_ ^ (state_ >> 30)) * b;
                x = (x ^ (x >> 27)) * c;
                return x ^ (x >> 31);
            }
            constexpr void seed(uint64_t s = 0) noexcept { state_ = s; }
            static constexpr uint64_t min() noexcept { return 0; }
            static constexpr uint64_t max() noexcept { return ~ uint64_t(0); }
        private:
            uint64_t state_;
        };

    }

    class Xoshiro {

    public:

        using result_type = uint64_t;

        constexpr Xoshiro() noexcept: state_() { seed(0); }
        constexpr explicit Xoshiro(uint64_t s) noexcept: state_() { seed(s); }
        constexpr Xoshiro(uint64_t s, uint64_t t) noexcept: state_() { seed(s, t); }
        constexpr Xoshiro(uint64_t s, uint64_t t, uint64_t u, uint64_t v) noexcept: state_() { seed(s, t, u, v); }

        constexpr uint64_t operator()() noexcept {
            uint64_t x = Detail::rotl(state_[1] * 5, 7) * 9;
            uint64_t y = state_[1] << 17;
            state_[2] ^= state_[0];
            state_[3] ^= state_[1];
            state_[1] ^= state_[2];
            state_[0] ^= state_[3];
            state_[2] ^= y;
            state_[3] = Detail::rotl(state_[3], 45);
            return x;
        }

        constexpr void seed(uint64_t s = 0) noexcept {
            Detail::SplitMix64 sm(s);
            state_[0] = sm();
            state_[1] = sm();
            state_[2] = sm();
            state_[3] = sm();
        }

        constexpr void seed(uint64_t s, uint64_t t) noexcept {
            Detail::SplitMix64 sm(s);
            state_[0] = sm();
            state_[1] = sm();
            sm.seed(t);
            state_[2] = sm();
            state_[3] = sm();
        }

        constexpr void seed(uint64_t s, uint64_t t, uint64_t u, uint64_t v) noexcept {
            state_[0] = s;
            state_[1] = t;
            state_[2] = u;
            state_[3] = v;
        }

        static constexpr uint64_t min() noexcept { return 0; }
        static constexpr uint64_t max() noexcept { return ~ uint64_t(0); }

    private:

        std::array<uint64_t, 4> state_;

    };

    // Replacements for standard distributions
    // (to ensure reproducibility)

    template <typename T>
    class UniformInteger {

    public:

        static_assert(std::is_integral_v<T>);

        using result_type = T;

        constexpr UniformInteger() noexcept {} // Defaults to 0-1
        constexpr explicit UniformInteger(T range) noexcept: min_(0), max_(range - 1) {} // 0 to range-1; UB if range<1
        constexpr UniformInteger(T a, T b) noexcept: min_(a), max_(b) {} // a to b inclusive; UB if a>b

        template <typename RNG>
        constexpr T operator()(RNG& rng) const noexcept {
            return generate(rng, min(), max());
        }

        constexpr T min() const noexcept { return min_; }
        constexpr T max() const noexcept { return max_; }

    private:

        T min_ = 0;
        T max_ = 1;

        template <typename RNG>
        constexpr static T generate(RNG& rng, T min, T max) noexcept {

            // We need an unsigned integer type big enough for both the RNG
            // and the output range.

            using output_range_type = std::make_unsigned_t<T>;
            using rng_result_type = typename RNG::result_type;
            using working_type = std::common_type_t<output_range_type, rng_result_type, unsigned>;

            if (min == max)
                return min;

            // Compare the input range (max-min of the values generated by the
            // RNG) with the output range (max-min of the possible results).

            auto rng_min = working_type(rng.min());
            auto rng_range = working_type(rng.max()) - rng_min;
            auto out_range = working_type(output_range_type(max) - output_range_type(min));
            working_type result;

            if (out_range < rng_range) {

                // The RNG range is larger than the output range. Divide the
                // output of the RNG by the rounded down quotient of the
                // ranges. If one range is not an exact multiple of the
                // other, this may yield a value too large; try again.

                working_type ratio = (rng_range - out_range) / (out_range + 1) + 1;
                working_type limit = ratio * (out_range + 1) - 1;
                do result = working_type(rng() - rng_min);
                    while (result > limit);
                result /= ratio;

            } else if (out_range == rng_range) {

                // The trivial case where the two ranges are equal.

                result = working_type(rng() - rng_min);

            } else {

                // The output range is larger than the RNG range. Split the
                // output range into a quotient and remainder modulo the RNG
                // range +1; call this function recursively for the quotient,
                // then call the RNG directly for the remainder. Try again if
                // the result is too large.

                working_type high = 0, low = 0;
                working_type ratio = (out_range - rng_range) / (rng_range + 1);
                do {
                    high = generate(rng, working_type(0), ratio) * (rng_range + 1);
                    low = working_type(rng() - rng_min);
                } while (low > out_range - high);
                result = high + low;

            }

            return min + T(result);

        }

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
            constexpr int den = std::numeric_limits<int>::max();
            int num = int(std::lround(p * den));
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

    template <typename T>
    class DiscreteNormal {

    public:

        static_assert(std::is_integral_v<T>);

        using result_type = T;

        DiscreteNormal() = default; // Defaults to (0,1)
        DiscreteNormal(double mean, double sd) noexcept: norm_(mean, sd) {}

        template <typename RNG>
        T operator()(RNG& rng) const noexcept {
            return T(std::lround(norm_(rng)));
        }

        double mean() const noexcept { return norm_.mean(); }
        double sd() const noexcept { return norm_.sd(); }

    private:

        NormalDistribution<double> norm_;

    };

    template <typename T, typename U = double>
    class PoissonDistribution {

    public:

        static_assert(std::is_integral_v<T>);
        static_assert(std::is_floating_point_v<U>);

        using result_type = T;
        using scalar_type = U;

        PoissonDistribution() noexcept {}
        explicit PoissonDistribution(U lambda) noexcept: lambda_(lambda), log_lambda_(std::log(lambda)) {}

        template <typename RNG>
        T operator()(RNG& rng) const noexcept {

            // https://www.johndcook.com/blog/2010/06/14/generating-poisson-random-values/

            T n;
            UniformReal<U> unit;

            if (lambda_ <= 30) {

                // Knuth algorithm

                U xl = std::exp(- lambda_);
                T k = 0;
                U p = 1;

                do {
                    ++k;
                    U u = unit(rng);
                    p *= u;
                } while (p > xl);

                n = k - 1;

            } else {

                // Atkinson algorithm

                U c = U(0.767) - U(3.36) / lambda_;
                U beta = pi_c<U> / std::sqrt(3 * lambda_);
                U alpha = beta * lambda_;
                U k = std::log(c) - lambda_ - std::log(beta);
                U lhs, rhs;

                do {
                    U u = unit(rng);
                    U x = (alpha - std::log((1 - u) / u)) / beta;
                    n = T(std::floor(x + U(0.5)));
                    if (n < 0)
                        continue;
                    U v = unit(rng);
                    U y = alpha - beta * x;
                    U z = 1 + std::exp(y);
                    lhs = y + std::log(v / (z * z));
                    rhs = k + n * log_lambda_ - std::lgamma(U(n) + 1);
                } while (lhs > rhs);

            }

            return n;

        }

        U mean() const noexcept { return lambda_; }
        U sd() const noexcept { return std::sqrt(lambda_); }

    private:

        U lambda_ = 1;
        U log_lambda_ = 0;

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
                throw std::invalid_argument("Constrained distribution has no possible values");
        }

    // Selection from a set of discrete values

    template <typename T>
    class RandomChoice {

    public:

        using result_type = T;

        RandomChoice() = default;
        RandomChoice(std::initializer_list<T> list): vec_(list) {}

        template <typename Range>
        explicit RandomChoice(const Range& range) {
            using std::begin;
            using std::end;
            vec_.assign(begin(range), end(range));
        }

        template <typename RNG>
        const T& operator()(RNG& rng) const {
            UniformInteger<size_t> dist(vec_.size());
            size_t index = dist(rng);
            return vec_[index];
        }

        template <typename... Args>
        RandomChoice& add(const T& t, const Args&... args) {
            vec_.push_back(t);
            if constexpr (sizeof...(Args) > 0)
                add(args...);
            return *this;
        }

        bool empty() const noexcept { return vec_.empty(); }
        size_t size() const noexcept { return vec_.size(); }

    private:

        std::vector<T> vec_;

    };

    template <typename Range>
    RandomChoice<TL::RangeValue<Range>> random_choice(const Range& range) {
        return RandomChoice<TL::RangeValue<Range>>(range);
    }

    // Selection from a weighted set of discrete values

    template <typename T>
    class WeightedChoice {

    private:

        struct group {
            double weight = 0;
            std::vector<T> values;
            group() = default;
            template <typename... Args> group(double w, const T& t, const Args&... args): weight(w), values{t, args...} {}
        };

    public:

        using result_type = T;

        WeightedChoice() = default;

        WeightedChoice(std::initializer_list<group> list) {
            for (auto& g: list)
                add_group(g);
        }

        template <typename RNG>
        const T& operator()(RNG& rng) const {
            double x = dist_(rng);
            auto it = table_.upper_bound(x);
            return it->second;
        }

        template <typename... Args>
        WeightedChoice& add(double w, const T& t, const Args&... args) {
            group g(w, t, args...);
            add_group(g);
            return *this;
        }

        bool empty() const noexcept { return table_.empty(); }
        double total_weight() const noexcept { return dist_.max(); }

    private:

        std::map<double, T> table_; // Cumulative => value
        UniformReal<double> dist_{0};

        void add_group(const group& g) {
            if (g.weight <= 0 || g.values.empty())
                return;
            T last = g.values.back();
            auto most = TL::irange(g.values.begin(), g.values.end() - 1);
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

    };

    // Random Uuid

    class RandomUuid {
    public:

        using result_type = TL::Uuid;

        template <typename RNG>
        TL::Uuid operator()(RNG& rng) const {
            std::array<uint64_t, 2> array;
            UniformInteger<uint64_t> make64(0, ~uint64_t(0));
            for (auto& a: array)
                a = make64(rng);
            TL::Uuid u(array.data(), 16);
            u[6] = (u[6] & 0x0f) | 0x40;
            u[8] = (u[8] & 0x3f) | 0x80;
            return u;
        }

    };

    // Spatial distributions

    template <typename T, int N>
    class RandomVector {

    public:

        static_assert(std::is_floating_point_v<T>);
        static_assert(N > 0);

        using result_type = Graphics::Core::Vector<T, N>;
        using scalar_type = T;

        static constexpr int dim = N;

        constexpr RandomVector() noexcept: vec_(T(1)) {}
        constexpr explicit RandomVector(T t) noexcept: vec_(t) {}
        constexpr explicit RandomVector(const result_type& v) noexcept: vec_(v) {}

        template <typename RNG>
        result_type operator()(RNG& rng) const {
            result_type v;
            for (int i = 0; i < N; ++i)
                v[i] = UniformReal<T>(vec_[i])(rng);
            return v;
        }

        constexpr result_type apex() const noexcept { return vec_; }

    private:

        result_type vec_;

    };

    template <typename T, int N>
    class SymmetricRandomVector {

    public:

        static_assert(std::is_floating_point_v<T>);
        static_assert(N > 0);

        using result_type = Graphics::Core::Vector<T, N>;
        using scalar_type = T;

        static constexpr int dim = N;

        constexpr SymmetricRandomVector() noexcept: vec_(T(1)) {}
        constexpr explicit SymmetricRandomVector(T t) noexcept: vec_(t) {}
        constexpr explicit SymmetricRandomVector(const result_type& v) noexcept: vec_(v) {}

        template <typename RNG>
        result_type operator()(RNG& rng) const {
            result_type v;
            for (int i = 0; i < N; ++i)
                v[i] = UniformReal<T>(- vec_[i], vec_[i])(rng);
            return v;
        }

        constexpr result_type apex() const noexcept { return vec_; }

    private:

        result_type vec_;

    };

    template <typename T, int N>
    class RandomPointInSphere {

    public:

        static_assert(std::is_floating_point_v<T>);
        static_assert(N > 0);

        using result_type = Graphics::Core::Vector<T, N>;
        using scalar_type = T;

        static constexpr int dim = N;

        constexpr RandomPointInSphere() noexcept: radius_(1) {}
        constexpr explicit RandomPointInSphere(T r) noexcept: radius_(std::fabs(r)) {}

        template <typename RNG>
        result_type operator()(RNG& rng) const {

            result_type v;

            if constexpr (N == 1) {

                v[0] = UniformReal<T>(- radius_, radius_)(rng);

            } else if constexpr (N <= 4) {

                do {
                    for (auto& x: v)
                        x = UniformReal<T>(-1, 1)(rng);
                } while (v.r2() > 1);

                v *= radius_;

            } else {

                for (int i = 0; i < N; i += 2) {
                    T a = std::sqrt(-2 * std::log(UniformReal<T>()(rng)));
                    T b = 2 * pi_c<T> * UniformReal<T>()(rng);
                    v[i] = a * std::cos(b);
                    if (i + 1 < N)
                        v[i + 1] = a * std::sin(b);
                }

                v *= radius_ * std::pow(UniformReal<T>()(rng), T(1) / T(N)) / v.r();

            }

            return v;

        }

        constexpr T radius() const noexcept { return radius_; }

    private:

        T radius_;

    };

    template <typename T, int N>
    class RandomDirection {

    public:

        static_assert(std::is_floating_point_v<T>);
        static_assert(N > 0);

        using result_type = Graphics::Core::Vector<T, N>;
        using scalar_type = T;

        static constexpr int dim = N;

        constexpr RandomDirection() noexcept {}

        template <typename RNG>
        result_type operator()(RNG& rng) const {

            result_type v;

            if constexpr (N == 1) {

                v[0] = BernoulliDistribution()(rng) ? 1 : -1;

            } else if constexpr (N == 2) {

                T phi = UniformReal<T>(0, 2 * pi_c<T>)(rng);
                v[0] = std::cos(phi);
                v[1] = std::sin(phi);

            } else if constexpr (N == 3) {

                T phi = UniformReal<T>(0, 2 * pi_c<T>)(rng);
                v[2] = UniformReal<T>(-1, 1)(rng);
                T r = std::sqrt(1 - v[2] * v[2]);
                v[0] = r * std::cos(phi);
                v[1] = r * std::sin(phi);

            } else {

                for (int i = 0; i < N; i += 2) {
                    T a = std::sqrt(-2 * std::log(UniformReal<T>()(rng)));
                    T b = 2 * pi_c<T> * UniformReal<T>()(rng);
                    v[i] = a * std::cos(b);
                    if (i + 1 < N)
                        v[i + 1] = a * std::sin(b);
                }

                v /= v.r();

            }

            return v;

        }

    };

}
