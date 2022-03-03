#include "rs-sci/approx.hpp"
#include "rs-unit-test.hpp"

using namespace RS::Sci;

void test_rs_sci_approx_format_paired() {

    // TODO

}

void test_rs_sci_approx_format_ulps() {

    Approx a;

    TRY((a = {0.00012345,  0.00000067}));  TEST_EQUAL(a.str("dq"), "0.00012345(67)");
    TRY((a = {0.0012345,   0.0000067}));   TEST_EQUAL(a.str("dq"), "0.0012345(67)");
    TRY((a = {0.012345,    0.000067}));    TEST_EQUAL(a.str("dq"), "0.012345(67)");
    TRY((a = {0.12345,     0.00067}));     TEST_EQUAL(a.str("dq"), "0.12345(67)");
    TRY((a = {1.2345,      0.0067}));      TEST_EQUAL(a.str("dq"), "1.2345(67)");
    TRY((a = {12.345,      0.067}));       TEST_EQUAL(a.str("dq"), "12.345(67)");
    TRY((a = {123.45,      0.67}));        TEST_EQUAL(a.str("dq"), "123.45(67)");
    TRY((a = {1234.5,      6.7}));         TEST_EQUAL(a.str("dq"), "1234.5(67)");
    TRY((a = {12345,       67}));          TEST_EQUAL(a.str("dq"), "12345(67)");
    // TRY((a = {123450,      670}));         TEST_EQUAL(a.str("dq"), "123450(67)");
    // TRY((a = {1234500,     6700}));        TEST_EQUAL(a.str("dq"), "1234500(67)");

    TRY((a = {0.00012345,  0.00000067}));  TEST_EQUAL(a.str("eq"), "1.2345(67)e-4");
    TRY((a = {0.0012345,   0.0000067}));   TEST_EQUAL(a.str("eq"), "1.2345(67)e-3");
    TRY((a = {0.012345,    0.000067}));    TEST_EQUAL(a.str("eq"), "1.2345(67)e-2");
    TRY((a = {0.12345,     0.00067}));     TEST_EQUAL(a.str("eq"), "1.2345(67)e-1");
    TRY((a = {1.2345,      0.0067}));      TEST_EQUAL(a.str("eq"), "1.2345(67)e0");
    TRY((a = {12.345,      0.067}));       TEST_EQUAL(a.str("eq"), "1.2345(67)e1");
    TRY((a = {123.45,      0.67}));        TEST_EQUAL(a.str("eq"), "1.2345(67)e2");
    TRY((a = {1234.5,      6.7}));         TEST_EQUAL(a.str("eq"), "1.2345(67)e3");
    TRY((a = {12345,       67}));          TEST_EQUAL(a.str("eq"), "1.2345(67)e4");
    TRY((a = {123450,      670}));         TEST_EQUAL(a.str("eq"), "1.2345(67)e5");
    TRY((a = {1234500,     6700}));        TEST_EQUAL(a.str("eq"), "1.2345(67)e6");

    TRY((a = {0.00012345,  0.00000067}));  TEST_EQUAL(a.str("gq"), "1.2345(67)e-4");
    TRY((a = {0.0012345,   0.0000067}));   TEST_EQUAL(a.str("gq"), "0.0012345(67)");
    TRY((a = {0.012345,    0.000067}));    TEST_EQUAL(a.str("gq"), "0.012345(67)");
    TRY((a = {0.12345,     0.00067}));     TEST_EQUAL(a.str("gq"), "0.12345(67)");
    TRY((a = {1.2345,      0.0067}));      TEST_EQUAL(a.str("gq"), "1.2345(67)");
    TRY((a = {12.345,      0.067}));       TEST_EQUAL(a.str("gq"), "12.345(67)");
    TRY((a = {123.45,      0.67}));        TEST_EQUAL(a.str("gq"), "123.45(67)");
    TRY((a = {1234.5,      6.7}));         TEST_EQUAL(a.str("gq"), "1234.5(67)");
    TRY((a = {12345,       67}));          TEST_EQUAL(a.str("gq"), "12345(67)");
    // TRY((a = {123450,      670}));         TEST_EQUAL(a.str("gq"), "123450(67)");
    TRY((a = {1234500,     6700}));        TEST_EQUAL(a.str("gq"), "1.2345(67)e6");

    // TODO

}

void test_rs_sci_approx_format_decimal() {

    // TODO

}
