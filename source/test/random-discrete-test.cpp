#include "rs-sci/random.hpp"
#include "rs-sci/statistics.hpp"
#include "rs-unit-test.hpp"
#include <map>

using namespace RS::Sci;

void test_rs_sci_random_bernoulli_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    BernoulliDistribution dist;
    bool b = false;
    int n = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(b = dist(rng));
        n += int(b);
    }

    double p = double(n) / iterations;
    TEST_NEAR(p, 0.5, 0.001);

    TRY(dist = BernoulliDistribution(1, 10));
    n = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(b = dist(rng));
        n += int(b);
    }

    p = double(n) / iterations;
    TEST_NEAR(p, 0.1, 0.001);

    TRY(dist = BernoulliDistribution(0.9));
    n = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(b = dist(rng));
        n += int(b);
    }

    p = double(n) / iterations;
    TEST_NEAR(p, 0.9, 0.001);

}

void test_rs_sci_random_uniform_integer_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    UniformInteger<int> dist(5);
    std::map<int, int> census;
    int x = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        ++census[x];
    }

    TEST_EQUAL(census.size(), 5u);

    TEST_NEAR(census[0] / double(iterations), 0.2, 0.002);
    TEST_NEAR(census[1] / double(iterations), 0.2, 0.002);
    TEST_NEAR(census[2] / double(iterations), 0.2, 0.002);
    TEST_NEAR(census[3] / double(iterations), 0.2, 0.002);
    TEST_NEAR(census[4] / double(iterations), 0.2, 0.002);

    TRY(dist = UniformInteger(1, 5));
    census.clear();

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        ++census[x];
    }

    TEST_EQUAL(census.size(), 5u);

    TEST_NEAR(census[1] / double(iterations), 0.2, 0.002);
    TEST_NEAR(census[2] / double(iterations), 0.2, 0.002);
    TEST_NEAR(census[3] / double(iterations), 0.2, 0.002);
    TEST_NEAR(census[4] / double(iterations), 0.2, 0.002);
    TEST_NEAR(census[5] / double(iterations), 0.2, 0.002);

}

void test_rs_sci_random_poisson_distribution() {

    static constexpr int iterations = 1'000'000;

    PoissonDistribution<int> poi;
    Statistics<double> stats;
    Pcg64 rng(42);
    int x = 0;

    TRY(poi = PoissonDistribution<int>(4));
    TEST_EQUAL(poi.mean(), 4);
    TEST_EQUAL(poi.sd(), 2);

    for (int i = 0; i < iterations; ++i) {
        TRY(x = poi(rng));
        stats(x);
    }

    TEST_NEAR(stats.mean(), poi.mean(), 0.002);
    TEST_NEAR(stats.sd(), poi.sd(), 0.002);

    TRY(poi = PoissonDistribution<int>(100));
    TEST_EQUAL(poi.mean(), 100);
    TEST_EQUAL(poi.sd(), 10);

    stats.clear();

    for (int i = 0; i < iterations; ++i) {
        TRY(x = poi(rng));
        stats(x);
    }

    TEST_NEAR(stats.mean(), poi.mean(), 0.2);
    TEST_NEAR(stats.sd(), poi.sd(), 0.2);

}
