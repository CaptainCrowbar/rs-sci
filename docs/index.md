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

Other libraries required (all header only):

[My formatting library](https://github.com/CaptainCrowbar/rs-format)
[My graphics core library](https://github.com/CaptainCrowbar/rs-graphics-core)
[My template library](https://github.com/CaptainCrowbar/rs-tl)
[My unit test library](https://github.com/CaptainCrowbar/rs-unit-test)

## Index

* [Algorithms](algorithms.html)
* [Constants](constants.html)
* [Multiple precision integers](mp-integer.html)
* [Random number generators](random.html)
* [Rational numbers](rational.html)
* [Statistics](statistics.html)
* [Useful formulae](formula.html)
* [Version information](version.html)
