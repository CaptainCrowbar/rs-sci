#include "rs-sci/mp-integer.hpp"
#include "rs-sci/rational.hpp"
#include "rs-unit-test.hpp"
#include <stdexcept>

using namespace RS::Sci;
using namespace RS::Sci::Literals;

void test_rs_sci_mp_integer_rational_basics() {

    Mpratio r;

    TEST_EQUAL(r.num(), 0);
    TEST_EQUAL(r.den(), 1);
    TEST(! bool(r));
    TEST(! r);
    TEST_EQUAL(int(r), 0);
    TEST_EQUAL(double(r), 0);
    TEST(r.is_integer());
    TEST_EQUAL(r.whole(), 0);
    TEST_EQUAL(r.frac().num(), 0);
    TEST_EQUAL(r.frac().den(), 1);
    TEST_EQUAL(r.str(), "0");
    TEST_EQUAL(r.vulgar(), "0/1");
    TEST_EQUAL(r.mixed(), "0");

    TRY(r = Mpratio(5, 3));
    TEST_EQUAL(r.num(), 5);
    TEST_EQUAL(r.den(), 3);
    TEST_EQUAL(int(r), 1);
    TEST_NEAR(double(r), 1.666667, 1e-6);
    TEST(! r.is_integer());
    TEST_EQUAL(r.whole(), 1);
    TEST_EQUAL(r.frac().num(), 2);
    TEST_EQUAL(r.frac().den(), 3);
    TEST_EQUAL(r.str(), "5/3");
    TEST_EQUAL(r.mixed(), "1 2/3");
    TEST_EQUAL(r.vulgar(), "5/3");

    TRY(r = Mpratio(-7, 9));
    TEST_EQUAL(r.num(), -7);
    TEST_EQUAL(r.den(), 9);
    TEST_EQUAL(int(r), -1);
    TEST_NEAR(double(r), -0.777778, 1e-6);
    TEST(! r.is_integer());
    TEST_EQUAL(r.whole(), 0);
    TEST_EQUAL(r.frac().num(), -7);
    TEST_EQUAL(r.frac().den(), 9);
    TEST_EQUAL(r.str(), "-7/9");
    TEST_EQUAL(r.mixed(), "-7/9");
    TEST_EQUAL(r.vulgar(), "-7/9");

}

void test_rs_sci_mp_integer_rational_reduction() {

    Mpratio r;

    TRY(r = Mpratio(0, 6));    TEST_EQUAL(r.num(), 0);    TEST_EQUAL(r.den(), 1);  TEST_EQUAL(r.whole(), 0);   TEST_EQUAL(r.frac(), Mpratio(0, 6));
    TRY(r = Mpratio(1, 6));    TEST_EQUAL(r.num(), 1);    TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), 0);   TEST_EQUAL(r.frac(), Mpratio(1, 6));
    TRY(r = Mpratio(2, 6));    TEST_EQUAL(r.num(), 1);    TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), 0);   TEST_EQUAL(r.frac(), Mpratio(2, 6));
    TRY(r = Mpratio(3, 6));    TEST_EQUAL(r.num(), 1);    TEST_EQUAL(r.den(), 2);  TEST_EQUAL(r.whole(), 0);   TEST_EQUAL(r.frac(), Mpratio(3, 6));
    TRY(r = Mpratio(4, 6));    TEST_EQUAL(r.num(), 2);    TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), 0);   TEST_EQUAL(r.frac(), Mpratio(4, 6));
    TRY(r = Mpratio(5, 6));    TEST_EQUAL(r.num(), 5);    TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), 0);   TEST_EQUAL(r.frac(), Mpratio(5, 6));
    TRY(r = Mpratio(6, 6));    TEST_EQUAL(r.num(), 1);    TEST_EQUAL(r.den(), 1);  TEST_EQUAL(r.whole(), 1);   TEST_EQUAL(r.frac(), Mpratio(0, 6));
    TRY(r = Mpratio(7, 6));    TEST_EQUAL(r.num(), 7);    TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), 1);   TEST_EQUAL(r.frac(), Mpratio(1, 6));
    TRY(r = Mpratio(8, 6));    TEST_EQUAL(r.num(), 4);    TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), 1);   TEST_EQUAL(r.frac(), Mpratio(2, 6));
    TRY(r = Mpratio(9, 6));    TEST_EQUAL(r.num(), 3);    TEST_EQUAL(r.den(), 2);  TEST_EQUAL(r.whole(), 1);   TEST_EQUAL(r.frac(), Mpratio(3, 6));
    TRY(r = Mpratio(10, 6));   TEST_EQUAL(r.num(), 5);    TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), 1);   TEST_EQUAL(r.frac(), Mpratio(4, 6));
    TRY(r = Mpratio(11, 6));   TEST_EQUAL(r.num(), 11);   TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), 1);   TEST_EQUAL(r.frac(), Mpratio(5, 6));
    TRY(r = Mpratio(12, 6));   TEST_EQUAL(r.num(), 2);    TEST_EQUAL(r.den(), 1);  TEST_EQUAL(r.whole(), 2);   TEST_EQUAL(r.frac(), Mpratio(0, 6));
    TRY(r = Mpratio(-1, 6));   TEST_EQUAL(r.num(), -1);   TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), -0);  TEST_EQUAL(r.frac(), Mpratio(-1, 6));
    TRY(r = Mpratio(-2, 6));   TEST_EQUAL(r.num(), -1);   TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), -0);  TEST_EQUAL(r.frac(), Mpratio(-2, 6));
    TRY(r = Mpratio(-3, 6));   TEST_EQUAL(r.num(), -1);   TEST_EQUAL(r.den(), 2);  TEST_EQUAL(r.whole(), -0);  TEST_EQUAL(r.frac(), Mpratio(-3, 6));
    TRY(r = Mpratio(-4, 6));   TEST_EQUAL(r.num(), -2);   TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), -0);  TEST_EQUAL(r.frac(), Mpratio(-4, 6));
    TRY(r = Mpratio(-5, 6));   TEST_EQUAL(r.num(), -5);   TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), -0);  TEST_EQUAL(r.frac(), Mpratio(-5, 6));
    TRY(r = Mpratio(-6, 6));   TEST_EQUAL(r.num(), -1);   TEST_EQUAL(r.den(), 1);  TEST_EQUAL(r.whole(), -1);  TEST_EQUAL(r.frac(), Mpratio(-0, 6));
    TRY(r = Mpratio(-7, 6));   TEST_EQUAL(r.num(), -7);   TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), -1);  TEST_EQUAL(r.frac(), Mpratio(-1, 6));
    TRY(r = Mpratio(-8, 6));   TEST_EQUAL(r.num(), -4);   TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), -1);  TEST_EQUAL(r.frac(), Mpratio(-2, 6));
    TRY(r = Mpratio(-9, 6));   TEST_EQUAL(r.num(), -3);   TEST_EQUAL(r.den(), 2);  TEST_EQUAL(r.whole(), -1);  TEST_EQUAL(r.frac(), Mpratio(-3, 6));
    TRY(r = Mpratio(-10, 6));  TEST_EQUAL(r.num(), -5);   TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), -1);  TEST_EQUAL(r.frac(), Mpratio(-4, 6));
    TRY(r = Mpratio(-11, 6));  TEST_EQUAL(r.num(), -11);  TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), -1);  TEST_EQUAL(r.frac(), Mpratio(-5, 6));
    TRY(r = Mpratio(-12, 6));  TEST_EQUAL(r.num(), -2);   TEST_EQUAL(r.den(), 1);  TEST_EQUAL(r.whole(), -2);  TEST_EQUAL(r.frac(), Mpratio(-0, 6));

}

void test_rs_sci_mp_integer_rational_arithmetic() {

    Mpratio r, s, t;

    TRY(r = Mpratio(5, 3));
    TRY(s = Mpratio(7, 9));

    TRY(t = r + s);  TEST_EQUAL(t.num(), 22);  TEST_EQUAL(t.den(), 9);
    TRY(t = r - s);  TEST_EQUAL(t.num(), 8);   TEST_EQUAL(t.den(), 9);
    TRY(t = r * s);  TEST_EQUAL(t.num(), 35);  TEST_EQUAL(t.den(), 27);
    TRY(t = r / s);  TEST_EQUAL(t.num(), 15);  TEST_EQUAL(t.den(), 7);

}

void test_rs_sci_mp_integer_rational_properties() {

    Mpratio r, s, t;

    TRY(r = Mpratio(5, 3));
    TRY(s = Mpratio(-7, 9));

    TEST_EQUAL(r.abs(), Mpratio(5, 3));
    TEST_EQUAL(s.abs(), Mpratio(7, 9));
    TEST_EQUAL(t.abs(), Mpratio(0));

    TRY(r = Mpratio(-6, 3));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -2);  TEST_EQUAL(r.round(), -2);
    TRY(r = Mpratio(-5, 3));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -2);
    TRY(r = Mpratio(-4, 3));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -1);
    TRY(r = Mpratio(-3, 3));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -1);
    TRY(r = Mpratio(-2, 3));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), -1);
    TRY(r = Mpratio(-1, 3));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), 0);
    TRY(r = Mpratio(0, 3));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), 0);
    TRY(r = Mpratio(1, 3));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 0);
    TRY(r = Mpratio(2, 3));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 1);
    TRY(r = Mpratio(3, 3));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 1);
    TRY(r = Mpratio(4, 3));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 1);
    TRY(r = Mpratio(5, 3));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 2);
    TRY(r = Mpratio(6, 3));   TEST_EQUAL(r.floor(), 2);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 2);
    TRY(r = Mpratio(-8, 4));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -2);  TEST_EQUAL(r.round(), -2);
    TRY(r = Mpratio(-7, 4));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -2);
    TRY(r = Mpratio(-6, 4));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -1);
    TRY(r = Mpratio(-5, 4));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -1);
    TRY(r = Mpratio(-4, 4));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -1);
    TRY(r = Mpratio(-3, 4));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), -1);
    TRY(r = Mpratio(-2, 4));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), 0);
    TRY(r = Mpratio(-1, 4));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), 0);
    TRY(r = Mpratio(0, 4));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), 0);
    TRY(r = Mpratio(1, 4));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 0);
    TRY(r = Mpratio(2, 4));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 1);
    TRY(r = Mpratio(3, 4));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 1);
    TRY(r = Mpratio(4, 4));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 1);
    TRY(r = Mpratio(5, 4));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 1);
    TRY(r = Mpratio(6, 4));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 2);
    TRY(r = Mpratio(7, 4));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 2);
    TRY(r = Mpratio(8, 4));   TEST_EQUAL(r.floor(), 2);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 2);

}

void test_rs_sci_mp_integer_rational_comparison() {

    Mpratio r, s;

    TRY(r = Mpratio(5, 6));   TRY(s = Mpratio(7, 9));     TEST(r > s);   TEST(r >= s);  TEST(r != s);
    TRY(r = Mpratio(5, 6));   TRY(s = Mpratio(8, 9));     TEST(r < s);   TEST(r <= s);  TEST(r != s);
    TRY(r = Mpratio(5, 6));   TRY(s = Mpratio(10, 12));   TEST(r == s);  TEST(r <= s);  TEST(r >= s);
    TRY(r = Mpratio(-5, 6));  TRY(s = Mpratio(-7, 9));    TEST(r < s);   TEST(r <= s);  TEST(r != s);
    TRY(r = Mpratio(-5, 6));  TRY(s = Mpratio(-8, 9));    TEST(r > s);   TEST(r >= s);  TEST(r != s);
    TRY(r = Mpratio(-5, 6));  TRY(s = Mpratio(-10, 12));  TEST(r == s);  TEST(r <= s);  TEST(r >= s);

}

void test_rs_sci_mp_integer_rational_mixed() {

    Mpratio r;

    TRY(r = Mpint(42));    TEST_EQUAL(r.num(), 42);   TEST_EQUAL(r.den(), 1);
    TRY(r += Mpint(100));  TEST_EQUAL(r.num(), 142);  TEST_EQUAL(r.den(), 1);
    TRY(r *= Mpint(2));    TEST_EQUAL(r.num(), 284);  TEST_EQUAL(r.den(), 1);

    TRY(r = Mpint(42));

    TEST(r == Mpint(42));   TEST(r == Mpint(42));
    TEST(r <= Mpint(42));   TEST(r <= Mpint(42));
    TEST(r >= Mpint(42));   TEST(r >= Mpint(42));
    TEST(r != Mpint(100));  TEST(r != Mpint(100));
    TEST(r < Mpint(100));   TEST(r < Mpint(100));
    TEST(r <= Mpint(100));  TEST(r <= Mpint(100));

}

void test_rs_sci_mp_integer_rational_parsing() {

    Mpratio r;

    TRY(r = Mpratio::parse("0"));       TEST_EQUAL(r.num(), 0);   TEST_EQUAL(r.den(), 1);
    TRY(r = Mpratio::parse("5"));       TEST_EQUAL(r.num(), 5);   TEST_EQUAL(r.den(), 1);
    TRY(r = Mpratio::parse("-5"));      TEST_EQUAL(r.num(), -5);  TEST_EQUAL(r.den(), 1);
    TRY(r = Mpratio::parse("1/3"));     TEST_EQUAL(r.num(), 1);   TEST_EQUAL(r.den(), 3);
    TRY(r = Mpratio::parse("4/6"));     TEST_EQUAL(r.num(), 2);   TEST_EQUAL(r.den(), 3);
    TRY(r = Mpratio::parse("-1/3"));    TEST_EQUAL(r.num(), -1);  TEST_EQUAL(r.den(), 3);
    TRY(r = Mpratio::parse("-4/6"));    TEST_EQUAL(r.num(), -2);  TEST_EQUAL(r.den(), 3);
    TRY(r = Mpratio::parse("1 2/3"));   TEST_EQUAL(r.num(), 5);   TEST_EQUAL(r.den(), 3);
    TRY(r = Mpratio::parse("-1 2/3"));  TEST_EQUAL(r.num(), -5);  TEST_EQUAL(r.den(), 3);

    TEST_THROW(r = Mpratio::parse(""), std::invalid_argument);
    TEST_THROW(r = Mpratio::parse("1 2"), std::invalid_argument);
    TEST_THROW(r = Mpratio::parse("1 -2/3"), std::invalid_argument);

}
