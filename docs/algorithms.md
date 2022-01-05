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
