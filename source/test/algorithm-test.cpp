#include "rs-sci/algorithm.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-unit-test.hpp"
#include <algorithm>
#include <functional>
#include <random>
#include <string>
#include <utility>
#include <vector>

using namespace RS::Graphics::Core;
using namespace RS::Sci;
using namespace std::literals;

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

void test_rs_sci_algorithm_integer_power() {

    TEST_EQUAL(integer_power(0ll, 0ll),    1ll);
    TEST_EQUAL(integer_power(0ll, 1ll),    0ll);
    TEST_EQUAL(integer_power(0ll, 2ll),    0ll);
    TEST_EQUAL(integer_power(0ll, 3ll),    0ll);
    TEST_EQUAL(integer_power(0ll, 4ll),    0ll);
    TEST_EQUAL(integer_power(0ll, 5ll),    0ll);
    TEST_EQUAL(integer_power(0ll, 10ll),   0ll);
    TEST_EQUAL(integer_power(0ll, 15ll),   0ll);
    TEST_EQUAL(integer_power(0ll, 20ll),   0ll);
    TEST_EQUAL(integer_power(1ll, 0ll),    1ll);
    TEST_EQUAL(integer_power(1ll, 1ll),    1ll);
    TEST_EQUAL(integer_power(1ll, 2ll),    1ll);
    TEST_EQUAL(integer_power(1ll, 3ll),    1ll);
    TEST_EQUAL(integer_power(1ll, 4ll),    1ll);
    TEST_EQUAL(integer_power(1ll, 5ll),    1ll);
    TEST_EQUAL(integer_power(1ll, 10ll),   1ll);
    TEST_EQUAL(integer_power(1ll, 15ll),   1ll);
    TEST_EQUAL(integer_power(1ll, 20ll),   1ll);
    TEST_EQUAL(integer_power(2ll, 0ll),    1ll);
    TEST_EQUAL(integer_power(2ll, 1ll),    2ll);
    TEST_EQUAL(integer_power(2ll, 2ll),    4ll);
    TEST_EQUAL(integer_power(2ll, 3ll),    8ll);
    TEST_EQUAL(integer_power(2ll, 4ll),    16ll);
    TEST_EQUAL(integer_power(2ll, 5ll),    32ll);
    TEST_EQUAL(integer_power(2ll, 10ll),   1'024ll);
    TEST_EQUAL(integer_power(2ll, 15ll),   32'768ll);
    TEST_EQUAL(integer_power(2ll, 20ll),   1'048'576ll);
    TEST_EQUAL(integer_power(5ll, 0ll),    1ll);
    TEST_EQUAL(integer_power(5ll, 1ll),    5ll);
    TEST_EQUAL(integer_power(5ll, 2ll),    25ll);
    TEST_EQUAL(integer_power(5ll, 3ll),    125ll);
    TEST_EQUAL(integer_power(5ll, 4ll),    625ll);
    TEST_EQUAL(integer_power(5ll, 5ll),    3'125ll);
    TEST_EQUAL(integer_power(5ll, 10ll),   9'765'625ll);
    TEST_EQUAL(integer_power(5ll, 15ll),   30'517'578'125ll);
    TEST_EQUAL(integer_power(5ll, 20ll),   95'367'431'640'625ll);
    TEST_EQUAL(integer_power(-1ll, 0ll),   1ll);
    TEST_EQUAL(integer_power(-1ll, 1ll),   -1ll);
    TEST_EQUAL(integer_power(-1ll, 2ll),   1ll);
    TEST_EQUAL(integer_power(-1ll, 3ll),   -1ll);
    TEST_EQUAL(integer_power(-1ll, 4ll),   1ll);
    TEST_EQUAL(integer_power(-1ll, 5ll),   -1ll);
    TEST_EQUAL(integer_power(-1ll, 10ll),  1ll);
    TEST_EQUAL(integer_power(-1ll, 15ll),  -1ll);
    TEST_EQUAL(integer_power(-1ll, 20ll),  1ll);
    TEST_EQUAL(integer_power(-2ll, 0ll),   1ll);
    TEST_EQUAL(integer_power(-2ll, 1ll),   -2ll);
    TEST_EQUAL(integer_power(-2ll, 2ll),   4ll);
    TEST_EQUAL(integer_power(-2ll, 3ll),   -8ll);
    TEST_EQUAL(integer_power(-2ll, 4ll),   16ll);
    TEST_EQUAL(integer_power(-2ll, 5ll),   -32ll);
    TEST_EQUAL(integer_power(-2ll, 10ll),  1'024ll);
    TEST_EQUAL(integer_power(-2ll, 15ll),  -32'768ll);
    TEST_EQUAL(integer_power(-2ll, 20ll),  1'048'576ll);
    TEST_EQUAL(integer_power(-5ll, 0ll),   1ll);
    TEST_EQUAL(integer_power(-5ll, 1ll),   -5ll);
    TEST_EQUAL(integer_power(-5ll, 2ll),   25ll);
    TEST_EQUAL(integer_power(-5ll, 3ll),   -125ll);
    TEST_EQUAL(integer_power(-5ll, 4ll),   625ll);
    TEST_EQUAL(integer_power(-5ll, 5ll),   -3'125ll);
    TEST_EQUAL(integer_power(-5ll, 10ll),  9'765'625ll);
    TEST_EQUAL(integer_power(-5ll, 15ll),  -30'517'578'125ll);
    TEST_EQUAL(integer_power(-5ll, 20ll),  95'367'431'640'625ll);

}

void test_rs_sci_algorithm_integer_power_generalised() {

    static const auto f = std::plus<std::string>();

    TEST_EQUAL(integer_power("A"s, 0, f, ""s),  "");
    TEST_EQUAL(integer_power("A"s, 1, f, ""s),  "A");
    TEST_EQUAL(integer_power("A"s, 2, f, ""s),  "AA");
    TEST_EQUAL(integer_power("A"s, 3, f, ""s),  "AAA");
    TEST_EQUAL(integer_power("A"s, 4, f, ""s),  "AAAA");
    TEST_EQUAL(integer_power("A"s, 5, f, ""s),  "AAAAA");

}

void test_rs_sci_algorithm_interpolation() {

    TEST_EQUAL(interpolate(1.0, 10.0, 2.0, 20.0, 0.75),                  7.5);
    TEST_EQUAL(interpolate(1.0, 10.0, 2.0, 20.0, 1.00),                  10.0);
    TEST_EQUAL(interpolate(1.0, 10.0, 2.0, 20.0, 1.25),                  12.5);
    TEST_EQUAL(interpolate(1.0, 10.0, 2.0, 20.0, 1.50),                  15.0);
    TEST_EQUAL(interpolate(1.0, 10.0, 2.0, 20.0, 1.75),                  17.5);
    TEST_EQUAL(interpolate(1.0, 10.0, 2.0, 20.0, 2.00),                  20.0);
    TEST_EQUAL(interpolate(1.0, 10.0, 2.0, 20.0, 2.25),                  22.5);
    TEST_NEAR(interpolate(2.0, 10.0, 32.0, 20.0, 1.0, log_x),            7.5,    1e-6);
    TEST_NEAR(interpolate(2.0, 10.0, 32.0, 20.0, 2.0, log_x),            10.0,   1e-6);
    TEST_NEAR(interpolate(2.0, 10.0, 32.0, 20.0, 4.0, log_x),            12.5,   1e-6);
    TEST_NEAR(interpolate(2.0, 10.0, 32.0, 20.0, 8.0, log_x),            15.0,   1e-6);
    TEST_NEAR(interpolate(2.0, 10.0, 32.0, 20.0, 16.0, log_x),           17.5,   1e-6);
    TEST_NEAR(interpolate(2.0, 10.0, 32.0, 20.0, 32.0, log_x),           20.0,   1e-6);
    TEST_NEAR(interpolate(2.0, 10.0, 32.0, 20.0, 64.0, log_x),           22.5,   1e-6);
    TEST_NEAR(interpolate(1.0, 20.0, 2.0, 320.0, 0.75, log_y),           10.0,   1e-6);
    TEST_NEAR(interpolate(1.0, 20.0, 2.0, 320.0, 1.00, log_y),           20.0,   1e-6);
    TEST_NEAR(interpolate(1.0, 20.0, 2.0, 320.0, 1.25, log_y),           40.0,   1e-6);
    TEST_NEAR(interpolate(1.0, 20.0, 2.0, 320.0, 1.50, log_y),           80.0,   1e-6);
    TEST_NEAR(interpolate(1.0, 20.0, 2.0, 320.0, 1.75, log_y),           160.0,  1e-6);
    TEST_NEAR(interpolate(1.0, 20.0, 2.0, 320.0, 2.00, log_y),           320.0,  1e-6);
    TEST_NEAR(interpolate(1.0, 20.0, 2.0, 320.0, 2.25, log_y),           640.0,  1e-6);
    TEST_NEAR(interpolate(2.0, 20.0, 32.0, 320.0, 1.0, log_x | log_y),   10.0,   1e-6);
    TEST_NEAR(interpolate(2.0, 20.0, 32.0, 320.0, 2.0, log_x | log_y),   20.0,   1e-6);
    TEST_NEAR(interpolate(2.0, 20.0, 32.0, 320.0, 4.0, log_x | log_y),   40.0,   1e-6);
    TEST_NEAR(interpolate(2.0, 20.0, 32.0, 320.0, 8.0, log_x | log_y),   80.0,   1e-6);
    TEST_NEAR(interpolate(2.0, 20.0, 32.0, 320.0, 16.0, log_x | log_y),  160.0,  1e-6);
    TEST_NEAR(interpolate(2.0, 20.0, 32.0, 320.0, 32.0, log_x | log_y),  320.0,  1e-6);
    TEST_NEAR(interpolate(2.0, 20.0, 32.0, 320.0, 64.0, log_x | log_y),  640.0,  1e-6);

}

void test_rs_sci_algorithm_interpolated_map() {

    InterpolatedMap<double> map;
    double y = 0;

    TRY(map.insert(10, 100));
    TRY(map.insert(20, 150));
    TRY(map.insert(30, 50));
    TRY(map.insert(60, 110));

    TRY(y = map(0));   TEST_EQUAL(y, 50);
    TRY(y = map(5));   TEST_EQUAL(y, 75);
    TRY(y = map(10));  TEST_EQUAL(y, 100);
    TRY(y = map(15));  TEST_EQUAL(y, 125);
    TRY(y = map(20));  TEST_EQUAL(y, 150);
    TRY(y = map(25));  TEST_EQUAL(y, 100);
    TRY(y = map(30));  TEST_EQUAL(y, 50);
    TRY(y = map(35));  TEST_EQUAL(y, 60);
    TRY(y = map(40));  TEST_EQUAL(y, 70);
    TRY(y = map(45));  TEST_EQUAL(y, 80);
    TRY(y = map(50));  TEST_EQUAL(y, 90);
    TRY(y = map(55));  TEST_EQUAL(y, 100);
    TRY(y = map(60));  TEST_EQUAL(y, 110);
    TRY(y = map(65));  TEST_EQUAL(y, 120);
    TRY(y = map(70));  TEST_EQUAL(y, 130);

    TRY((map = {
        { 100, 1000 },
        { 200, 1500 },
        { 300, 500 },
        { 600, 1100 },
    }));

    TRY(y = map(0));    TEST_EQUAL(y, 500);
    TRY(y = map(50));   TEST_EQUAL(y, 750);
    TRY(y = map(100));  TEST_EQUAL(y, 1000);
    TRY(y = map(150));  TEST_EQUAL(y, 1250);
    TRY(y = map(200));  TEST_EQUAL(y, 1500);
    TRY(y = map(250));  TEST_EQUAL(y, 1000);
    TRY(y = map(300));  TEST_EQUAL(y, 500);
    TRY(y = map(350));  TEST_EQUAL(y, 600);
    TRY(y = map(400));  TEST_EQUAL(y, 700);
    TRY(y = map(450));  TEST_EQUAL(y, 800);
    TRY(y = map(500));  TEST_EQUAL(y, 900);
    TRY(y = map(550));  TEST_EQUAL(y, 1000);
    TRY(y = map(600));  TEST_EQUAL(y, 1100);
    TRY(y = map(650));  TEST_EQUAL(y, 1200);
    TRY(y = map(700));  TEST_EQUAL(y, 1300);

}

void test_rs_sci_algorithm_interpolated_map_lin_log() {

    InterpolatedMap<double, log_y> map;
    double y = 0;

    TRY((map = {
        { 10, 1e20 },
        { 20, 1e30 },
        { 30, 1e10 },
        { 60, 1e22 },
    }));

    TRY(y = map(0));   TEST_NEAR(y, 1e10, 1);
    TRY(y = map(5));   TEST_NEAR(y, 1e15, 1e5);
    TRY(y = map(10));  TEST_NEAR(y, 1e20, 1e10);
    TRY(y = map(15));  TEST_NEAR(y, 1e25, 1e15);
    TRY(y = map(20));  TEST_NEAR(y, 1e30, 1e20);
    TRY(y = map(25));  TEST_NEAR(y, 1e20, 1e10);
    TRY(y = map(30));  TEST_NEAR(y, 1e10, 1);
    TRY(y = map(35));  TEST_NEAR(y, 1e12, 1e2);
    TRY(y = map(40));  TEST_NEAR(y, 1e14, 1e4);
    TRY(y = map(45));  TEST_NEAR(y, 1e16, 1e6);
    TRY(y = map(50));  TEST_NEAR(y, 1e18, 1e8);
    TRY(y = map(55));  TEST_NEAR(y, 1e20, 1e10);
    TRY(y = map(60));  TEST_NEAR(y, 1e22, 1e12);
    TRY(y = map(65));  TEST_NEAR(y, 1e24, 1e14);
    TRY(y = map(70));  TEST_NEAR(y, 1e26, 1e16);

}

void test_rs_sci_algorithm_interpolated_map_log_log() {

    InterpolatedMap<double, log_x | log_y> map;
    double y = 0;

    TRY(map.insert(4, 1e20));
    TRY(map.insert(16, 1e30));
    TRY(map.insert(64, 1e10));
    TRY(map.insert(4096, 1e22));

    TRY(y = map(1));      TEST_NEAR(y, 1e10, 1);
    TRY(y = map(2));      TEST_NEAR(y, 1e15, 1e5);
    TRY(y = map(4));      TEST_NEAR(y, 1e20, 1e10);
    TRY(y = map(8));      TEST_NEAR(y, 1e25, 1e15);
    TRY(y = map(16));     TEST_NEAR(y, 1e30, 1e20);
    TRY(y = map(32));     TEST_NEAR(y, 1e20, 1e10);
    TRY(y = map(64));     TEST_NEAR(y, 1e10, 1);
    TRY(y = map(128));    TEST_NEAR(y, 1e12, 1e2);
    TRY(y = map(256));    TEST_NEAR(y, 1e14, 1e4);
    TRY(y = map(512));    TEST_NEAR(y, 1e16, 1e6);
    TRY(y = map(1024));   TEST_NEAR(y, 1e18, 1e8);
    TRY(y = map(2048));   TEST_NEAR(y, 1e20, 1e10);
    TRY(y = map(4096));   TEST_NEAR(y, 1e22, 1e12);
    TRY(y = map(8192));   TEST_NEAR(y, 1e24, 1e14);
    TRY(y = map(16384));  TEST_NEAR(y, 1e26, 1e16);

}

void test_rs_sci_algorithm_cubic_spline_map() {

    using cubic = CubicSplineMap<double>;

    cubic c;
    std::vector<std::pair<double, double>> points;
    double y = 0;

    points = {
        { 12, 5 },
        { 34, 25 },
        { 60, 8 },
        { 98, 28 },
    };

    TRY(c = cubic(points));

    TRY(y = c(1));    TEST_NEAR(y, -8.6066, 1e-4);
    TRY(y = c(12));   TEST_NEAR(y, 5.0000, 1e-4);
    TRY(y = c(23));   TEST_NEAR(y, 18.6066, 1e-4);
    TRY(y = c(34));   TEST_NEAR(y, 25.0000, 1e-4);
    TRY(y = c(47));   TEST_NEAR(y, 18.1768, 1e-4);
    TRY(y = c(60));   TEST_NEAR(y, 8.0000, 1e-4);
    TRY(y = c(79));   TEST_NEAR(y, 10.8217, 1e-4);
    TRY(y = c(98));   TEST_NEAR(y, 28.0000, 1e-4);
    TRY(y = c(117));  TEST_NEAR(y, 45.1783, 1e-4);

    points = {
        { 12, 5 },
        { 34, 25 },
        { 60, 8 },
        { 98, 28 },
        { 102, 28 },
        { 140, 8 },
        { 166, 25 },
        { 188, 5 },
    };

    TRY(c = cubic(points));

    TRY(y = c(1));    TEST_NEAR(y, -8.8157, 1e-4);
    TRY(y = c(12));   TEST_NEAR(y, 5.0000, 1e-4);
    TRY(y = c(23));   TEST_NEAR(y, 18.8157, 1e-4);
    TRY(y = c(34));   TEST_NEAR(y, 25.0000, 1e-4);
    TRY(y = c(47));   TEST_NEAR(y, 17.3904, 1e-4);
    TRY(y = c(60));   TEST_NEAR(y, 8.0000, 1e-4);
    TRY(y = c(79));   TEST_NEAR(y, 15.8511, 1e-4);
    TRY(y = c(98));   TEST_NEAR(y, 28.0000, 1e-4);
    TRY(y = c(100));  TEST_NEAR(y, 28.1625, 1e-4);
    TRY(y = c(102));  TEST_NEAR(y, 28.0000, 1e-4);
    TRY(y = c(121));  TEST_NEAR(y, 15.8511, 1e-4);
    TRY(y = c(140));  TEST_NEAR(y, 8.0000, 1e-4);
    TRY(y = c(153));  TEST_NEAR(y, 17.3904, 1e-4);
    TRY(y = c(166));  TEST_NEAR(y, 25.0000, 1e-4);
    TRY(y = c(177));  TEST_NEAR(y, 18.8157, 1e-4);
    TRY(y = c(188));  TEST_NEAR(y, 5.0000, 1e-4);
    TRY(y = c(199));  TEST_NEAR(y, -8.8157, 1e-4);

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
