#include "rs-sci/algorithms.hpp"
#include "rs-unit-test.hpp"
#include <functional>
#include <string>
#include <vector>

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
