#include "rs-sci/algorithms.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-unit-test.hpp"
#include <algorithm>
#include <functional>
#include <random>
#include <string>
#include <vector>

using namespace RS::Graphics::Core;
using namespace RS::Sci;

void test_rs_sci_algorithm_binomial_coefficients() {

    TEST_EQUAL(binomial(-1, -1),  0);
    TEST_EQUAL(binomial(-1, 0),   0);
    TEST_EQUAL(binomial(-1, 1),   0);
    TEST_EQUAL(binomial(0, -1),   0);
    TEST_EQUAL(binomial(0, 0),    1);
    TEST_EQUAL(binomial(0, 1),    0);
    TEST_EQUAL(binomial(1, -1),   0);
    TEST_EQUAL(binomial(1, 0),    1);
    TEST_EQUAL(binomial(1, 1),    1);
    TEST_EQUAL(binomial(1, 2),    0);
    TEST_EQUAL(binomial(2, -1),   0);
    TEST_EQUAL(binomial(2, 0),    1);
    TEST_EQUAL(binomial(2, 1),    2);
    TEST_EQUAL(binomial(2, 2),    1);
    TEST_EQUAL(binomial(2, 3),    0);
    TEST_EQUAL(binomial(3, -1),   0);
    TEST_EQUAL(binomial(3, 0),    1);
    TEST_EQUAL(binomial(3, 1),    3);
    TEST_EQUAL(binomial(3, 2),    3);
    TEST_EQUAL(binomial(3, 3),    1);
    TEST_EQUAL(binomial(3, 4),    0);
    TEST_EQUAL(binomial(4, -1),   0);
    TEST_EQUAL(binomial(4, 0),    1);
    TEST_EQUAL(binomial(4, 1),    4);
    TEST_EQUAL(binomial(4, 2),    6);
    TEST_EQUAL(binomial(4, 3),    4);
    TEST_EQUAL(binomial(4, 4),    1);
    TEST_EQUAL(binomial(4, 5),    0);
    TEST_EQUAL(binomial(5, -1),   0);
    TEST_EQUAL(binomial(5, 0),    1);
    TEST_EQUAL(binomial(5, 1),    5);
    TEST_EQUAL(binomial(5, 2),    10);
    TEST_EQUAL(binomial(5, 3),    10);
    TEST_EQUAL(binomial(5, 4),    5);
    TEST_EQUAL(binomial(5, 5),    1);
    TEST_EQUAL(binomial(5, 6),    0);

}

namespace {

    template <typename T, typename RNG>
    void precision_sum_test(size_t copies, size_t cycles, T highval, RNG& rng) {

        std::vector<T> values;
        values.reserve(4 * copies);

        for (size_t i = 0; i < copies; ++i) {
            values.push_back(1);
            values.push_back(highval);
            values.push_back(1);
            values.push_back(- highval);
        }

        PrecisionSum<T> psum;

        for (size_t i = 0; i < cycles; ++i) {
            std::shuffle(values.begin(), values.end(), rng);
            TRY(psum.clear());
            for (T t: values)
                TRY(psum.add(t));
            T sum = 0;
            TRY(sum = psum.get());
            TEST_EQUAL(sum, T(2 * copies));
        }

        for (size_t i = 0; i < cycles; ++i) {
            std::shuffle(values.begin(), values.end(), rng);
            T sum = 0;
            TRY(sum = precision_sum(values));
            TEST_EQUAL(sum, T(2 * copies));
        }

    }

}

void test_rs_sci_algorithm_precision_sum() {

    std::mt19937 rng(42);

    precision_sum_test<float>(1000, 100, 1e20f, rng);
    precision_sum_test<double>(1000, 100, 1e100, rng);

    // GCC doesn't provide strict IEEE mode for long double
    // (not even with -ffloat-store)

    #if ! defined(__GNUC__) || defined(__clang__)
        precision_sum_test<long double>(1000, 100, 1e100l, rng);
    #endif

}

void test_rs_sci_algorithm_line_integral() {

    double integral = 0;
    auto f = [] (auto x) { return x * x; };

    TRY((integral = line_integral(0.0, 10.0, 10, f)));
    TEST_EQUAL(integral, 335);

}

void test_rs_sci_algorithm_volume_integral() {

    double integral = 0;
    auto f = [] (auto x) { return x[0] * x[1]; };

    TRY((integral = volume_integral(Double2(0, 0), Double2(5, 5), 5, f)));
    TEST_EQUAL(integral, 156.25);

}

void test_rs_sci_algorithm_find_optimum() {

    std::vector<std::string> vec;
    auto i = vec.begin();
    auto str_size = [] (const std::string& s) { return s.size(); };

    TRY(i = find_optimum(vec, str_size));
    TEST(i == vec.end());

    vec = {"alpha"};
    TRY(i = find_optimum(vec, str_size));
    REQUIRE(i != vec.end());
    TEST_EQUAL(*i, "alpha");

    vec = {"alpha", "bravo", "charlie", "delta", "echo", "foxtrot", "golf", "hotel"};
    TRY(i = find_optimum(vec, str_size));
    REQUIRE(i != vec.end());
    TEST_EQUAL(*i, "charlie");
    TRY(i = find_optimum(vec, str_size, std::less<>()));
    REQUIRE(i != vec.end());
    TEST_EQUAL(*i, "echo");

}
