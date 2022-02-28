#pragma once

#include "rs-format/enum.hpp"
#include "rs-format/spec.hpp"
#include "rs-tl/types.hpp"
#include <string>

namespace RS::Sci {

    class Approx:
    public TL::BinaryOperators<Approx>,
    public TL::TotalOrder<Approx> {

    public:

        enum class err { abs, rel };
        enum class parse { approx, exact };

        constexpr Approx() noexcept {}
        constexpr Approx(double v) noexcept: value_(v), error_(0) {}
        constexpr Approx(double v, double se, err mode = err::abs) noexcept;
        explicit Approx(const std::string& str, parse mode = parse::approx);

        constexpr bool is_exact() const noexcept { return error_ == 0; }
        constexpr double value() const noexcept { return value_; }
        constexpr double error(err mode = err::abs) const noexcept;
        constexpr void set_value(double v) noexcept { value_ = v; }
        constexpr void set_error(double se, err mode = err::abs) noexcept;

        constexpr Approx operator+() noexcept { return *this; }
        constexpr Approx operator-() noexcept { return {- value_, error_}; }
        constexpr Approx& operator+=(const Approx& b) noexcept;
        constexpr Approx& operator-=(const Approx& b) noexcept;
        constexpr Approx& operator*=(const Approx& b) noexcept;
        constexpr Approx& operator/=(const Approx& b) noexcept;
        constexpr friend bool operator==(const Approx& a, const Approx& b) noexcept { return a.value_ == b.value_; }
        constexpr friend bool operator<(const Approx& a, const Approx& b) noexcept { return a.value_ < b.value_; }

        std::string str(Format::FormatSpec spec = {}) const;
        friend std::string to_string(const Approx& a, Format::FormatSpec spec = {}) { return a.str(spec); }
        friend std::ostream& operator<<(std::ostream& out, const Approx& a) { return out << a.str(); }

    private:

        double value_ = 0;
        double error_ = 0;

        static constexpr double c_abs(double x) noexcept { return x < 0 ? - x : x; }

    };

        constexpr Approx::Approx(double v, double se, err mode) noexcept:
        value_(v), error_(c_abs(se)) {
            if (mode == err::rel)
                error_ *= value_;
        }

        constexpr double Approx::error(err mode) const noexcept {
            return mode == err::rel ? c_abs(error_ / value_) : error_;
        }

        constexpr void Approx::set_error(double se, err mode) noexcept {
            error_ = c_abs(se);
            if (mode == err::rel)
                error_ *= value_;
        }

        constexpr Approx& Approx::operator+=(const Approx& b) noexcept {
            // TODO
            (void)b;
            return *this;
        }

        constexpr Approx& Approx::operator-=(const Approx& b) noexcept {
            // TODO
            (void)b;
            return *this;
        }

        constexpr Approx& Approx::operator*=(const Approx& b) noexcept {
            // TODO
            (void)b;
            return *this;
        }

        constexpr Approx& Approx::operator/=(const Approx& b) noexcept {
            // TODO
            (void)b;
            return *this;
        }

}
