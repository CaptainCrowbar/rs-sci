#include "rs-sci/random.hpp"
#include "rs-sci/statistics.hpp"
#include "rs-unit-test.hpp"
#include <array>
#include <cmath>

using namespace RS::Sci;

void test_rs_sci_random_uniform_real_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    UniformReal<double> dist;
    Statistics<double> stats;
    double x = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        stats(x);
    }

    TEST_NEAR(stats.mean(), 0.5, 0.001);
    TEST_NEAR(stats.sd(), 0.2887, 0.001);
    TEST_NEAR(stats.min(), 0, 0.001);
    TEST_NEAR(stats.max(), 1, 0.001);

    TRY(dist = UniformReal<double>(-100, 100));
    stats.clear();

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        stats(x);
    }

    TEST_NEAR(stats.mean(), 0, 0.1);
    TEST_NEAR(stats.sd(), 57.74, 0.1);
    TEST_NEAR(stats.min(), -100, 0.1);
    TEST_NEAR(stats.max(), 100, 0.1);

}

void test_rs_sci_random_log_uniform_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    LogUniform<double> dist(10, 100'000);
    Statistics<double> stats;
    std::array<int, 4> count = {{0,0,0,0}};
    double x = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        stats(x);
        if (x < 100)
            ++count[0];
        else if (x < 1'000)
            ++count[1];
        else if (x < 10'000)
            ++count[2];
        else
            ++count[3];
    }

    double fraction[4];
    for (int i = 0; i < 4; ++i)
        fraction[i] = count[i] / double(iterations);

    TEST_NEAR(fraction[0], 0.25, 0.001);
    TEST_NEAR(fraction[1], 0.25, 0.001);
    TEST_NEAR(fraction[2], 0.25, 0.001);
    TEST_NEAR(fraction[3], 0.25, 0.001);
    TEST_NEAR(stats.min(), 10, 0.01);
    TEST_NEAR(stats.max(), 100'000, 100);

}

void test_rs_sci_random_normal_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    NormalDistribution<double> norm;
    Statistics<double> stats;
    double x = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(x = norm(rng));
        stats(x);
    }

    TEST_NEAR(stats.mean(), 0, 0.001);
    TEST_NEAR(stats.sd(), 1, 0.001);

    TRY(norm = NormalDistribution<double>(100, 50));
    stats.clear();

    for (int i = 0; i < iterations; ++i) {
        TRY(x = norm(rng));
        stats(x);
    }

    TEST_NEAR(stats.mean(), 100, 0.1);
    TEST_NEAR(stats.sd(), 50, 0.1);

}

void test_rs_sci_random_log_normal_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    LogNormal<double> dist;
    Statistics<double> stats;
    double x = 0;
    double lx = 0;

    TEST_EQUAL(dist.m(), 0);
    TEST_EQUAL(dist.s(), 1);
    TEST_EQUAL(dist.median(), 1);
    TEST_NEAR(dist.s_factor(), 2.718'282, 1e-6);

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        lx = std::log(x);
        stats(lx);
    }

    TEST_NEAR(stats.mean(), 0, 0.001);
    TEST_NEAR(stats.sd(), 1, 0.001);

    dist = LogNormal<double>(2, 1, LogMode::common);
    stats.clear();

    TEST_NEAR(dist.m(), 4.605'170, 1e-6);
    TEST_NEAR(dist.s(), 2.302'585, 1e-6);
    TEST_NEAR(dist.median(), 100, 1e-6);
    TEST_NEAR(dist.s_factor(), 10, 1e-6);

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        lx = std::log(x);
        stats(lx);
    }

    TEST_NEAR(stats.mean(), 4.605, 0.01);
    TEST_NEAR(stats.sd(), 2.303, 0.01);

}
