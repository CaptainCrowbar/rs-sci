#pragma once

#include "rs-sci/random.hpp"
#include "rs-sci/rational.hpp"
#include "rs-format/format.hpp"
#include "rs-tl/binary.hpp"
#include "rs-tl/types.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace RS::Sci {

    // Unsigned integers

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

        inline Mpuint::Mpuint(uint64_t x) {
            if (x > 0)
                rep_.push_back(uint32_t(x));
            if (x > mask32)
                rep_.push_back(uint32_t(x >> 32));
        }

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

        inline Mpuint& Mpuint::operator+=(const Mpuint& rhs) {
            size_t common = std::min(rep_.size(), rhs.rep_.size());
            rep_.resize(std::max(rep_.size(), rhs.rep_.size()), 0);
            uint64_t sum = 0;
            for (size_t i = 0; i < common; ++i) {
                sum += uint64_t(rep_[i]) + uint64_t(rhs.rep_[i]);
                rep_[i] = uint32_t(sum);
                sum >>= 32;
            }
            const auto* rptr = &rep_;
            if (rhs.rep_.size() > common)
                rptr = &rhs.rep_;
            for (size_t i = common; i < rep_.size(); ++i) {
                sum += uint64_t((*rptr)[i]);
                rep_[i] = uint32_t(sum);
                sum >>= 32;
            }
            if (sum)
                rep_.push_back(uint32_t(sum));
            trim();
            return *this;
        }

        inline Mpuint& Mpuint::operator-=(const Mpuint& rhs) {
            size_t common = std::min(rep_.size(), rhs.rep_.size());
            bool carry = false;
            for (size_t i = 0; i < common; ++i) {
                bool c = rep_[i] < rhs.rep_[i] || (carry && rep_[i] == rhs.rep_[i]);
                rep_[i] -= rhs.rep_[i];
                if (carry)
                    --rep_[i];
                carry = c;
            }
            for (size_t i = common; carry && i < rep_.size(); ++i)
                carry = --rep_[i] == mask32;
            trim();
            return *this;
        }

        inline Mpuint& Mpuint::operator&=(const Mpuint& rhs) {
            size_t common = std::min(rep_.size(), rhs.rep_.size());
            rep_.resize(common);
            for (size_t i = 0; i < common; ++i)
                rep_[i] &= rhs.rep_[i];
            trim();
            return *this;
        }

        inline Mpuint& Mpuint::operator|=(const Mpuint& rhs) {
            size_t common = std::min(rep_.size(), rhs.rep_.size());
            rep_.resize(std::max(rep_.size(), rhs.rep_.size()));
            for (size_t i = 0; i < common; ++i)
                rep_[i] |= rhs.rep_[i];
            return *this;
        }

        inline Mpuint& Mpuint::operator^=(const Mpuint& rhs) {
            size_t common = std::min(rep_.size(), rhs.rep_.size());
            rep_.resize(std::max(rep_.size(), rhs.rep_.size()));
            for (size_t i = 0; i < common; ++i)
                rep_[i] ^= rhs.rep_[i];
            trim();
            return *this;
        }

        inline Mpuint& Mpuint::operator<<=(ptrdiff_t rhs) {
            if (rhs == 0)
                return *this;
            if (rhs < 0)
                return *this >>= - rhs;
            size_t words = rhs / 32;
            int bits = rhs % 32;
            uint32_t prev = 0;
            if (bits > 0) {
                for (auto& word: rep_) {
                    uint32_t next = word >> (32 - bits);
                    word = (word << bits) | prev;
                    prev = next;
                }
            }
            if (prev)
                rep_.push_back(prev);
            rep_.insert(rep_.begin(), words, 0);
            return *this;
        }

        inline Mpuint& Mpuint::operator>>=(ptrdiff_t rhs) {
            if (rhs == 0)
                return *this;
            if (rhs < 0)
                return *this <<= - rhs;
            size_t words = rhs / 32;
            int bits = rhs % 32;
            if (words >= rep_.size()) {
                rep_.clear();
            } else {
                rep_.erase(rep_.begin(), rep_.begin() + words);
                if (bits > 0) {
                    uint32_t prev = 0;
                    for (size_t i = rep_.size() - 1; i != std::string::npos; --i) {
                        uint32_t next = rep_[i] << (32 - bits);
                        rep_[i] = (rep_[i] >> bits) | prev;
                        prev = next;
                    }
                }
            }
            trim();
            return *this;
        }

        inline size_t Mpuint::bits() const noexcept {
            size_t n = 32 * rep_.size();
            if (! rep_.empty())
                n -= 32 - TL::bit_width(rep_.back());
            return n;
        }

        inline size_t Mpuint::bits_set() const noexcept {
            size_t n = 0;
            for (auto i: rep_)
                n += TL::popcount(i);
            return n;
        }

        inline size_t Mpuint::bytes() const noexcept {
            if (rep_.empty())
                return 0;
            else
                return 4 * (rep_.size() - 1) + size_t(rep_.back() > 0) + size_t(rep_.back() > size_t(0xff))
                    + size_t(rep_.back() > size_t(0xffff)) + size_t(rep_.back() > size_t(0xffffff));
        }

        inline int Mpuint::compare(const Mpuint& rhs) const noexcept {
            if (rep_.size() != rhs.rep_.size())
                return rep_.size() < rhs.rep_.size() ? -1 : 1;
            for (size_t i = rep_.size() - 1; i != std::string::npos; --i)
                if (rep_[i] != rhs.rep_[i])
                    return rep_[i] < rhs.rep_[i] ? -1 : 1;
            return 0;
        }

        inline bool Mpuint::get_bit(size_t i) const noexcept {
            if (i < 32 * rep_.size())
                return (rep_[i / 32] >> (i % 32)) & 1;
            else
                return false;
        }

        inline uint8_t Mpuint::get_byte(size_t i) const noexcept {
            if (i < 4 * rep_.size())
                return (rep_[i / 4] >> (i % 4 * 8)) & 0xff;
            else
                return 0;
        }

        inline void Mpuint::set_bit(size_t i, bool b) {
            bool in_rep = i < 32 * rep_.size();
            if (b) {
                if (! in_rep)
                    rep_.resize(i / 32 + 1, 0);
                rep_[i / 32] |= uint32_t(1) << (i % 32);
            } else if (in_rep) {
                rep_[i / 32] &= ~ (uint32_t(1) << (i % 32));
                trim();
            }
        }

        inline void Mpuint::set_byte(size_t i, uint8_t b) {
            if (i >= 4 * rep_.size())
                rep_.resize(i / 4 + 1, 0);
            rep_[i / 4] |= uint32_t(b) << (i % 4 * 8);
            trim();
        }

        inline void Mpuint::flip_bit(size_t i) {
            if (i >= 32 * rep_.size())
                rep_.resize(i / 32 + 1, 0);
            rep_[i / 32] ^= uint32_t(1) << (i % 32);
            trim();
        }

        inline Mpuint Mpuint::pow(const Mpuint& n) const {
            Mpuint x = *this, y = n, z = 1;
            while (y) {
                if (y.is_odd())
                    z *= x;
                x *= x;
                y >>= 1;
            }
            return z;
        }

        inline std::string Mpuint::str(int base, size_t digits) const {
            if (base < 2 || base > 36)
                throw std::invalid_argument("Invalid base: " + std::to_string(base));
            if (rep_.empty())
                return std::string(digits, '0');
            std::string s;
            if (base == 2) {
                s = Format::format_integer(rep_.back(), "b1");
                for (size_t i = rep_.size() - 2; i != std::string::npos; --i)
                    s += Format::format_integer(rep_[i], "b32");
            } else if (base == 16) {
                s = Format::format_integer(rep_.back(), "x1");
                for (size_t i = rep_.size() - 2; i != std::string::npos; --i)
                    s += Format::format_integer(rep_[i], "x8");
            } else if (base <= 10) {
                Mpuint b = base, q, r, t = *this;
                while (t) {
                    do_divide(t, b, q, r);
                    s += char(int(r) + '0');
                    t = std::move(q);
                }
                std::reverse(s.begin(), s.end());
            } else {
                Mpuint b = base, q, r, t = *this;
                while (t) {
                    do_divide(t, b, q, r);
                    int d = int(r);
                    if (d < 10)
                        s += char(d + '0');
                    else
                        s += char(d - 10 + 'a');
                    t = std::move(q);
                }
                std::reverse(s.begin(), s.end());
            }
            if (s.size() < digits)
                s.insert(0, digits - s.size(), '0');
            return s;
        }

        inline void Mpuint::write_be(void* ptr, size_t n) const noexcept {
            size_t nb = std::min(n, bytes());
            std::memset(ptr, 0, n - nb);
            auto bp = static_cast<uint8_t*>(ptr) + n - nb, end = bp + nb;
            while (bp != end)
                *bp++ = get_byte(--nb);
        }

        inline void Mpuint::write_le(void* ptr, size_t n) const noexcept {
            auto bp = static_cast<uint8_t*>(ptr);
            size_t nb = std::min(n, bytes());
            for (size_t i = 0; i < nb; ++i)
                bp[i] = get_byte(i);
            std::memset(bp + nb, 0, n - nb);
        }

        inline Mpuint Mpuint::from_double(double x) {
            int e = 0;
            double m = frexp(fabs(x), &e);
            Mpuint n = uint64_t(floor(ldexp(m, 64)));
            n <<= e - 64;
            return n;
        }

        inline Mpuint Mpuint::read_be(const void* ptr, size_t n) {
            Mpuint result;
            result.rep_.resize((n + 3) / 4);
            auto bp = static_cast<const uint8_t*>(ptr);
            for (size_t i = 0, j = n - 1; i < n; ++i, --j)
                result.set_byte(j, bp[i]);
            result.trim();
            return result;
        }

        inline Mpuint Mpuint::read_le(const void* ptr, size_t n) {
            Mpuint result;
            result.rep_.resize((n + 3) / 4);
            auto bp = static_cast<const uint8_t*>(ptr);
            for (size_t i = 0; i < n; ++i)
                result.set_byte(i, bp[i]);
            result.trim();
            return result;
        }

        inline void Mpuint::do_divide(const Mpuint& x, const Mpuint& y, Mpuint& q, Mpuint& r) {
            Mpuint quo, rem = x;
            if (x >= y) {
                size_t shift = x.bits() - y.bits();
                Mpuint rsub = y;
                rsub <<= shift;
                if (rsub > x) {
                    --shift;
                    rsub >>= 1;
                }
                Mpuint qadd = 1;
                qadd <<= shift;
                while (qadd) {
                    if (rem >= rsub) {
                        rem -= rsub;
                        quo += qadd;
                    }
                    rsub >>= 1;
                    qadd >>= 1;
                }
            }
            q = std::move(quo);
            r = std::move(rem);
        }

        inline void Mpuint::do_multiply(const Mpuint& x, const Mpuint& y, Mpuint& z) {
            if (! x || ! y) {
                z.rep_.clear();
            } else {
                size_t m = x.rep_.size(), n = y.rep_.size();
                z.rep_.assign(m + n, 0);
                uint64_t carry = 0;
                for (size_t k = 0; k <= m + n - 2; ++k) {
                    carry += uint64_t(z.rep_[k]);
                    z.rep_[k] = uint32_t(carry);
                    carry >>= 32;
                    size_t i_min = k < n ? 0 : k - n + 1;
                    size_t i_max = k < m ? k : m - 1;
                    for (size_t i = i_min; i <= i_max; ++i) {
                        uint64_t p = uint64_t(x.rep_[i]) * uint64_t(y.rep_[k - i]) + uint64_t(z.rep_[k]);
                        z.rep_[k] = uint32_t(p);
                        carry += uint32_t(p >> 32);
                    }
                }
                z.rep_[m + n - 1] = uint32_t(carry);
                z.trim();
            }
        }

        inline void Mpuint::init(std::string_view s, int base) {
            if (base < 0 || base == 1 || base > 36)
                throw std::invalid_argument("Invalid base: " + std::to_string(base));
            if (s.empty())
                return;
            auto ptr = s.data(), end = ptr + s.size();
            if (base == 0) {
                if (s[0] != '0' || s.size() < 3)
                    base = 10;
                else if (s[1] == 'B' || s[1] == 'b')
                    base = 2;
                else if (s[1] == 'X' || s[1] == 'x')
                    base = 16;
                else
                    base = 10;
                if (base != 10)
                    ptr += 2;
            }
            Mpuint nbase = base;
            int digit = 0;
            int (*get_digit)(char c);
            if (base <= 10)
                get_digit = [] (char c) noexcept { return c >= '0' && c <= '9' ? int(c - '0') : 64; };
            else
                get_digit = [] (char c) noexcept { return c >= '0' && c <= '9' ? int(c - '0') :
                    c >= 'A' && c <= 'Z' ? int(c - 'A') + 10 : c >= 'a' && c <= 'z' ? int(c - 'a') + 10 : 64; };
            for (; ptr != end; ++ptr) {
                if (*ptr == '\'')
                    continue;
                digit = get_digit(*ptr);
                if (digit >= base)
                    throw std::invalid_argument(Format::format("Invalid base {0} integer: {1:q}", base, s));
                *this *= nbase;
                *this += digit;
            }
        }

        inline void Mpuint::trim() noexcept {
            size_t i = rep_.size() - 1;
            while (i != std::string::npos && rep_[i] == 0)
                --i;
            rep_.resize(i + 1);
        }

    // Signed integers

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

        inline Mpint& Mpint::operator+=(const Mpint& rhs) {
            if (! rhs.mag_) {
                // do nothing
            } else if (! mag_) {
                mag_ = rhs.mag_;
                neg_ = rhs.neg_;
            } else if (neg_ == rhs.neg_) {
                mag_ += rhs.mag_;
            } else {
                switch (mag_.compare(rhs.mag_)) {
                    case -1:
                        mag_ = rhs.mag_ - mag_;
                        neg_ = ! neg_;
                        break;
                    case 1:
                        mag_ -= rhs.mag_;
                        break;
                    default:
                        mag_ = {};
                        neg_ = false;
                        break;
                }
            }
            return *this;
        }

        inline int Mpint::compare(const Mpint& rhs) const noexcept {
            if (neg_ != rhs.neg_)
                return neg_ ? -1 : 1;
            int c = mag_.compare(rhs.mag_);
            return neg_ ? - c : c;
        }

        inline Mpint Mpint::pow(const Mpint& n) const {
            Mpint z;
            z.mag_ = mag_.pow(n.mag_);
            z.neg_ = neg_ && n.mag_.is_odd();
            return z;
        }

        inline std::string Mpint::str(int base, size_t digits, bool sign) const {
            std::string s = mag_.str(base, digits);
            if (neg_)
                s.insert(s.begin(), '-');
            else if (sign)
                s.insert(s.begin(), '+');
            return s;
        }

        inline Mpint Mpint::from_double(double x) {
            Mpint i = Mpuint::from_double(x);
            if (x < 0)
                i = - i;
            return i;
        }

        inline void Mpint::init(std::string_view s, int base) {
            if (base < 0 || base == 1 || base > 36)
                throw std::invalid_argument("Invalid base: " + std::to_string(base));
            if (s.empty())
                return;
            auto b = s.data(), e = b + s.size();
            neg_ = *b == '-';
            if (*b == '+' || *b == '-')
                ++b;
            std::string_view u(b, e - b);
            mag_.init(u, base);
            neg_ &= bool(mag_);
        }

        inline void Mpint::do_divide(const Mpint& x, const Mpint& y, Mpint& q, Mpint& r) {
            Mpint quo, rem;
            Mpuint::do_divide(x.mag_, y.mag_, quo.mag_, rem.mag_);
            if (rem.mag_ && (x.neg_ || y.neg_)) {
                if (x.neg_) {
                    ++quo.mag_;
                    rem.mag_ = y.mag_ - rem.mag_;
                }
            }
            quo.neg_ = bool(quo.mag_) && x.neg_ != y.neg_;
            q = std::move(quo);
            r = std::move(rem);
        }

        inline void Mpint::do_multiply(const Mpint& x, const Mpint& y, Mpint& z) {
            Mpuint::do_multiply(x.mag_, y.mag_, z.mag_);
            z.neg_ = bool(x) && bool(y) && x.neg_ != y.neg_;
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