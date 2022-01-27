// This header is not part of the public interface;
// include "rs-sci/random.hpp" instead.

#pragma once

#include "rs-tl/binary.hpp"
#include "rs-tl/fixed-binary.hpp"
#include <array>
#include <functional>
#include <iterator>
#include <type_traits>

namespace RS::Sci {

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

        constexpr SipHash() noexcept {}
        constexpr SipHash(uint64_t key0, uint64_t key1) noexcept: key0_(key0), key1_(key1) {}

        constexpr uint64_t operator()(const void* ptr, size_t len) const noexcept {

            uint64_t v0 = 0x736f'6d65'7073'6575ull;
            uint64_t v1 = 0x646f'7261'6e64'6f6dull;
            uint64_t v2 = 0x6c79'6765'6e65'7261ull;
            uint64_t v3 = 0x7465'6462'7974'6573ull;

            v0 ^= key0_;
            v1 ^= key1_;
            v2 ^= key0_;
            v3 ^= key1_;

            auto bptr = static_cast<const uint8_t*>(ptr);
            auto end = bptr + (len - len % sizeof(uint64_t));

            while (bptr != end) {
                uint64_t m = 0;
                for (int i = 0; i < 64; i += 8)
                    m |= uint64_t(*bptr++) << i;
                v3 ^= m;
                siprounds(2, v0, v1, v2, v3);
                v0 ^= m;
            }

            int left = int(len & 7);
            uint64_t b = uint64_t(len) << 56;

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

        static constexpr void siprounds(int n, uint64_t& v0, uint64_t& v1, uint64_t& v2, uint64_t& v3) noexcept {
            for (int i = 0; i < n; ++i) {
                v0 += v1;
                v1 = TL::rotl(v1, 13);
                v1 ^= v0;
                v0 = TL::rotl(v0, 32);
                v2 += v3;
                v3 = TL::rotl(v3, 16);
                v3 ^= v2;
                v0 += v3;
                v3 = TL::rotl(v3, 21);
                v3 ^= v0;
                v2 += v1;
                v1 = TL::rotl(v1, 17);
                v1 ^= v2;
                v2 = TL::rotl(v2, 32);
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
            return TL::rotr(x, y);
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
            uint64_t x = TL::rotl(state_[1] * 5, 7) * 9;
            uint64_t y = state_[1] << 17;
            state_[2] ^= state_[0];
            state_[3] ^= state_[1];
            state_[1] ^= state_[2];
            state_[0] ^= state_[3];
            state_[2] ^= y;
            state_[3] = TL::rotl(state_[3], 45);
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

    // Default choice of RNG

    using StdRng = Pcg64;

}
