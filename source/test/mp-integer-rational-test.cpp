#include "rs-sci/mp-integer.hpp"
#include "rs-sci/rational.hpp"
#include "rs-unit-test.hpp"
#include <stdexcept>

using namespace RS::Sci;
using namespace RS::Sci::Literals;

void test_rs_sci_mp_integer_rational_basics() {

    MPQ r;

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
    TEST_EQUAL(r.str("nv"), "0/1");

    TRY(r = MPQ(5, 3));
    TEST_EQUAL(r.num(), 5);
    TEST_EQUAL(r.den(), 3);
    TEST_EQUAL(int(r), 1);
    TEST_NEAR(double(r), 1.666667, 1e-6);
    TEST(! r.is_integer());
    TEST_EQUAL(r.whole(), 1);
    TEST_EQUAL(r.frac().num(), 2);
    TEST_EQUAL(r.frac().den(), 3);
    TEST_EQUAL(r.str(), "1 2/3");
    TEST_EQUAL(r.str("nv"), "5/3");

    TRY(r = MPQ(-7, 9));
    TEST_EQUAL(r.num(), -7);
    TEST_EQUAL(r.den(), 9);
    TEST_EQUAL(int(r), -1);
    TEST_NEAR(double(r), -0.777778, 1e-6);
    TEST(! r.is_integer());
    TEST_EQUAL(r.whole(), 0);
    TEST_EQUAL(r.frac().num(), -7);
    TEST_EQUAL(r.frac().den(), 9);
    TEST_EQUAL(r.str(), "-7/9");
    TEST_EQUAL(r.str("nv"), "-7/9");

}

void test_rs_sci_mp_integer_rational_reduction() {

    MPQ r;

    TRY(r = MPQ(0, 6));    TEST_EQUAL(r.num(), 0);    TEST_EQUAL(r.den(), 1);  TEST_EQUAL(r.whole(), 0);   TEST_EQUAL(r.frac(), MPQ(0, 6));
    TRY(r = MPQ(1, 6));    TEST_EQUAL(r.num(), 1);    TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), 0);   TEST_EQUAL(r.frac(), MPQ(1, 6));
    TRY(r = MPQ(2, 6));    TEST_EQUAL(r.num(), 1);    TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), 0);   TEST_EQUAL(r.frac(), MPQ(2, 6));
    TRY(r = MPQ(3, 6));    TEST_EQUAL(r.num(), 1);    TEST_EQUAL(r.den(), 2);  TEST_EQUAL(r.whole(), 0);   TEST_EQUAL(r.frac(), MPQ(3, 6));
    TRY(r = MPQ(4, 6));    TEST_EQUAL(r.num(), 2);    TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), 0);   TEST_EQUAL(r.frac(), MPQ(4, 6));
    TRY(r = MPQ(5, 6));    TEST_EQUAL(r.num(), 5);    TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), 0);   TEST_EQUAL(r.frac(), MPQ(5, 6));
    TRY(r = MPQ(6, 6));    TEST_EQUAL(r.num(), 1);    TEST_EQUAL(r.den(), 1);  TEST_EQUAL(r.whole(), 1);   TEST_EQUAL(r.frac(), MPQ(0, 6));
    TRY(r = MPQ(7, 6));    TEST_EQUAL(r.num(), 7);    TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), 1);   TEST_EQUAL(r.frac(), MPQ(1, 6));
    TRY(r = MPQ(8, 6));    TEST_EQUAL(r.num(), 4);    TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), 1);   TEST_EQUAL(r.frac(), MPQ(2, 6));
    TRY(r = MPQ(9, 6));    TEST_EQUAL(r.num(), 3);    TEST_EQUAL(r.den(), 2);  TEST_EQUAL(r.whole(), 1);   TEST_EQUAL(r.frac(), MPQ(3, 6));
    TRY(r = MPQ(10, 6));   TEST_EQUAL(r.num(), 5);    TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), 1);   TEST_EQUAL(r.frac(), MPQ(4, 6));
    TRY(r = MPQ(11, 6));   TEST_EQUAL(r.num(), 11);   TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), 1);   TEST_EQUAL(r.frac(), MPQ(5, 6));
    TRY(r = MPQ(12, 6));   TEST_EQUAL(r.num(), 2);    TEST_EQUAL(r.den(), 1);  TEST_EQUAL(r.whole(), 2);   TEST_EQUAL(r.frac(), MPQ(0, 6));
    TRY(r = MPQ(-1, 6));   TEST_EQUAL(r.num(), -1);   TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), -0);  TEST_EQUAL(r.frac(), MPQ(-1, 6));
    TRY(r = MPQ(-2, 6));   TEST_EQUAL(r.num(), -1);   TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), -0);  TEST_EQUAL(r.frac(), MPQ(-2, 6));
    TRY(r = MPQ(-3, 6));   TEST_EQUAL(r.num(), -1);   TEST_EQUAL(r.den(), 2);  TEST_EQUAL(r.whole(), -0);  TEST_EQUAL(r.frac(), MPQ(-3, 6));
    TRY(r = MPQ(-4, 6));   TEST_EQUAL(r.num(), -2);   TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), -0);  TEST_EQUAL(r.frac(), MPQ(-4, 6));
    TRY(r = MPQ(-5, 6));   TEST_EQUAL(r.num(), -5);   TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), -0);  TEST_EQUAL(r.frac(), MPQ(-5, 6));
    TRY(r = MPQ(-6, 6));   TEST_EQUAL(r.num(), -1);   TEST_EQUAL(r.den(), 1);  TEST_EQUAL(r.whole(), -1);  TEST_EQUAL(r.frac(), MPQ(-0, 6));
    TRY(r = MPQ(-7, 6));   TEST_EQUAL(r.num(), -7);   TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), -1);  TEST_EQUAL(r.frac(), MPQ(-1, 6));
    TRY(r = MPQ(-8, 6));   TEST_EQUAL(r.num(), -4);   TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), -1);  TEST_EQUAL(r.frac(), MPQ(-2, 6));
    TRY(r = MPQ(-9, 6));   TEST_EQUAL(r.num(), -3);   TEST_EQUAL(r.den(), 2);  TEST_EQUAL(r.whole(), -1);  TEST_EQUAL(r.frac(), MPQ(-3, 6));
    TRY(r = MPQ(-10, 6));  TEST_EQUAL(r.num(), -5);   TEST_EQUAL(r.den(), 3);  TEST_EQUAL(r.whole(), -1);  TEST_EQUAL(r.frac(), MPQ(-4, 6));
    TRY(r = MPQ(-11, 6));  TEST_EQUAL(r.num(), -11);  TEST_EQUAL(r.den(), 6);  TEST_EQUAL(r.whole(), -1);  TEST_EQUAL(r.frac(), MPQ(-5, 6));
    TRY(r = MPQ(-12, 6));  TEST_EQUAL(r.num(), -2);   TEST_EQUAL(r.den(), 1);  TEST_EQUAL(r.whole(), -2);  TEST_EQUAL(r.frac(), MPQ(-0, 6));

}

void test_rs_sci_mp_integer_rational_arithmetic() {

    MPQ r, s, t;

    TRY(r = MPQ(5, 3));
    TRY(s = MPQ(7, 9));

    TRY(t = r + s);  TEST_EQUAL(t.num(), 22);  TEST_EQUAL(t.den(), 9);
    TRY(t = r - s);  TEST_EQUAL(t.num(), 8);   TEST_EQUAL(t.den(), 9);
    TRY(t = r * s);  TEST_EQUAL(t.num(), 35);  TEST_EQUAL(t.den(), 27);
    TRY(t = r / s);  TEST_EQUAL(t.num(), 15);  TEST_EQUAL(t.den(), 7);

}

void test_rs_sci_mp_integer_rational_properties() {

    MPQ r, s, t;

    TRY(r = MPQ(5, 3));
    TRY(s = MPQ(-7, 9));

    TEST_EQUAL(r.abs(), MPQ(5, 3));
    TEST_EQUAL(s.abs(), MPQ(7, 9));
    TEST_EQUAL(t.abs(), MPQ(0));

    TRY(r = MPQ(-6, 3));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -2);  TEST_EQUAL(r.round(), -2);
    TRY(r = MPQ(-5, 3));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -2);
    TRY(r = MPQ(-4, 3));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -1);
    TRY(r = MPQ(-3, 3));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -1);
    TRY(r = MPQ(-2, 3));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), -1);
    TRY(r = MPQ(-1, 3));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), 0);
    TRY(r = MPQ(0, 3));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), 0);
    TRY(r = MPQ(1, 3));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 0);
    TRY(r = MPQ(2, 3));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 1);
    TRY(r = MPQ(3, 3));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 1);
    TRY(r = MPQ(4, 3));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 1);
    TRY(r = MPQ(5, 3));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 2);
    TRY(r = MPQ(6, 3));   TEST_EQUAL(r.floor(), 2);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 2);
    TRY(r = MPQ(-8, 4));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -2);  TEST_EQUAL(r.round(), -2);
    TRY(r = MPQ(-7, 4));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -2);
    TRY(r = MPQ(-6, 4));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -1);
    TRY(r = MPQ(-5, 4));  TEST_EQUAL(r.floor(), -2);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -1);
    TRY(r = MPQ(-4, 4));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), -1);  TEST_EQUAL(r.round(), -1);
    TRY(r = MPQ(-3, 4));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), -1);
    TRY(r = MPQ(-2, 4));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), 0);
    TRY(r = MPQ(-1, 4));  TEST_EQUAL(r.floor(), -1);  TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), 0);
    TRY(r = MPQ(0, 4));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 0);   TEST_EQUAL(r.round(), 0);
    TRY(r = MPQ(1, 4));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 0);
    TRY(r = MPQ(2, 4));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 1);
    TRY(r = MPQ(3, 4));   TEST_EQUAL(r.floor(), 0);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 1);
    TRY(r = MPQ(4, 4));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 1);   TEST_EQUAL(r.round(), 1);
    TRY(r = MPQ(5, 4));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 1);
    TRY(r = MPQ(6, 4));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 2);
    TRY(r = MPQ(7, 4));   TEST_EQUAL(r.floor(), 1);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 2);
    TRY(r = MPQ(8, 4));   TEST_EQUAL(r.floor(), 2);   TEST_EQUAL(r.ceil(), 2);   TEST_EQUAL(r.round(), 2);

}

void test_rs_sci_mp_integer_rational_comparison() {

    MPQ r, s;

    TRY(r = MPQ(5, 6));   TRY(s = MPQ(7, 9));     TEST(r > s);   TEST(r >= s);  TEST(r != s);
    TRY(r = MPQ(5, 6));   TRY(s = MPQ(8, 9));     TEST(r < s);   TEST(r <= s);  TEST(r != s);
    TRY(r = MPQ(5, 6));   TRY(s = MPQ(10, 12));   TEST(r == s);  TEST(r <= s);  TEST(r >= s);
    TRY(r = MPQ(-5, 6));  TRY(s = MPQ(-7, 9));    TEST(r < s);   TEST(r <= s);  TEST(r != s);
    TRY(r = MPQ(-5, 6));  TRY(s = MPQ(-8, 9));    TEST(r > s);   TEST(r >= s);  TEST(r != s);
    TRY(r = MPQ(-5, 6));  TRY(s = MPQ(-10, 12));  TEST(r == s);  TEST(r <= s);  TEST(r >= s);

}

void test_rs_sci_mp_integer_rational_mixed() {

    MPQ r;

    TRY(r = MPZ(42));    TEST_EQUAL(r.num(), 42);   TEST_EQUAL(r.den(), 1);
    TRY(r += MPZ(100));  TEST_EQUAL(r.num(), 142);  TEST_EQUAL(r.den(), 1);
    TRY(r *= MPZ(2));    TEST_EQUAL(r.num(), 284);  TEST_EQUAL(r.den(), 1);

    TRY(r = MPZ(42));

    TEST(r == MPZ(42));   TEST(r == MPZ(42));
    TEST(r <= MPZ(42));   TEST(r <= MPZ(42));
    TEST(r >= MPZ(42));   TEST(r >= MPZ(42));
    TEST(r != MPZ(100));  TEST(r != MPZ(100));
    TEST(r < MPZ(100));   TEST(r < MPZ(100));
    TEST(r <= MPZ(100));  TEST(r <= MPZ(100));

}

void test_rs_sci_mp_integer_rational_parsing() {

    MPQ r;

    TRY(r = MPQ::parse("0"));       TEST_EQUAL(r.num(), 0);   TEST_EQUAL(r.den(), 1);
    TRY(r = MPQ::parse("5"));       TEST_EQUAL(r.num(), 5);   TEST_EQUAL(r.den(), 1);
    TRY(r = MPQ::parse("-5"));      TEST_EQUAL(r.num(), -5);  TEST_EQUAL(r.den(), 1);
    TRY(r = MPQ::parse("1/3"));     TEST_EQUAL(r.num(), 1);   TEST_EQUAL(r.den(), 3);
    TRY(r = MPQ::parse("4/6"));     TEST_EQUAL(r.num(), 2);   TEST_EQUAL(r.den(), 3);
    TRY(r = MPQ::parse("-1/3"));    TEST_EQUAL(r.num(), -1);  TEST_EQUAL(r.den(), 3);
    TRY(r = MPQ::parse("-4/6"));    TEST_EQUAL(r.num(), -2);  TEST_EQUAL(r.den(), 3);
    TRY(r = MPQ::parse("1 2/3"));   TEST_EQUAL(r.num(), 5);   TEST_EQUAL(r.den(), 3);
    TRY(r = MPQ::parse("-1 2/3"));  TEST_EQUAL(r.num(), -5);  TEST_EQUAL(r.den(), 3);

    TEST_THROW(r = MPQ::parse(""), std::invalid_argument);
    TEST_THROW(r = MPQ::parse("1 2"), std::invalid_argument);
    TEST_THROW(r = MPQ::parse("1 -2/3"), std::invalid_argument);

}
