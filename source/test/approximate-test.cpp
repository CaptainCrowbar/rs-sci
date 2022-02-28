#include "rs-sci/approximate.hpp"
#include "rs-unit-test.hpp"
#include <string>

using namespace RS::Sci;

void test_rs_sci_approx_initialization() {

    Approx a;

    TEST_EQUAL(a.value(), 0);
    TEST_EQUAL(a.error(), 0);
    TEST(a.is_exact());

    TRY(a = 10);
    TEST_EQUAL(a.value(), 10);
    TEST_EQUAL(a.error(), 0);
    TEST(a.is_exact());

    TRY(a = Approx(10, 0.25));
    TEST_EQUAL(a.value(), 10);
    TEST_EQUAL(a.error(), 0.25);
    TEST(! a.is_exact());

    TRY(a = Approx(10, 0.25, Approx::err::abs));
    TEST_EQUAL(a.value(), 10);
    TEST_EQUAL(a.error(), 0.25);
    TEST(! a.is_exact());

    TRY(a = Approx(10, 0.25, Approx::err::rel));
    TEST_EQUAL(a.value(), 10);
    TEST_EQUAL(a.error(), 2.5);
    TEST(! a.is_exact());

    // TODO

}
