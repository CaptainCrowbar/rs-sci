#pragma once

#include "rs-tl/binary.hpp"
#include <functional>
#include <iterator>
#include <string>
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

    // Cryptographic hash functions

    #define RS_DEFINE_CRYPTOGRAPHIC_HASH_CLASS(ClassName, BitSize) \
        class ClassName { \
        public: \
            static_assert(BitSize % 8 == 0); \
            using result_type = std::string; \
            static constexpr size_t bits = BitSize; \
            static constexpr size_t bytes = BitSize / 8; \
            ClassName(): hash_(bytes, '\0'), impl_(nullptr) {} \
            ~ClassName() noexcept { done(); } \
            ClassName(const ClassName&) = delete; \
            ClassName(ClassName&&) = delete; \
            ClassName& operator=(const ClassName&) = delete; \
            ClassName& operator=(ClassName&&) = delete; \
            std::string operator()(const void* ptr, size_t len) { clear(); add(ptr, len); return get(); } \
            std::string operator()(const std::string& str) { clear(); add(str); return get(); } \
            void add(const void* ptr, size_t len); \
            void add(const std::string& str) { add(str.data(), str.size()); } \
            void clear() noexcept { done(); hash_.assign(bytes, '\0'); } \
            std::string get() { done(); return hash_; } \
        private: \
            struct impl_type; \
            std::string hash_ = {}; \
            impl_type* impl_ = nullptr; \
            auto data() noexcept { return reinterpret_cast<unsigned char*>(hash_.data()); } \
            void done() noexcept; \
        };

    RS_DEFINE_CRYPTOGRAPHIC_HASH_CLASS(Md5, 128)
    RS_DEFINE_CRYPTOGRAPHIC_HASH_CLASS(Sha1, 160)
    RS_DEFINE_CRYPTOGRAPHIC_HASH_CLASS(Sha256, 256)
    RS_DEFINE_CRYPTOGRAPHIC_HASH_CLASS(Sha512, 512)

    #undef RS_DEFINE_CRYPTOGRAPHIC_HASH_CLASS

}
