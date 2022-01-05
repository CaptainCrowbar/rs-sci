# Statistics

_[Scientific Library by Ross Smith](index.html)_

```c++
#include "rs-sci/statistics.hpp"
namespace RS::Sci;
```

## Statistics class

```c++
template <typename T> class Statistics;
```

A `Statistics` object keeps running statistics for one or two variables. `T`
must be a floating point arithmetic type.

Based on code by John Cook:

* [Standard deviation](https://www.johndcook.com/blog/standard_deviation/)
* [Skewness and kurtosis](https://www.johndcook.com/blog/skewness_kurtosis/)
* [Linear regression](https://www.johndcook.com/blog/running_regression/)

```c++
constexpr Statistics::Statistics() noexcept;
constexpr Statistics::Statistics(const Statistics& s) noexcept;
constexpr Statistics::Statistics(Statistics&& s) noexcept;
constexpr Statistics::~Statistics() noexcept;
constexpr Statistics& Statistics::operator=(const Statistics& s) noexcept;
constexpr Statistics& Statistics::operator=(Statistics&& s) noexcept;
```

Life cycle functions.

```c++
constexpr Statistics& Statistics::operator()(T x) noexcept;
constexpr Statistics& Statistics::operator()(T x, T y) noexcept;
```

The function call operator adds a new value, or a new pair of values, to the
statistics. Calling `stats(x)` is equivalent to `stats(x,0)`, but skips
redundant arithmetic.

```c++
constexpr Statistics Statistics::operator+(const Statistics& s) const noexcept;
constexpr Statistics& Statistics::operator+=(const Statistics& s) noexcept;
```

The addition operators combine sets of statistics.

```c++
constexpr void Statistics::clear() noexcept;
```

Resets all statistics o their initial empty state.

```c++
constexpr int Statistics::n() const noexcept;
constexpr T Statistics::tn() const noexcept;
```

Return the number of items entered, as an integer or a floating point number.

```c++
constexpr T Statistics::x_min() const noexcept;
constexpr T Statistics::y_min() const noexcept;
constexpr T Statistics::x_max() const noexcept;
constexpr T Statistics::y_max() const noexcept;
```

Return the minimum and maximum values. Behaviour is undefined if `n<1`.

```c++
constexpr T Statistics::x_mean() const noexcept;
constexpr T Statistics::y_mean() const noexcept;
```

Return the mean values. Behaviour is undefined if `n<1`.

```c++
constexpr T Statistics::x_variance() const noexcept;
constexpr T Statistics::y_variance() const noexcept;
T Statistics::x_sd() const noexcept;
T Statistics::y_sd() const noexcept;
constexpr T Statistics::x_pop_variance() const noexcept;
constexpr T Statistics::y_pop_variance() const noexcept;
T Statistics::x_pop_sd() const noexcept;
T Statistics::y_pop_sd() const noexcept;
```

Return the sample (with Bessel correction) and population (no Bessel
correction) variance and standard deviation. Behaviour is undefined for the
uncorrected population measures if `n<1`, and for the corrected sample
measures if `n<2`.

```c++
T Statistics::x_skewness() const noexcept;
T Statistics::y_skewness() const noexcept;
T Statistics::x_kurtosis() const noexcept;
T Statistics::y_kurtosis() const noexcept;
```

Return the skewness (third standard moment) and kurtosis (Pearson's modified
fourth moment) of the sample. Behaviour is undefined for both functions if
`n<2`.

```c++
T Statistics::r() const noexcept;
```

Returns the Pearson coefficient of correlation. Behaviour is undefined if
`n<1`.

```c++
T Statistics::a() const noexcept;
T Statistics::b() const noexcept;
T Statistics::inv_a() const noexcept;
T Statistics::inv_b() const noexcept;
```

Return the linear regression coefficients in `y=a*x+b`. The `inv_*()`
functions return the inverse coefficients in `x=a'*y+b'`. Behaviour of all of
these functions is undefined if `n<1`.
