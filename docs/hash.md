# Hash Functions

_[Scientific Library by Ross Smith](index.html)_

```c++
#include "rs-sci/hash.hpp"
namespace RS::Sci;
```

## Contents

* TOC
{:toc}

## Hash mixing functions

```c++
template <typename... Args> size_t hash_mix(const Args&... args);
template <typename Range> size_t hash_mix(const Range& r);
```

Hash mixing functions. These return the combined hash of a list or range of
objects, calling `std::hash` on each item.

## Bernstein multiplicative hash

```c++
uint32_t bernstein_hash(const void* ptr, size_t len) noexcept {
```

Daniel Bernstein's simple multiplicative hash.

## SipHash

```c++
class SipHash {
    using result_type = uint64_t;
    constexpr SipHash() noexcept; // keys=(0,0)
    constexpr SipHash(uint64_t key0, uint64_t key1) noexcept;
    uint64_t constexpr operator()(const void* ptr, size_t len) const noexcept;
};
```

[SipHash](https://github.com/veorq/SipHash) by Jean-Philippe Aumasson and
Daniel J. Bernstein is widely used as a hash table keying function to avoid
hash flooding attacks. This implements the most common variant,
SipHash-2-4-64.
