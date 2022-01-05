# Algorithms

_[Scientific Library by Ross Smith](index.html)_

```c++
#include "rs-sci/algorithms.hpp"
namespace RS::Sci;
```

## Contents

* TOC
{:toc}

## Integer algorithms

```c++
template <typename T> constexpr T binomial(T a, T b) noexcept;
```

Returns the binomial coefficient of `(a,b)`, equal to `a!/b!(a-b)!` if
`b∈[0,a]`, otherwise zero). `T` must be an arithmetic type. Behaviour is
undefined if the correct result would be out of range for `T`, or if `T` is
floating point and either argument is not an integer value.

## Numerical algorithms

```c++
template <typename T, typename F>
    T line_integral(T x1, T x2, int k, F f);
```

Computes the integral of `f(x)` over the interval `[x1,x2]` by the trapezoid
algorithm, using `k` subdivisions. Behaviour is undefined if `k<1` or the
function has a pole within the interval.

```c++
template <typename T, int N, typename F>
    T volume_integral(Vector<T, N> x1, Vector<T, N> x2, int k, F f);
```

Computes the volume integral of `f(x)` over the rectangular prism whose
opposite corners are `x1` and `x2`, dividing each side into `k` subdivisions.
This has complexity _O(k<sup>N</sup>)._ Behaviour is undefined if `k<1` or
the function has a pole within the volume.

```c++
template <typename T> class PrecisionSum {
    using value_type = T;
    PrecisionSum& add(T t);
    PrecisionSum& operator()(T t); // same as add()
    void clear() noexcept;
    T get() const;
    operator T() const; // same as get()
};
template <typename SinglePassRange>
    [value type] precision_sum(const SinglePassRange& range);
```

Calculate the sum of a sequence of numbers using the high precision algorithm from
[Shewchuk](http://www-2.cs.cmu.edu/afs/cs/project/quake/public/papers/robust-arithmetic.ps)
and [Hettinger](http://code.activestate.com/recipes/393090/).

This can be called as either an accumulator to which values can be added one
at a time, or a range based function that calculates the sum in one call. The
range's value type must be a floating point arithmetic type. This is always
much more accurate than simple addition, and is guaranteed to give the
correct answer (the exact sum correctly rounded) if the value type implements
IEEE arithmetic (on GCC this requires the `-ffloat-store` option).

## Range algorithms

```c++
template <typename ForwardRange, typename UnaryFunction, typename Compare>
    [iterator] find_optimum(ForwardRange& range, UnaryFunction f,
        Compare cmp);
template <typename ForwardRange, typename UnaryFunction>
    [iterator] find_optimum(ForwardRange& range, UnaryFunction f);
```

Return an iterator identifying the range element for which `f(x)` has the
maximum value, according to the given comparison function. The comparison
function is expected to have less-than semantics, and defaults to
`std::greater<T>()`, where `T` is the return type of `f()`. Use
`std::less<T>()` to get the minimum value of `f(x)` instead of the maximum. If
there is more than one optimum element, the first one will be returned. These
will return an end iterator only if the range is empty.
