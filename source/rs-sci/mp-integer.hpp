#pragma once

#include "rs-sci/random.hpp"
#include "rs-sci/rational.hpp"
#include "rs-format/format.hpp"
#include "rs-tl/binary.hpp"
#include "rs-tl/types.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace RS::Sci {

    // Unsigned integer class

    class Mpuint:
    public TL::TotalOrder<Mpuint> {
    public:

        Mpuint() = default;
        Mpuint(uint64_t x);
        explicit Mpuint(std::string_view s, int base = 0) { init(s, base); }

        template <typename T> explicit operator T() const;
        explicit operator bool() const noexcept { return ! rep_.empty(); }

        bool operator!() const noexcept { return ! bool(*this); }
        Mpuint operator+() const { return *this; }
        Mpuint& operator++() { return *this += 1; }
        Mpuint operator++(int) { auto x = *this; ++*this; return x; }
        Mpuint& operator--() { return *this -= 1; }
        Mpuint operator--(int) { auto x = *this; --*this; return x; }
        Mpuint& operator+=(const Mpuint& rhs);
        Mpuint& operator-=(const Mpuint& rhs);
        Mpuint& operator*=(const Mpuint& rhs) { Mpuint z; do_multiply(*this, rhs, z); rep_.swap(z.rep_); return *this; }
        Mpuint& operator/=(const Mpuint& rhs) { Mpuint q, r; do_divide(*this, rhs, q, r); rep_.swap(q.rep_); return *this; }
        Mpuint& operator%=(const Mpuint& rhs) { Mpuint q, r; do_divide(*this, rhs, q, r); rep_.swap(r.rep_); return *this; }
        Mpuint& operator&=(const Mpuint& rhs);
        Mpuint& operator|=(const Mpuint& rhs);
        Mpuint& operator^=(const Mpuint& rhs);
        Mpuint& operator<<=(ptrdiff_t rhs);
        Mpuint& operator>>=(ptrdiff_t rhs);

        size_t bits() const noexcept;
        size_t bits_set() const noexcept;
        size_t bytes() const noexcept;
        int compare(const Mpuint& rhs) const noexcept;
        bool get_bit(size_t i) const noexcept;
        uint8_t get_byte(size_t i) const noexcept;
        void set_bit(size_t i, bool b = true);
        void set_byte(size_t i, uint8_t b);
        void flip_bit(size_t i);
        size_t hash() const noexcept { return hash_mix(rep_); }
        bool is_even() const noexcept { return rep_.empty() || (rep_.front() & 1) == 0; }
        bool is_odd() const noexcept { return ! is_even(); }
        Mpuint pow(const Mpuint& n) const;
        int sign() const noexcept { return int(bool(*this)); }
        std::string str(int base = 10, size_t digits = 1) const;
        void write_be(void* ptr, size_t n) const noexcept;
        void write_le(void* ptr, size_t n) const noexcept;

        static Mpuint from_double(double x);
        static Mpuint read_be(const void* ptr, size_t n);
        static Mpuint read_le(const void* ptr, size_t n);

        friend Mpuint operator*(const Mpuint& lhs, const Mpuint& rhs) { Mpuint z; Mpuint::do_multiply(lhs, rhs, z); return z; }
        friend Mpuint operator/(const Mpuint& lhs, const Mpuint& rhs) { Mpuint q, r; Mpuint::do_divide(lhs, rhs, q, r); return q; }
        friend Mpuint operator%(const Mpuint& lhs, const Mpuint& rhs) { Mpuint q, r; Mpuint::do_divide(lhs, rhs, q, r); return r; }

        friend std::pair<Mpuint, Mpuint> divide(const Mpuint& lhs, const Mpuint& rhs) { Mpuint q, r; Mpuint::do_divide(lhs, rhs, q, r); return {q, r}; }

    private:

        friend class Mpint;

        static constexpr auto mask32 = ~ uint32_t(0);

        std::vector<uint32_t> rep_; // Least significant word first

        void init(std::string_view s, int base);
        void trim() noexcept;

        static void do_divide(const Mpuint& x, const Mpuint& y, Mpuint& q, Mpuint& r);
        static void do_multiply(const Mpuint& x, const Mpuint& y, Mpuint& z);

    };

        inline Mpuint operator+(const Mpuint& lhs, const Mpuint& rhs) { auto z = lhs; z += rhs; return z; }
        inline Mpuint operator-(const Mpuint& lhs, const Mpuint& rhs) { auto z = lhs; z -= rhs; return z; }
        inline Mpuint operator&(const Mpuint& lhs, const Mpuint& rhs) { auto z = lhs; z &= rhs; return z; }
        inline Mpuint operator|(const Mpuint& lhs, const Mpuint& rhs) { auto z = lhs; z |= rhs; return z; }
        inline Mpuint operator^(const Mpuint& lhs, const Mpuint& rhs) { auto z = lhs; z ^= rhs; return z; }
        inline Mpuint operator<<(const Mpuint& lhs, size_t rhs) { auto z = lhs; z <<= rhs; return z; }
        inline Mpuint operator>>(const Mpuint& lhs, size_t rhs) { auto z = lhs; z >>= rhs; return z; }
        inline bool operator==(const Mpuint& lhs, const Mpuint& rhs) noexcept { return lhs.compare(rhs) == 0; }
        inline bool operator<(const Mpuint& lhs, const Mpuint& rhs) noexcept { return lhs.compare(rhs) == -1; }
        inline std::string to_string(const Mpuint& x) { return x.str(); }
        inline std::ostream& operator<<(std::ostream& out, const Mpuint& x) { return out << x.str(); }

        template <typename T>
        Mpuint::operator T() const {
            T t = 0;
            int bit = 0;
            for (auto w: rep_) {
                if constexpr (std::is_floating_point_v<T>) {
                    t += std::ldexp(T(w), bit);
                } else {
                    if (bit >= int(8 * sizeof(T)))
                        break;
                    t += T(w) << bit;
                }
                bit += 32;
            }
            return t;
        }

    // Signed integer class

    class Mpint:
    public TL::TotalOrder<Mpint> {

    public:

        Mpint() = default;
        Mpint(int64_t x): mag_(uint64_t(std::abs(x))), neg_(x < 0) {}
        Mpint(const Mpuint& x): mag_(x), neg_(false) {}
        explicit Mpint(std::string_view s, int base = 0) { init(s, base); }

        template <typename T> explicit operator T() const;
        explicit operator Mpuint() const { return mag_; }
        explicit operator bool() const noexcept { return bool(mag_); }

        bool operator!() const noexcept { return ! bool(*this); }
        Mpint operator+() const { return *this; }
        Mpint operator-() const { Mpint z = *this; if (z) z.neg_ = ! z.neg_; return z; }
        Mpint& operator++() { return *this += 1; }
        Mpint operator++(int) { auto x = *this; ++*this; return x; }
        Mpint& operator--() { return *this -= 1; }
        Mpint operator--(int) { auto x = *this; --*this; return x; }
        Mpint& operator+=(const Mpint& rhs);
        Mpint& operator-=(const Mpint& rhs) { return *this += - rhs; }
        Mpint& operator*=(const Mpint& rhs) { Mpint z; do_multiply(*this, rhs, z); std::swap(*this, z); return *this; }
        Mpint& operator/=(const Mpint& rhs) { Mpint q, r; do_divide(*this, rhs, q, r); std::swap(*this, q); return *this; }
        Mpint& operator%=(const Mpint& rhs) { Mpint q, r; do_divide(*this, rhs, q, r); std::swap(*this, r); return *this; }

        Mpuint abs() const { return mag_; }
        int compare(const Mpint& rhs) const noexcept;
        size_t hash() const noexcept { return hash_mix(mag_, neg_); }
        bool is_even() const noexcept { return mag_.is_even(); }
        bool is_odd() const noexcept { return mag_.is_odd(); }
        Mpint pow(const Mpint& n) const;
        int sign() const noexcept { return neg_ ? -1 : mag_.sign(); }
        std::string str(int base = 10, size_t digits = 1, bool sign = false) const;

        static Mpint from_double(double x);

        friend Mpint operator*(const Mpint& lhs, const Mpint& rhs) { Mpint z; Mpint::do_multiply(lhs, rhs, z); return z; }
        friend Mpint operator/(const Mpint& lhs, const Mpint& rhs) { Mpint q, r; Mpint::do_divide(lhs, rhs, q, r); return q; }
        friend Mpint operator%(const Mpint& lhs, const Mpint& rhs) { Mpint q, r; Mpint::do_divide(lhs, rhs, q, r); return r; }
        friend std::pair<Mpint, Mpint> divide(const Mpint& lhs, const Mpint& rhs) { Mpint q, r; Mpint::do_divide(lhs, rhs, q, r); return {q, r}; }

    private:

        Mpuint mag_;
        bool neg_ = false;

        void init(std::string_view s, int base);

        static void do_divide(const Mpint& x, const Mpint& y, Mpint& q, Mpint& r);
        static void do_multiply(const Mpint& x, const Mpint& y, Mpint& z);

    };

        using Mpratio = Ratio<Mpint>;

        inline Mpint operator+(const Mpint& lhs, const Mpint& rhs) { auto z = lhs; z += rhs; return z; }
        inline Mpint operator-(const Mpint& lhs, const Mpint& rhs) { auto z = lhs; z -= rhs; return z; }
        inline bool operator==(const Mpint& lhs, const Mpint& rhs) noexcept { return lhs.compare(rhs) == 0; }
        inline bool operator<(const Mpint& lhs, const Mpint& rhs) noexcept { return lhs.compare(rhs) == -1; }
        inline std::string to_string(const Mpint& x) { return x.str(); }
        inline std::ostream& operator<<(std::ostream& out, const Mpint& x) { return out << x.str(); }

        template <typename T>
        Mpint::operator T() const {
            auto t = T(mag_);
            if (neg_)
                t = T(0) - t;
            return t;
        }

    // Literals

    namespace Literals {

        inline Mpint operator""_mpi(const char* raw) { return Mpint(raw); }
        inline Mpuint operator""_mpu(const char* raw) { return Mpuint(raw); }

    }

}

namespace std {

    template <>
    class hash<RS::Sci::Mpint> {
    public:
        size_t operator()(const RS::Sci::Mpint& x) const noexcept {
            return x.hash();
        }
    };

    template <>
    class hash<RS::Sci::Mpuint> {
    public:
        size_t operator()(const RS::Sci::Mpuint& x) const noexcept {
            return x.hash();
        }
    };

}
