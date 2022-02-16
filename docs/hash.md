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
    constexpr SipHash() noexcept; // SipHash(0,0)
    constexpr SipHash(uint64_t key0, uint64_t key1) noexcept;
    uint64_t constexpr operator()(const void* ptr, size_t len) const noexcept;
};
```

[SipHash](https://github.com/veorq/SipHash) by Jean-Philippe Aumasson and
Daniel J. Bernstein is widely used as a hash table keying function to avoid
hash flooding attacks. This implements the most common variant,
SipHash-2-4-64.

## Cryptographic hash functions

| Class     | Bits  | Bytes  |
| -----     | ----  | -----  |
| `Md5`     | 128   | 16     |
| `Sha1`    | 160   | 20     |
| `Sha256`  | 256   | 32     |
| `Sha512`  | 512   | 64     |

Each class has the following interface:

```c++
class ClassName {
    using result_type = std::string;
    static constexpr size_t bits = BitSize;
    static constexpr size_t bytes = BitSize / 8;
    ClassName();
    ~ClassName() noexcept;
    std::string operator()(const void* ptr, size_t len);
    std::string operator()(const std::string& str);
    void add(const void* ptr, size_t len);
    void add(const std::string& str);
    void clear() noexcept;
    std::string get();
};
```

These classes generate cryptographic hashes by calling the operating system's
native cryptographic API. The hash is returned as a string containing a fixed
number of bytes. These classes are not copyable or movable. They can be used
either in progressive or immediate mode.

In progressive mode, a hash class object is default constructed or reset using
`clear()`. One or more blocks of data are processed by calling `add()` any
number of times. The hash value can then be retrieved using `get()`. The
`get()` function will return the same value if called multiple times with no
intervening calls to `clear()` or `add()`. Behaviour is undefined if `add()`
is called after `get()` without an intervening call to `clear()`.

In immediate mode, the function call operator is used to hash a single block
of data and return the resulting hash value in one call. This always starts
from a clean slate; any progressive hash state already in the object will be
discarded. Calling `get()` after `operator()` will return the same value.

Uses of the progressive and immediate mode APIs can be mixed on the same
object, provided `clear()` is always used to reset the state before a
progressive hash.
