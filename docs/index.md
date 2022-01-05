# Scientific Library

By Ross Smith

_[GitHub repository](https://github.com/CaptainCrowbar/rs-sci)_

## Overview

```c++
#include "rs-sci.hpp"
namespace RS::Sci;
```

This is a headers-only library; there is nothing to build (unless you want to
run the unit tests), and no binaries to install. The CMake file includes an
`install` target to copy the headers into `/usr/local/include` or the
equivalent. Headers can be included individually as required, or the entire
library can be included using `"rs-sci.hpp"`.

[My unit test library](https://github.com/CaptainCrowbar/rs-unit-test) is used
for this library's unit tests. This a header-only library, and is not
required if you only want to use this library without building its unit
tests.

## Index

* [Algorithms](algorithms.html)
* [Constants](constants.html)
* [Statistics](statistics.html)
* [Version information](version.html)
