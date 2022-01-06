# Random Number Generators

_[Scientific Library by Ross Smith](index.html)_

```c++
#include "rs-sci/random.hpp"
namespace RS::Sci;
```

## Contents

* TOC
{:toc}

## Supporting types

```c++
enum class LogMode: int {
    natural,
    common
};
```

Used in some of the logarithmic distributions to indicate whether natural
(base e) or common (base 10) logs are intended.

## Primitive random engines

### Linear congruential generators

```c++
constexpr uint32_t lcg32(uint32_t x) noexcept;
constexpr uint64_t lcg64(uint64_t x) noexcept;
```

Good LCG transformations for 32 and 64 bit integers, from Pierre L'Ecuyer (1999),
["Tables of Linear Congruential Generators of Different Sizes and Good Lattice Structure"](http://www.ams.org/journals/mcom/1999-68-225/S0025-5718-99-00996-5/S0025-5718-99-00996-5.pdf).

```c++
class Lcg32 {
    using result_type = uint32_t;
    constexpr Lcg32() noexcept; // seed(0)
    explicit constexpr Lcg32(uint32_t s) noexcept;
    constexpr uint32_t operator()() noexcept;
    constexpr bool operator==(const Lcg32& rhs) const noexcept;
    constexpr bool operator!=(const Lcg32& rhs) const noexcept;
    constexpr void seed(uint32_t s) noexcept;
    static constexpr uint32_t min() noexcept;
    static constexpr uint32_t max() noexcept;
};
```

```c++
class Lcg64 {
    using result_type = uint64_t;
    constexpr Lcg64() noexcept; // seed(0)
    explicit constexpr Lcg64(uint64_t s) noexcept;
    uint64_t constexpr operator()() noexcept;
    bool constexpr operator==(const Lcg64& rhs) const noexcept;
    bool constexpr operator!=(const Lcg64& rhs) const noexcept;
    void constexpr seed(uint64_t s) noexcept;
    static constexpr uint64_t min() noexcept;
    static constexpr uint64_t max() noexcept;
};
```

### PCG generator

```c++
class Pcg64 {
    using result_type = uint64_t;
    constexpr Pcg64() noexcept; // seed(0,0)
    constexpr explicit Pcg64(uint64_t s) noexcept;
    constexpr explicit Pcg64(uint64_t hi, uint64_t lo) noexcept;
    constexpr uint64_t operator()() noexcept;
    constexpr void advance(int64_t offset) noexcept;
    constexpr void seed(uint64_t s) noexcept;
    constexpr void seed(uint64_t hi, uint64_t lo) noexcept;
    static constexpr uint64_t min() noexcept;
    static constexpr uint64_t max() noexcept;
};
```

[PCG generator](http://www.pcg-random.org/) by Melissa O'Neill.

### Xoshiro generator

```c++
class Xoshiro {
    using result_type = uint64_t;
    constexpr Xoshiro() noexcept; // seed(0,0,0,0)
    constexpr explicit Xoshiro(uint64_t s) noexcept;
    constexpr Xoshiro(uint64_t s, uint64_t t) noexcept;
    constexpr Xoshiro(uint64_t s, uint64_t t, uint64_t u, uint64_t v) noexcept;
    constexpr uint64_t operator()() noexcept;
    constexpr void seed(uint64_t s = 0) noexcept;
    constexpr void seed(uint64_t s, uint64_t t) noexcept;
    constexpr void seed(uint64_t s, uint64_t t, uint64_t u, uint64_t v) noexcept;
    static constexpr uint64_t min() noexcept;
    static constexpr uint64_t max() noexcept;
};
```

[Xoshiro256** generator](http://xoshiro.di.unimi.it/) by David Blackman and
Sebastiano Vigna.

## Standard distributions

Most of these duplicate distributions from the standard library, to allow
consistently reproducible results without depending on the library
implementation.

### Discrete distributions

```c++
template <typename T> class UniformInteger {
    using result_type = T;
    constexpr UniformInteger() noexcept; // range=[0,1]
    constexpr explicit UniformInteger(T r) noexcept; // range=[0,r-1]; UB if r<1
    constexpr UniformInteger(T a, T b) noexcept; // range=[a,b]; UB if a>b
    template <typename RNG> constexpr T operator()(RNG& rng) const noexcept;
    constexpr T min() const noexcept;
    constexpr T max() const noexcept;
};
```

```c++
class BernoulliDistribution {
    using result_type = bool;
    constexpr BernoulliDistribution() noexcept;
    constexpr explicit BernoulliDistribution(double p) noexcept; // UB if p<0 or p>1
    constexpr explicit BernoulliDistribution(Rational p) noexcept: // UB if p<0 or p>1
    constexpr BernoulliDistribution(int num, int den) noexcept; // p=num/den; UB if num<0, den<1, or num>den
    template <typename RNG> constexpr bool operator()(RNG& rng) const noexcept;
    constexpr Rational p() const noexcept;
};
```

```c++
template <typename T> class DiscreteNormal {
    using result_type = T;
    DiscreteNormal() noexcept; // mean=0,sd=1
    DiscreteNormal(double mean, double sd) noexcept;
    template <typename RNG> T operator()(RNG& rng) const noexcept;
    double mean() const noexcept;
    double sd() const noexcept;
};
```

```c++
template <typename T, typename U = double> class PoissonDistribution {
    using result_type = T; // integer type
    using scalar_type = U; // floating point type
    PoissonDistribution() noexcept; // lambda=1
    explicit PoissonDistribution(U lambda) noexcept; // UB if lambda<=0
    template <typename RNG> T operator()(RNG& rng) const noexcept;
    U mean() const noexcept;
    U sd() const noexcept;
};
```

### Continuous distributions

```c++
template <typename T> class UniformReal {
    using result_type = T;
    constexpr UniformReal() noexcept; // range=[0,1]
    constexpr explicit UniformReal(T r) noexcept; // range=[0,r); UB if r<=0
    constexpr UniformReal(T a, T b) noexcept; // range=[a,b); UB if a>b
    template <typename RNG> constexpr T operator()(RNG& rng) const noexcept;
    constexpr T min() const noexcept;
    constexpr T max() const noexcept;
};
```

```c++
template <typename T> class NormalDistribution {
    using result_type = T;
    NormalDistribution() noexcept; // mean=0, sd=1
    NormalDistribution(T mean, T sd) noexcept;
    template <typename RNG> T operator()(RNG& rng) const noexcept;
    T mean() const noexcept;
    T sd() const noexcept;
};
```

```c++
template <typename T> class LogUniform {
    using result_type = T;
    constexpr LogUniform() noexcept; // range=[0,e)
    constexpr LogUniform(T min, T max) noexcept; // UB if min>max
    template <typename RNG> constexpr T operator()(RNG& rng) const noexcept;
    constexpr T min() const noexcept;
    constexpr T max() const noexcept;
};
```

```c++
template <typename T> class LogNormal {
    using result_type = T;
    LogNormal() noexcept; // m=0, s=1
    LogNormal(T m, T s, LogMode mode = LogMode::natural) noexcept;
    template <typename RNG> T operator()(RNG& rng) const noexcept;
    T m() const noexcept;
    T s() const noexcept;
    T median() const noexcept;
    T s_factor() const noexcept; // pow(base,s)
};
```

## Distribution adapters

```c++
template <typename Base> class ConstrainedDistribution {
    using base_distribution = Base;
    using result_type = typename Base::result_type;
    ConstrainedDistribution(const Base& dist, result_type min, result_type max);
    template <typename... Args> ConstrainedDistribution(result_type min, result_type max, Args&&... args);
    template <typename RNG> result_type operator()(RNG& rng) const;
    result_type min() const noexcept;
    result_type max() const noexcept;
};
```

Distribution with resampling from a constrained range. The underlying base
distribution can be any distribution with a totally ordered result type.

The first constructor takes an underlying distribution (which is copied) and
the permitted range; the second takes the permitted range and a set of
arguments that will be passed to the base distribution's constructor (the
different argument order between the two constructors is to ensure unambiguous
overload resolution). The constructors will throw `std::invalid_argument` if
`min>max` or if the intersection of `[min,max]` and the range of the base
distribution is empty.

## Non-arithmetic discrete distributions

```c++
template <typename T> class RandomChoice {
    using result_type = T;
    RandomChoice();
    RandomChoice(std::initializer_list<T> list);
    template <typename Range> explicit RandomChoice(const Range& range);
    template <typename RNG> const T& operator()(RNG& rng) const;
    template <typename... Args> RandomChoice& add(const Args&... args);
    bool empty() const noexcept;
    size_t size() const noexcept;
};
```

Selects a random item from a set of values. Behaviour is undefined if the
function call operator is called on an empty distribution.

```c++
template <typename T> class WeightedChoice {
    using result_type = T;
    WeightedChoice();
    WeightedChoice(std::initializer_list<...> list);
    template <typename RNG> const T& operator()(RNG& rng) const;
    template <typename... Args> WeightedChoice& add(double w, const Args&... args);
    bool empty() const noexcept;
    double total_weight() const noexcept;
};
```

Selects a random item from a weighted set of values.

The `add()` function takes a weight and a number of values; the weight is
divided evenly between the values. A call to `add()` is ignored if the weight
is less than or equal to zero. The constructor argument list is a list of
nested initializer lists, each corresponding to the arguments to a call to
`add()`.

Behaviour is undefined if the function call operator is called on an empty
distribution.
