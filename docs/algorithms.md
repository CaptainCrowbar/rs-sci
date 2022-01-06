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

## Interpolation

For all of these templates, `T` must be a floating point arithmetic type.

```c++
enum Interpolate: int {
    log_x = 1,
    log_y = 2,
};
```

The interpolation functions take a flags parameter, which is a combination of
these bitmask flags, to indicate which of the X and Y scales are
logarithmic.

```c++
template <typename T>
    T interpolate(T x1, T y1, T x2, T y2, T x, int flags = 0) noexcept;
```

Interpolate or extrapolate to a third point, given two points. Behaviour is
undefined if `x1==x2`; if the `log_x` flag is used and any of the X values
are less than or equal to zero; or if the `log_y` flag is used and either of
the Y values is less than or equal to zero.

```c++
template <typename T, int Flags = 0> class InterpolatedMap {
    InterpolatedMap();
    InterpolatedMap(std::initializer_list<std::pair<const T, T>> list);
    explicit InterpolatedMap(const std::vector<std::pair<T, T>> points);
    InterpolatedMap& insert(T x, T y);
    T operator()(T x) const noexcept;
};
```

This is a map containing a sequence of key-value pairs. The function call
operator performs interpolation between the nearest two points
(or extrapolation if the argument is outside the mapped range). If two points
have the same X value but different Y values, a later entry will overwrite an
earlier one. The constructors and `insert()` function will throw
`std::invalid_argument` if a zero or negative value is supplied for a
log-scaled parameter.

```c++
template <typename T, int Flags = 0> class CubicSplineMap {
    CubicSplineMap();
    CubicSplineMap(std::initializer_list<std::pair<T, T>> list);
    explicit CubicSplineMap(const std::vector<std::pair<T, T>> points);
    T operator()(T x) const noexcept;
};
```

This performs cubic spline interpolation. Its behaviour is otherwise similar
to `InterpolatedMap`. In addition to the usual log scale checking, the
constructors will throw `std::invalid_argument` if less than four distinct
points are supplied, or if two points have the same X value but different Y
values.

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
