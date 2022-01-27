#include "rs-sci/random-other-distributions.hpp"
#include "rs-sci/random-engines.hpp"
#include "rs-unit-test.hpp"
#include <algorithm>

using namespace RS::Sci;

void test_rs_sci_random_constrained_distribution() {

    using constrained_int = ConstrainedDistribution<UniformInteger<int>>;

    Pcg64 rng(42);
    constrained_int ci(1, 10, -20, 20);
    int min = 9999;
    int max = -9999;
    int x = 0;

    TEST_EQUAL(ci.min(), 1);
    TEST_EQUAL(ci.max(), 10);

    for (int i = 0; i < 100'000; ++i) {
        TRY(x = ci(rng));
        min = std::min(min, x);
        max = std::max(max, x);
    }

    TEST_EQUAL(min, 1);
    TEST_EQUAL(max, 10);

}
