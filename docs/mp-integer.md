# Multiple Precision Integers

_[Scientific Library by Ross Smith](index.html)_

```c++
#include "rs-sci/mp-integer.hpp"
namespace RS::Sci;
```

## Contents

* TOC
{:toc}

## Multiple precision unsigned integer class

```c++
class Mpuint;
```

An arbitrary precision unsigned integer (natural number). Most of its methods
are conventional arithmetic operations with their natural behaviour.

```c++
Mpuint::Mpuint();
```

The default constructor sets the value to zero.

```c++
Mpuint::Mpuint(uint64_t x);
```

Constructor from a 64-bit value.

```c++
explicit Mpuint::Mpuint(std::string_view s, int base = 0);
```

The constructor from a string accepts base of 0 or 2-36; it will throw
`std::invalid_argument` if the base is not one of these. Base 0 will check
for a `"0b"` or `"0x"` prefix, assuming decimal otherwise. Input is case
insensitive; apostrophe delimiters are allowed.

```c++
Mpuint::Mpuint(const Mpuint& m);
Mpuint::Mpuint(Mpuint&& m) noexcept;
Mpuint::~Mpuint() noexcept;
Mpuint& Mpuint::operator=(const Mpuint& m);
Mpuint& Mpuint::operator=(Mpuint&& m) noexcept;
```

Other life cycle functions.

```c++
template <typename T> explicit Mpuint::operator T() const;
```

The explicit cast template returns the value converted to `T`, if possible.
`T` must be a primitive integer or floating point arithmetic type. Results
are unspecified if the value is outside the return type's representable
range.

```c++
explicit Mpuint::operator bool() const noexcept;
```

True if the value is not zero.

```c++
bool Mpuint::operator!() const noexcept;
Mpuint Mpuint::operator+() const;
Mpuint& Mpuint::operator++();
Mpuint Mpuint::operator++(int);
Mpuint& Mpuint::operator--();
Mpuint Mpuint::operator--(int);
Mpuint& Mpuint::operator+=(const Mpuint& y);
Mpuint& Mpuint::operator-=(const Mpuint& y);
Mpuint& Mpuint::operator*=(const Mpuint& y);
Mpuint& Mpuint::operator/=(const Mpuint& y);
Mpuint& Mpuint::operator%=(const Mpuint& y);
Mpuint operator+(const Mpuint& x, const Mpuint& y);
Mpuint operator-(const Mpuint& x, const Mpuint& y);
Mpuint operator*(const Mpuint& x, const Mpuint& y);
Mpuint operator/(const Mpuint& x, const Mpuint& y);
Mpuint operator%(const Mpuint& x, const Mpuint& y);
```

Arithmetic operators. For division operations, behaviour is undefined if the
divisor is zero.

```c++
Mpuint& Mpuint::operator&=(const Mpuint& y);
Mpuint& Mpuint::operator|=(const Mpuint& y);
Mpuint& Mpuint::operator^=(const Mpuint& y);
Mpuint& Mpuint::operator<<=(ptrdiff_t y);
Mpuint& Mpuint::operator>>=(ptrdiff_t y);
Mpuint operator&(const Mpuint& x, const Mpuint& y);
Mpuint operator|(const Mpuint& x, const Mpuint& y);
Mpuint operator^(const Mpuint& x, const Mpuint& y);
Mpuint operator<<(const Mpuint& x, size_t y);
Mpuint operator>>(const Mpuint& x, size_t y);
```

Bitwise operators.

```c++
bool operator==(const Mpuint& x, const Mpuint& y) noexcept;
bool operator!=(const Mpuint& x, const Mpuint& y) noexcept;
bool operator<(const Mpuint& x, const Mpuint& y) noexcept;
bool operator>(const Mpuint& x, const Mpuint& y) noexcept;
bool operator<=(const Mpuint& x, const Mpuint& y) noexcept;
bool operator>=(const Mpuint& x, const Mpuint& y) noexcept;
```

Comparison operators.

```c++
size_t Mpuint::bits() const noexcept;
size_t Mpuint::bits_set() const noexcept;
```

The `bits()` function returns the number of bits in the number's binary
representation, starting with the highest 1 bit; `bits_set()` returns the
number of 1 bits.

```c++
size_t Mpuint::bytes() const noexcept;
```

The `bytes()` function returns the number of bytes needed to hold the number.

```c++
int Mpuint::compare(const Mpuint& y) const noexcept;
```

Returns a three way comparison of `*this` and `y`, returning -1 if
`*this<y`, 0 if `*this=y`, and 1 if `*this>y`.

```c++
void Mpuint::flip_bit(size_t i);
bool Mpuint::get_bit(size_t i) const noexcept;
void Mpuint::set_bit(size_t i, bool b = true);
uint8_t Mpuint::get_byte(size_t i) const noexcept;
void Mpuint::set_byte(size_t i, uint8_t b);
```

The `get_bit/byte()`, `set_bit/byte()`, and `flip_bit()` functions read or
write a specific bit or byte, selected by zero based index from the least
significant end. If the index is too big for the stored value,
`get_bit/byte()` will return zero or false, while `set_bit/byte()` and
`flip_bit()` will extend the value to include any bits newly set to 1.

```c++
size_t Mpuint::hash() const noexcept;
class std::hash<Mpuint>;
```

Hash functions.

```c++
bool Mpuint::is_even() const noexcept;
bool Mpuint::is_odd() const noexcept;
```

Parity functions.

```c++
Mpuint Mpuint::pow(const Mpuint& n) const;
```

Raises `*this` to the power of `n`.

```c++
int Mpuint::sign() const noexcept;
```

Returns the sign of `*this` (0 if zero, otherwise 1).

```c++
std::string Mpuint::str(int base = 10, size_t digits = 1) const;
std::string to_string(const Mpuint& x);
std::ostream& operator<<(std::ostream& out, const Mpuint& x);
```

The `str()` function will throw `std::invalid_argument` if the base is less
than 2 or greater than 36. The output operator and `to_string()` call
`str(10)`.

```c++
void Mpuint::write_be(void* ptr, size_t n) const noexcept;
void Mpuint::write_le(void* ptr, size_t n) const noexcept;
static Mpuint Mpuint::read_be(const void* ptr, size_t n);
static Mpuint Mpuint::read_le(const void* ptr, size_t n);
```

These read and write an integer from a block of bytes, in big or little endian
order. The write functions will truncate the integer if it is too big to fit
in the specified number of bytes.

```c++
std::pair<Mpuint, Mpuint> divide(const Mpuint& x, const Mpuint& y);
```

Returns the quotient and remainder of `x/y`. Behaviour is undefined if the
divisor is zero.

```c++
static Mpuint Mpuint::from_double(double x);
```

Converts a `double` to an integer. Fractions are rounded down.

## Multiple precision signed integer class

```c++
class Mpint;
```

An arbitrary precision signed integer. Most of its operations do the same
thing as the corresponding functions on `Mpuint` or plain `int`.

```c++
using Mpratio = Ratio<Mpint>;
```

An arbitrary precision rational number.

```c++
Mpint::Mpint();
```

The default constructor sets the value to zero.

```c++
Mpint::Mpint(int64_t x);
```

Constructor from a 64-bit value.

```c++
Mpint::Mpint(const Mpuint& x);
```

Conversion constructor from an unsigned multiple precision integer.

```c++
explicit Mpint::Mpint(std::string_view s, int base = 0);
```

The constructor from a string accepts base of 0 or 2-36; it will throw
`std::invalid_argument` if the base is not one of these. Base 0 will check
for a `"0b"` or `"0x"` prefix, assuming decimal otherwise. Input is case
insensitive; apostrophe delimiters are allowed.

```c++
Mpint::Mpint(const Mpint& m);
Mpint::Mpint(Mpint&& m) noexcept;
Mpint::~Mpint() noexcept;
Mpint& Mpint::operator=(const Mpint& m);
Mpint& Mpint::operator=(Mpint&& m) noexcept;
```

Other life cycle functions.

```c++
template <typename T> explicit Mpint::operator T() const;
```

The explicit cast template returns the value converted to `T`, if possible.
`T` must be a primitive integer or floating point arithmetic type. Results
are unspecified if the value is outside the return type's representable
range.

```c++
explicit Mpint::operator Mpuint() const;
```

The explicit conversion to `Mpuint` returns the absolute value of the number.

```c++
explicit Mpint::operator bool() const noexcept;
```

True if the value is not zero.

```c++
bool Mpint::operator!() const noexcept;
Mpint Mpint::operator+() const;
Mpint Mpint::operator-() const;
Mpint& Mpint::operator++();
Mpint Mpint::operator++(int);
Mpint& Mpint::operator--();
Mpint Mpint::operator--(int);
Mpint& Mpint::operator+=(const Mpint& y);
Mpint& Mpint::operator-=(const Mpint& y);
Mpint& Mpint::operator*=(const Mpint& y);
Mpint& Mpint::operator/=(const Mpint& y);
Mpint& Mpint::operator%=(const Mpint& y);
Mpint operator+(const Mpint& x, const Mpint& y);
Mpint operator-(const Mpint& x, const Mpint& y);
Mpint operator*(const Mpint& x, const Mpint& y);
Mpint operator/(const Mpint& x, const Mpint& y);
Mpint operator%(const Mpint& x, const Mpint& y);
```

Arithmetic operators. The division operators perform Euclidean division: if
the division is not exact, the remainder is always positive regardless of the
signs of the arguments, and the quotient is the integer that satisfies
`x=q*y+r`. Behaviour is undefined if the divisor is zero.

```c++
bool operator==(const Mpint& x, const Mpint& y) noexcept;
bool operator!=(const Mpint& x, const Mpint& y) noexcept;
bool operator<(const Mpint& x, const Mpint& y) noexcept;
bool operator>(const Mpint& x, const Mpint& y) noexcept;
bool operator<=(const Mpint& x, const Mpint& y) noexcept;
bool operator>=(const Mpint& x, const Mpint& y) noexcept;
```

Comparison operators.

```c++
Mpuint Mpint::abs() const;
```

Returns the absolute value of `*this`.

```c++
int Mpint::compare(const Mpint& y) const noexcept;
```

Returns a three way comparison of `*this` and `y`, returning -1 if
`*this<y`, 0 if `*this=y`, and 1 if `*this>y`.

```c++
size_t Mpint::hash() const noexcept;
class std::hash<Mpint>;
```

Hash functions.

```c++
bool Mpint::is_even() const noexcept;
bool Mpint::is_odd() const noexcept;
```

Parity functions.

```c++
Mpint Mpint::pow(const Mpint& n) const;
```

Raises `*this` to the power of `n`. Behaviour is undefined if the exponent is
negative.

```c++
int Mpint::sign() const noexcept;
```

Returns the sign of `*this` (-1 if negative, 0 if zero, 1 if positive).

```c++
std::string Mpint::str(int base = 10, size_t digits = 1, bool sign = false) const;
std::string to_string(const Mpint& x);
std::ostream& operator<<(std::ostream& out, const Mpint& x);
```

The `str()` function will throw `std::invalid_argument` if the base is less
than 2 or greater than 36. The output operator and `to_string()` call
`str(10)`.

```c++
static std::pair<Mpint, Mpint> divide(const Mpint& x, const Mpint& y);
```

Returns the quotient and remainder of `x/y`. Division works as described above.

```c++
static Mpint Mpint::from_double(double x);
```

Converts a `double` to an integer. Fractions are rounded down.

## Integer literals

```c++
namespace Literals {
    Mpint operator""_mpi(const char* raw);
    Mpuint operator""_mpu(const char* raw);
}
```

Multiple precision integer literals. These allow literals such as `12345_mpi`
or `0xabcdef_mpu`. They perform the same conversions as the constructors with
base 0.
