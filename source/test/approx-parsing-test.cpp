#include "rs-sci/approx.hpp"
#include "rs-unit-test.hpp"

using namespace RS::Sci;

void test_rs_sci_approx_parsing() {

    Approx a;

    TRY(a = Approx(".0000"));                TEST_EQUAL(a.value(), 0);            TEST_EQUAL(a.error(), 0);
    TRY(a = Approx("0.000"));                TEST_EQUAL(a.value(), 0);            TEST_EQUAL(a.error(), 0);
    TRY(a = Approx("00.00"));                TEST_EQUAL(a.value(), 0);            TEST_EQUAL(a.error(), 0);
    TRY(a = Approx("000.0"));                TEST_EQUAL(a.value(), 0);            TEST_EQUAL(a.error(), 0);
    TRY(a = Approx("0000."));                TEST_EQUAL(a.value(), 0);            TEST_EQUAL(a.error(), 0);
    TRY(a = Approx("0000"));                 TEST_EQUAL(a.value(), 0);            TEST_EQUAL(a.error(), 0);
    TRY(a = Approx(".1234e10+/-.0056e10"));  TEST_EQUAL(a.value(), 1.234e9);      TEST_EQUAL(a.error(), 5.6e7);
    TRY(a = Approx("1.234e10+/-0.056e10"));  TEST_EQUAL(a.value(), 1.234e10);     TEST_EQUAL(a.error(), 5.6e8);
    TRY(a = Approx("12.34e10+/-00.56e10"));  TEST_EQUAL(a.value(), 1.234e11);     TEST_EQUAL(a.error(), 5.6e9);
    TRY(a = Approx("123.4e10+/-005.6e10"));  TEST_EQUAL(a.value(), 1.234e12);     TEST_EQUAL(a.error(), 5.6e10);
    TRY(a = Approx("1234.e10+/-0056.e10"));  TEST_EQUAL(a.value(), 1.234e13);     TEST_EQUAL(a.error(), 5.6e11);
    TRY(a = Approx("1234e10+/-0056e10"));    TEST_EQUAL(a.value(), 1.234e13);     TEST_EQUAL(a.error(), 5.6e11);
    TRY(a = Approx(".1234+/-.0056"));        TEST_NEAR(a.value(), 0.1234, 1e-6);  TEST_NEAR(a.error(), 0.0056, 1e-6);
    TRY(a = Approx("1.234+/-0.056"));        TEST_NEAR(a.value(), 1.234, 1e-6);   TEST_NEAR(a.error(), 0.056, 1e-6);
    TRY(a = Approx("12.34+/-00.56"));        TEST_NEAR(a.value(), 12.34, 1e-6);   TEST_NEAR(a.error(), 0.56, 1e-6);
    TRY(a = Approx("123.4+/-005.6"));        TEST_NEAR(a.value(), 123.4, 1e-6);   TEST_NEAR(a.error(), 5.6, 1e-6);
    TRY(a = Approx("1234.+/-0056."));        TEST_EQUAL(a.value(), 1234);         TEST_EQUAL(a.error(), 56);
    TRY(a = Approx("1234+/-0056"));          TEST_EQUAL(a.value(), 1234);         TEST_EQUAL(a.error(), 56);
    TRY(a = Approx(".1234e10±.0056e10"));    TEST_EQUAL(a.value(), 1.234e9);      TEST_EQUAL(a.error(), 5.6e7);
    TRY(a = Approx("1.234e10±0.056e10"));    TEST_EQUAL(a.value(), 1.234e10);     TEST_EQUAL(a.error(), 5.6e8);
    TRY(a = Approx("12.34e10±00.56e10"));    TEST_EQUAL(a.value(), 1.234e11);     TEST_EQUAL(a.error(), 5.6e9);
    TRY(a = Approx("123.4e10±005.6e10"));    TEST_EQUAL(a.value(), 1.234e12);     TEST_EQUAL(a.error(), 5.6e10);
    TRY(a = Approx("1234.e10±0056.e10"));    TEST_EQUAL(a.value(), 1.234e13);     TEST_EQUAL(a.error(), 5.6e11);
    TRY(a = Approx("1234e10±0056e10"));      TEST_EQUAL(a.value(), 1.234e13);     TEST_EQUAL(a.error(), 5.6e11);
    TRY(a = Approx(".1234±.0056"));          TEST_NEAR(a.value(), 0.1234, 1e-6);  TEST_NEAR(a.error(), 0.0056, 1e-6);
    TRY(a = Approx("1.234±0.056"));          TEST_NEAR(a.value(), 1.234, 1e-6);   TEST_NEAR(a.error(), 0.056, 1e-6);
    TRY(a = Approx("12.34±00.56"));          TEST_NEAR(a.value(), 12.34, 1e-6);   TEST_NEAR(a.error(), 0.56, 1e-6);
    TRY(a = Approx("123.4±005.6"));          TEST_NEAR(a.value(), 123.4, 1e-6);   TEST_NEAR(a.error(), 5.6, 1e-6);
    TRY(a = Approx("1234.±0056."));          TEST_EQUAL(a.value(), 1234);         TEST_EQUAL(a.error(), 56);
    TRY(a = Approx("1234±0056"));            TEST_EQUAL(a.value(), 1234);         TEST_EQUAL(a.error(), 56);
    TRY(a = Approx(".1234(56)e10"));         TEST_EQUAL(a.value(), 1.234e9);      TEST_EQUAL(a.error(), 5.6e7);
    TRY(a = Approx("1.234(56)e10"));         TEST_EQUAL(a.value(), 1.234e10);     TEST_EQUAL(a.error(), 5.6e8);
    TRY(a = Approx("12.34(56)e10"));         TEST_EQUAL(a.value(), 1.234e11);     TEST_EQUAL(a.error(), 5.6e9);
    TRY(a = Approx("123.4(56)e10"));         TEST_EQUAL(a.value(), 1.234e12);     TEST_EQUAL(a.error(), 5.6e10);
    TRY(a = Approx("1234.(56)e10"));         TEST_EQUAL(a.value(), 1.234e13);     TEST_EQUAL(a.error(), 5.6e11);
    TRY(a = Approx("1234(56)e10"));          TEST_EQUAL(a.value(), 1.234e13);     TEST_EQUAL(a.error(), 5.6e11);
    TRY(a = Approx(".1234(56)"));            TEST_NEAR(a.value(), 0.1234, 1e-6);  TEST_NEAR(a.error(), 0.0056, 1e-6);
    TRY(a = Approx("1.234(56)"));            TEST_NEAR(a.value(), 1.234, 1e-6);   TEST_NEAR(a.error(), 0.056, 1e-6);
    TRY(a = Approx("12.34(56)"));            TEST_NEAR(a.value(), 12.34, 1e-6);   TEST_NEAR(a.error(), 0.56, 1e-6);
    TRY(a = Approx("123.4(56)"));            TEST_NEAR(a.value(), 123.4, 1e-6);   TEST_NEAR(a.error(), 5.6, 1e-6);
    TRY(a = Approx("1234.(56)"));            TEST_EQUAL(a.value(), 1234);         TEST_EQUAL(a.error(), 56);
    TRY(a = Approx("1234(56)"));             TEST_EQUAL(a.value(), 1234);         TEST_EQUAL(a.error(), 56);
    TRY(a = Approx(".1234e10"));             TEST_EQUAL(a.value(), 1.234e9);      TEST_EQUAL(a.error(), 5e5);
    TRY(a = Approx("1.234e10"));             TEST_EQUAL(a.value(), 1.234e10);     TEST_EQUAL(a.error(), 5e6);
    TRY(a = Approx("12.34e10"));             TEST_EQUAL(a.value(), 1.234e11);     TEST_EQUAL(a.error(), 5e7);
    TRY(a = Approx("123.4e10"));             TEST_EQUAL(a.value(), 1.234e12);     TEST_EQUAL(a.error(), 5e8);
    TRY(a = Approx("1234.e10"));             TEST_EQUAL(a.value(), 1.234e13);     TEST_EQUAL(a.error(), 5e9);
    TRY(a = Approx("1234e10"));              TEST_EQUAL(a.value(), 1.234e13);     TEST_EQUAL(a.error(), 5e9);
    TRY(a = Approx(".1234"));                TEST_NEAR(a.value(), 0.1234, 1e-6);  TEST_NEAR(a.error(), 0.00005, 1e-6);
    TRY(a = Approx("1.234"));                TEST_NEAR(a.value(), 1.234, 1e-6);   TEST_NEAR(a.error(), 0.0005, 1e-6);
    TRY(a = Approx("12.34"));                TEST_NEAR(a.value(), 12.34, 1e-6);   TEST_NEAR(a.error(), 0.005, 1e-6);
    TRY(a = Approx("123.4"));                TEST_NEAR(a.value(), 123.4, 1e-6);   TEST_NEAR(a.error(), 0.05, 1e-6);
    TRY(a = Approx("1234."));                TEST_EQUAL(a.value(), 1234);         TEST_NEAR(a.error(), 0.5, 1e-6);
    TRY(a = Approx("1234"));                 TEST_EQUAL(a.value(), 1234);         TEST_NEAR(a.error(), 0.5, 1e-6);

}
