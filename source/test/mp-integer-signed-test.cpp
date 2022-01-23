#include "rs-sci/mp-integer.hpp"
#include "rs-unit-test.hpp"
#include <string>

using namespace RS::Sci;
using namespace RS::Sci::Literals;

void test_rs_sci_mp_integer_signed_conversion_integers() {

    MPZ x;
    std::string s;

    TEST_EQUAL(x.sign(), 0);
    TEST_EQUAL(int64_t(x), 0);
    TEST_EQUAL(double(x), 0);
    TEST(x.is_even());
    TEST(! x.is_odd());
    TRY(s = to_string(x));  TEST_EQUAL(s, "0");
    TRY(s = x.str(16));     TEST_EQUAL(s, "0");
    TRY(s = x.str(36));     TEST_EQUAL(s, "0");

    TRY(x = 123456789l);
    TEST_EQUAL(x.sign(), 1);
    TEST_EQUAL(int64_t(x), 123456789l);
    TEST_EQUAL(double(x), 123456789.0);
    TEST(! x.is_even());
    TEST(x.is_odd());
    TRY(s = to_string(x));  TEST_EQUAL(s, "123456789");
    TRY(s = x.str(16));     TEST_EQUAL(s, "75bcd15");
    TRY(s = x.str(36));     TEST_EQUAL(s, "21i3v9");

    TRY(x = - 123456789l);
    TEST_EQUAL(x.sign(), -1);
    TEST_EQUAL(int64_t(x), -123456789l);
    TEST_EQUAL(double(x), -123456789.0);
    TEST(! x.is_even());
    TEST(x.is_odd());
    TRY(s = to_string(x));  TEST_EQUAL(s, "-123456789");
    TRY(s = x.str(16));     TEST_EQUAL(s, "-75bcd15");
    TRY(s = x.str(36));     TEST_EQUAL(s, "-21i3v9");

    TRY(x = 123456789123456789ll);
    TEST_EQUAL(x.sign(), 1);
    TEST_EQUAL(int64_t(x), 123456789123456789ll);
    TEST_NEAR(double(x), 1.234568e17, 1e11);
    TEST(! x.is_even());
    TEST(x.is_odd());
    TRY(s = to_string(x));  TEST_EQUAL(s, "123456789123456789");
    TRY(s = x.str(16));     TEST_EQUAL(s, "1b69b4bacd05f15");
    TRY(s = x.str(36));     TEST_EQUAL(s, "xrls1yk9rf9");

    TRY(x = - 123456789123456789ll);
    TEST_EQUAL(x.sign(), -1);
    TEST_EQUAL(int64_t(x), -123456789123456789ll);
    TEST_NEAR(double(x), -1.234568e17, 1e11);
    TEST(! x.is_even());
    TEST(x.is_odd());
    TRY(s = to_string(x));  TEST_EQUAL(s, "-123456789123456789");
    TRY(s = x.str(16));     TEST_EQUAL(s, "-1b69b4bacd05f15");
    TRY(s = x.str(36));     TEST_EQUAL(s, "-xrls1yk9rf9");

}

void test_rs_sci_mp_integer_signed_conversion_strings() {

    MPZ x;
    std::string s;

    TRY(x = MPZ("123456789123456789123456789123456789123456789", 10));
    TEST_EQUAL(x.sign(), 1);
    TEST_NEAR(double(x), 1.234568e44, 1e38);
    TRY(s = to_string(x));  TEST_EQUAL(s, "123456789123456789123456789123456789123456789");
    TRY(s = x.str(16));  TEST_EQUAL(s, "58936e53d139afefabb2683f150b684045f15");

    TRY(x = MPZ("123456789abcdef123456789abcdef123456789abcdef123456789abcdef", 16));
    TEST_EQUAL(x.sign(), 1);
    TEST_NEAR(double(x), 1.256425e71, 1e65);
    TRY(s = to_string(x));  TEST_EQUAL(s, "125642457939796217460094503631385345882379387509263401568735420576681455");
    TRY(s = x.str(16));  TEST_EQUAL(s, "123456789abcdef123456789abcdef123456789abcdef123456789abcdef");

    TRY(x = MPZ("-123456789123456789123456789123456789123456789", 10));
    TEST_EQUAL(x.sign(), -1);
    TEST_NEAR(double(x), -1.234568e44, 1e38);
    TRY(s = to_string(x));  TEST_EQUAL(s, "-123456789123456789123456789123456789123456789");
    TRY(s = x.str(16));  TEST_EQUAL(s, "-58936e53d139afefabb2683f150b684045f15");

    TRY(x = MPZ("-123456789abcdef123456789abcdef123456789abcdef123456789abcdef", 16));
    TEST_EQUAL(x.sign(), -1);
    TEST_NEAR(double(x), -1.256425e71, 1e65);
    TRY(s = to_string(x));  TEST_EQUAL(s, "-125642457939796217460094503631385345882379387509263401568735420576681455");
    TRY(s = x.str(16));  TEST_EQUAL(s, "-123456789abcdef123456789abcdef123456789abcdef123456789abcdef");

    TRY(x = MPZ("123456789123456789123456789123456789123456789", 0));
    TEST_EQUAL(x.sign(), 1);
    TEST_NEAR(double(x), 1.234568e44, 1e38);
    TRY(s = to_string(x));  TEST_EQUAL(s, "123456789123456789123456789123456789123456789");
    TRY(s = x.str(16));  TEST_EQUAL(s, "58936e53d139afefabb2683f150b684045f15");

    TRY(x = MPZ("0x123456789abcdef123456789abcdef123456789abcdef123456789abcdef", 0));
    TEST_EQUAL(x.sign(), 1);
    TEST_NEAR(double(x), 1.256425e71, 1e65);
    TRY(s = to_string(x));  TEST_EQUAL(s, "125642457939796217460094503631385345882379387509263401568735420576681455");
    TRY(s = x.str(16));  TEST_EQUAL(s, "123456789abcdef123456789abcdef123456789abcdef123456789abcdef");

    TRY(x = MPZ("-123456789123456789123456789123456789123456789", 0));
    TEST_EQUAL(x.sign(), -1);
    TEST_NEAR(double(x), -1.234568e44, 1e38);
    TRY(s = to_string(x));  TEST_EQUAL(s, "-123456789123456789123456789123456789123456789");
    TRY(s = x.str(16));  TEST_EQUAL(s, "-58936e53d139afefabb2683f150b684045f15");

    TRY(x = MPZ("-0x123456789abcdef123456789abcdef123456789abcdef123456789abcdef", 0));
    TEST_EQUAL(x.sign(), -1);
    TEST_NEAR(double(x), -1.256425e71, 1e65);
    TRY(s = to_string(x));  TEST_EQUAL(s, "-125642457939796217460094503631385345882379387509263401568735420576681455");
    TRY(s = x.str(16));  TEST_EQUAL(s, "-123456789abcdef123456789abcdef123456789abcdef123456789abcdef");

}

void test_rs_sci_mp_integer_signed_conversion_base_strings() {

    MPZ x;
    std::string s;

    TRY(x = MPZ("+1", 2));                                     TRY(s = x.str());  TEST_EQUAL(s, "1");
    TRY(x = MPZ("-12", 3));                                    TRY(s = x.str());  TEST_EQUAL(s, "-5");
    TRY(x = MPZ("+123", 4));                                   TRY(s = x.str());  TEST_EQUAL(s, "27");
    TRY(x = MPZ("-1234", 5));                                  TRY(s = x.str());  TEST_EQUAL(s, "-194");
    TRY(x = MPZ("+12345", 6));                                 TRY(s = x.str());  TEST_EQUAL(s, "1865");
    TRY(x = MPZ("-123456", 7));                                TRY(s = x.str());  TEST_EQUAL(s, "-22875");
    TRY(x = MPZ("+1234567", 8));                               TRY(s = x.str());  TEST_EQUAL(s, "342391");
    TRY(x = MPZ("-12345678", 9));                              TRY(s = x.str());  TEST_EQUAL(s, "-6053444");
    TRY(x = MPZ("+123456789", 10));                            TRY(s = x.str());  TEST_EQUAL(s, "123456789");
    TRY(x = MPZ("-123456789a", 11));                           TRY(s = x.str());  TEST_EQUAL(s, "-2853116705");
    TRY(x = MPZ("+123456789ab", 12));                          TRY(s = x.str());  TEST_EQUAL(s, "73686780563");
    TRY(x = MPZ("-123456789abc", 13));                         TRY(s = x.str());  TEST_EQUAL(s, "-2103299351334");
    TRY(x = MPZ("+123456789abcd", 14));                        TRY(s = x.str());  TEST_EQUAL(s, "65751519677857");
    TRY(x = MPZ("-123456789abcde", 15));                       TRY(s = x.str());  TEST_EQUAL(s, "-2234152501943159");
    TRY(x = MPZ("+123456789abcdef", 16));                      TRY(s = x.str());  TEST_EQUAL(s, "81985529216486895");
    TRY(x = MPZ("-123456789abcdefg", 17));                     TRY(s = x.str());  TEST_EQUAL(s, "-3231407272993502984");
    TRY(x = MPZ("+123456789abcdefgh", 18));                    TRY(s = x.str());  TEST_EQUAL(s, "136146740744970718253");
    TRY(x = MPZ("-123456789abcdefghi", 19));                   TRY(s = x.str());  TEST_EQUAL(s, "-6106233505124424657789");
    TRY(x = MPZ("+123456789abcdefghij", 20));                  TRY(s = x.str());  TEST_EQUAL(s, "290464265927977839335179");
    TRY(x = MPZ("-123456789abcdefghijk", 21));                 TRY(s = x.str());  TEST_EQUAL(s, "-14606467545964956303452810");
    TRY(x = MPZ("+123456789abcdefghijkl", 22));                TRY(s = x.str());  TEST_EQUAL(s, "774212873841767703847271481");
    TRY(x = MPZ("-123456789abcdefghijklm", 23));               TRY(s = x.str());  TEST_EQUAL(s, "-43141462809603124037923621715");
    TRY(x = MPZ("+123456789abcdefghijklmn", 24));              TRY(s = x.str());  TEST_EQUAL(s, "2521239653781255433741174806887");
    TRY(x = MPZ("-123456789abcdefghijklmno", 25));             TRY(s = x.str());  TEST_EQUAL(s, "-154197642309049519503282176123724");
    TRY(x = MPZ("+123456789abcdefghijklmnop", 26));            TRY(s = x.str());  TEST_EQUAL(s, "9849791328331451697274678861440325");
    TRY(x = MPZ("-123456789abcdefghijklmnopq", 27));           TRY(s = x.str());  TEST_EQUAL(s, "-655956343554789600515162175472115225");
    TRY(x = MPZ("+123456789abcdefghijklmnopqr", 28));          TRY(s = x.str());  TEST_EQUAL(s, "45467109894723422308055868660308101251");
    TRY(x = MPZ("-123456789abcdefghijklmnopqrs", 29));         TRY(s = x.str());  TEST_EQUAL(s, "-3275109889236238692043148233075275671534");
    TRY(x = MPZ("+123456789abcdefghijklmnopqrst", 30));        TRY(s = x.str());  TEST_EQUAL(s, "244817041729665873959571938168846611177169");
    TRY(x = MPZ("-123456789abcdefghijklmnopqrstu", 31));       TRY(s = x.str());  TEST_EQUAL(s, "-18965749034136928842900714476732571951160815");
    TRY(x = MPZ("+123456789abcdefghijklmnopqrstuv", 32));      TRY(s = x.str());  TEST_EQUAL(s, "1520813358304789717173449357665226867487963103");
    TRY(x = MPZ("-123456789abcdefghijklmnopqrstuvw", 33));     TRY(s = x.str());  TEST_EQUAL(s, "-126084023523204128749625029233070835487633968144");
    TRY(x = MPZ("+123456789abcdefghijklmnopqrstuvwx", 34));    TRY(s = x.str());  TEST_EQUAL(s, "10795811667005157035274610194040730536839248769117");
    TRY(x = MPZ("-123456789abcdefghijklmnopqrstuvwxy", 35));   TRY(s = x.str());  TEST_EQUAL(s, "-953726210513969460165905219215837671424690827366389");
    TRY(x = MPZ("+123456789abcdefghijklmnopqrstuvwxyz", 36));  TRY(s = x.str());  TEST_EQUAL(s, "86846823611197163108337531226495015298096208677436155");

}

void test_rs_sci_mp_integer_signed_conversion_double() {

    MPZ x;
    std::string s;

    TRY(x = MPZ::from_double(0.0));             TRY(s = x.str());  TEST_EQUAL(s, "0");
    TRY(x = MPZ::from_double(0.99));            TRY(s = x.str());  TEST_EQUAL(s, "0");
    TRY(x = MPZ::from_double(-0.99));           TRY(s = x.str());  TEST_EQUAL(s, "0");
    TRY(x = MPZ::from_double(1.0));             TRY(s = x.str());  TEST_EQUAL(s, "1");
    TRY(x = MPZ::from_double(-1.0));            TRY(s = x.str());  TEST_EQUAL(s, "-1");
    TRY(x = MPZ::from_double(123456789.0));     TRY(s = x.str());  TEST_EQUAL(s, "123456789");
    TRY(x = MPZ::from_double(-123456789.0));    TRY(s = x.str());  TEST_EQUAL(s, "-123456789");
    TRY(x = MPZ::from_double(1.23456789e40));   TRY(s = x.str());  TEST_MATCH(s, "^12345678\\d{33}$");
    TRY(x = MPZ::from_double(-1.23456789e40));  TRY(s = x.str());  TEST_MATCH(s, "^-12345678\\d{33}$");

}

void test_rs_sci_mp_integer_signed_arithmetic() {

    MPZ a, b, c, d, x;
    std::string s;

    TRY(x = a + b);  TRY(s = to_string(x));  TEST_EQUAL(s, "0");
    TRY(x = a - b);  TRY(s = to_string(x));  TEST_EQUAL(s, "0");
    TRY(x = a * b);  TRY(s = to_string(x));  TEST_EQUAL(s, "0");

    TRY(b = 10);

    TRY(x = a + b);  TRY(s = to_string(x));  TEST_EQUAL(s, "10");
    TRY(x = a - b);  TRY(s = to_string(x));  TEST_EQUAL(s, "-10");
    TRY(x = b - a);  TRY(s = to_string(x));  TEST_EQUAL(s, "10");
    TRY(x = a * b);  TRY(s = to_string(x));  TEST_EQUAL(s, "0");
    TRY(x = a / b);  TRY(s = to_string(x));  TEST_EQUAL(s, "0");
    TRY(x = a % b);  TRY(s = to_string(x));  TEST_EQUAL(s, "0");

    TRY(a = MPZ("42"));
    TRY(b = MPZ("10"));
    TRY(c = - a);
    TRY(d = - b);

    TRY(x = a + b);  TRY(s = to_string(x));  TEST_EQUAL(s, "52");
    TRY(x = a - b);  TRY(s = to_string(x));  TEST_EQUAL(s, "32");
    TRY(x = b - a);  TRY(s = to_string(x));  TEST_EQUAL(s, "-32");
    TRY(x = a * b);  TRY(s = to_string(x));  TEST_EQUAL(s, "420");
    TRY(x = a / b);  TRY(s = to_string(x));  TEST_EQUAL(s, "4");
    TRY(x = a % b);  TRY(s = to_string(x));  TEST_EQUAL(s, "2");

    TRY(x = a + d);  TRY(s = to_string(x));  TEST_EQUAL(s, "32");
    TRY(x = a - d);  TRY(s = to_string(x));  TEST_EQUAL(s, "52");
    TRY(x = d - a);  TRY(s = to_string(x));  TEST_EQUAL(s, "-52");
    TRY(x = a * d);  TRY(s = to_string(x));  TEST_EQUAL(s, "-420");
    TRY(x = a / d);  TRY(s = to_string(x));  TEST_EQUAL(s, "-4");
    TRY(x = a % d);  TRY(s = to_string(x));  TEST_EQUAL(s, "2");

    TRY(x = c + b);  TRY(s = to_string(x));  TEST_EQUAL(s, "-32");
    TRY(x = c - b);  TRY(s = to_string(x));  TEST_EQUAL(s, "-52");
    TRY(x = b - c);  TRY(s = to_string(x));  TEST_EQUAL(s, "52");
    TRY(x = c * b);  TRY(s = to_string(x));  TEST_EQUAL(s, "-420");
    TRY(x = c / b);  TRY(s = to_string(x));  TEST_EQUAL(s, "-5");
    TRY(x = c % b);  TRY(s = to_string(x));  TEST_EQUAL(s, "8");

    TRY(x = c + d);  TRY(s = to_string(x));  TEST_EQUAL(s, "-52");
    TRY(x = c - d);  TRY(s = to_string(x));  TEST_EQUAL(s, "-32");
    TRY(x = d - c);  TRY(s = to_string(x));  TEST_EQUAL(s, "32");
    TRY(x = c * d);  TRY(s = to_string(x));  TEST_EQUAL(s, "420");
    TRY(x = c / d);  TRY(s = to_string(x));  TEST_EQUAL(s, "5");
    TRY(x = c % d);  TRY(s = to_string(x));  TEST_EQUAL(s, "8");

    TRY(x = a / 1);  TEST_EQUAL(x, a);
    TRY(x = b / 1);  TEST_EQUAL(x, b);
    TRY(x = c / 1);  TEST_EQUAL(x, c);
    TRY(x = d / 1);  TEST_EQUAL(x, d);

}

void test_rs_sci_mp_integer_signed_division() {

    MPZ a, b, q, r;

    TRY(a = -6);  TRY(b = 3);   TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 0);  TEST_EQUAL(b * q + r, a);
    TRY(a = -5);  TRY(b = 3);   TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 1);  TEST_EQUAL(b * q + r, a);
    TRY(a = -4);  TRY(b = 3);   TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 2);  TEST_EQUAL(b * q + r, a);
    TRY(a = -3);  TRY(b = 3);   TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 0);  TEST_EQUAL(b * q + r, a);
    TRY(a = -2);  TRY(b = 3);   TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 1);  TEST_EQUAL(b * q + r, a);
    TRY(a = -1);  TRY(b = 3);   TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 2);  TEST_EQUAL(b * q + r, a);
    TRY(a = 0);   TRY(b = 3);   TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 0);  TEST_EQUAL(b * q + r, a);
    TRY(a = 1);   TRY(b = 3);   TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 1);  TEST_EQUAL(b * q + r, a);
    TRY(a = 2);   TRY(b = 3);   TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 2);  TEST_EQUAL(b * q + r, a);
    TRY(a = 3);   TRY(b = 3);   TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 0);  TEST_EQUAL(b * q + r, a);
    TRY(a = 4);   TRY(b = 3);   TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 1);  TEST_EQUAL(b * q + r, a);
    TRY(a = 5);   TRY(b = 3);   TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 2);  TEST_EQUAL(b * q + r, a);
    TRY(a = 6);   TRY(b = 3);   TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 0);  TEST_EQUAL(b * q + r, a);
    TRY(a = -6);  TRY(b = -3);  TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 0);  TEST_EQUAL(b * q + r, a);
    TRY(a = -5);  TRY(b = -3);  TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 1);  TEST_EQUAL(b * q + r, a);
    TRY(a = -4);  TRY(b = -3);  TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, 2);   TEST_EQUAL(r, 2);  TEST_EQUAL(b * q + r, a);
    TRY(a = -3);  TRY(b = -3);  TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 0);  TEST_EQUAL(b * q + r, a);
    TRY(a = -2);  TRY(b = -3);  TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 1);  TEST_EQUAL(b * q + r, a);
    TRY(a = -1);  TRY(b = -3);  TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, 1);   TEST_EQUAL(r, 2);  TEST_EQUAL(b * q + r, a);
    TRY(a = 0);   TRY(b = -3);  TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 0);  TEST_EQUAL(b * q + r, a);
    TRY(a = 1);   TRY(b = -3);  TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 1);  TEST_EQUAL(b * q + r, a);
    TRY(a = 2);   TRY(b = -3);  TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, 0);   TEST_EQUAL(r, 2);  TEST_EQUAL(b * q + r, a);
    TRY(a = 3);   TRY(b = -3);  TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 0);  TEST_EQUAL(b * q + r, a);
    TRY(a = 4);   TRY(b = -3);  TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 1);  TEST_EQUAL(b * q + r, a);
    TRY(a = 5);   TRY(b = -3);  TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, -1);  TEST_EQUAL(r, 2);  TEST_EQUAL(b * q + r, a);
    TRY(a = 6);   TRY(b = -3);  TRY(q = a / b);  TRY(r = a % b);  TEST_EQUAL(q, -2);  TEST_EQUAL(r, 0);  TEST_EQUAL(b * q + r, a);

}

void test_rs_sci_mp_integer_signed_large_arithmetic() {

    MPZ a, b, c, d, x, y, z;
    std::string s;

    TRY(a = MPZ("123456789123456789123456789123456789123456789"));
    TRY(b = MPZ("1357913579135791357913579"));
    TRY(c = - a);
    TRY(d = - b);

    TRY(x = a + b);  TRY(s = to_string(x));  TEST_EQUAL(s, "123456789123456789124814702702592580481370368");
    TRY(x = a - b);  TRY(s = to_string(x));  TEST_EQUAL(s, "123456789123456789122098875544320997765543210");
    TRY(x = b - a);  TRY(s = to_string(x));  TEST_EQUAL(s, "-123456789123456789122098875544320997765543210");
    TRY(x = a * b);  TRY(s = to_string(x));  TEST_EQUAL(s, "167643650387245846409206049247037049247037049079393398859791202837831");
    TRY(x = a / b);  TRY(s = to_string(x));  TEST_EQUAL(s, "90916528872203810704");
    TRY(x = a % b);  TRY(s = to_string(x));  TEST_EQUAL(s, "603972940850418616307173");

    TRY(x = a + d);  TRY(s = to_string(x));  TEST_EQUAL(s, "123456789123456789122098875544320997765543210");
    TRY(x = a - d);  TRY(s = to_string(x));  TEST_EQUAL(s, "123456789123456789124814702702592580481370368");
    TRY(x = d - a);  TRY(s = to_string(x));  TEST_EQUAL(s, "-123456789123456789124814702702592580481370368");
    TRY(x = a * d);  TRY(s = to_string(x));  TEST_EQUAL(s, "-167643650387245846409206049247037049247037049079393398859791202837831");
    TRY(x = a / d);  TRY(s = to_string(x));  TEST_EQUAL(s, "-90916528872203810704");
    TRY(x = a % d);  TRY(s = to_string(x));  TEST_EQUAL(s, "603972940850418616307173");

    TRY(x = c + b);  TRY(s = to_string(x));  TEST_EQUAL(s, "-123456789123456789122098875544320997765543210");
    TRY(x = c - b);  TRY(s = to_string(x));  TEST_EQUAL(s, "-123456789123456789124814702702592580481370368");
    TRY(x = b - c);  TRY(s = to_string(x));  TEST_EQUAL(s, "123456789123456789124814702702592580481370368");
    TRY(x = c * b);  TRY(s = to_string(x));  TEST_EQUAL(s, "-167643650387245846409206049247037049247037049079393398859791202837831");
    TRY(x = c / b);  TRY(s = to_string(x));  TEST_EQUAL(s, "-90916528872203810705");
    TRY(x = c % b);  TRY(s = to_string(x));  TEST_EQUAL(s, "753940638285372741606406");

    TRY(x = c + d);  TRY(s = to_string(x));  TEST_EQUAL(s, "-123456789123456789124814702702592580481370368");
    TRY(x = c - d);  TRY(s = to_string(x));  TEST_EQUAL(s, "-123456789123456789122098875544320997765543210");
    TRY(x = d - c);  TRY(s = to_string(x));  TEST_EQUAL(s, "123456789123456789122098875544320997765543210");
    TRY(x = c * d);  TRY(s = to_string(x));  TEST_EQUAL(s, "167643650387245846409206049247037049247037049079393398859791202837831");
    TRY(x = c / d);  TRY(s = to_string(x));  TEST_EQUAL(s, "90916528872203810705");
    TRY(x = c % d);  TRY(s = to_string(x));  TEST_EQUAL(s, "753940638285372741606406");

    TRY(x = 1);   TRY(y = 1);   TRY(z = x * y);  TEST_EQUAL(z.str(), "1");
    TRY(x = 1);   TRY(y = 0);   TRY(z = x * y);  TEST_EQUAL(z.str(), "0");
    TRY(x = 1);   TRY(y = -1);  TRY(z = x * y);  TEST_EQUAL(z.str(), "-1");
    TRY(x = 0);   TRY(y = 1);   TRY(z = x * y);  TEST_EQUAL(z.str(), "0");
    TRY(x = 0);   TRY(y = 0);   TRY(z = x * y);  TEST_EQUAL(z.str(), "0");
    TRY(x = 0);   TRY(y = -1);  TRY(z = x * y);  TEST_EQUAL(z.str(), "0");
    TRY(x = -1);  TRY(y = 1);   TRY(z = x * y);  TEST_EQUAL(z.str(), "-1");
    TRY(x = -1);  TRY(y = 0);   TRY(z = x * y);  TEST_EQUAL(z.str(), "0");
    TRY(x = -1);  TRY(y = -1);  TRY(z = x * y);  TEST_EQUAL(z.str(), "1");

}

void test_rs_sci_mp_integer_signed_powers() {

    MPZ x, y;

    TRY(x = 0);    TRY(y = x.pow(0));   TEST_EQUAL(y.str(), "1");
    TRY(x = 0);    TRY(y = x.pow(1));   TEST_EQUAL(y.str(), "0");
    TRY(x = 0);    TRY(y = x.pow(2));   TEST_EQUAL(y.str(), "0");
    TRY(x = 0);    TRY(y = x.pow(3));   TEST_EQUAL(y.str(), "0");
    TRY(x = 1);    TRY(y = x.pow(0));   TEST_EQUAL(y.str(), "1");
    TRY(x = 1);    TRY(y = x.pow(1));   TEST_EQUAL(y.str(), "1");
    TRY(x = 1);    TRY(y = x.pow(2));   TEST_EQUAL(y.str(), "1");
    TRY(x = 1);    TRY(y = x.pow(3));   TEST_EQUAL(y.str(), "1");
    TRY(x = -1);   TRY(y = x.pow(0));   TEST_EQUAL(y.str(), "1");
    TRY(x = -1);   TRY(y = x.pow(1));   TEST_EQUAL(y.str(), "-1");
    TRY(x = -1);   TRY(y = x.pow(2));   TEST_EQUAL(y.str(), "1");
    TRY(x = -1);   TRY(y = x.pow(3));   TEST_EQUAL(y.str(), "-1");
    TRY(x = 10);   TRY(y = x.pow(0));   TEST_EQUAL(y.str(), "1");
    TRY(x = 10);   TRY(y = x.pow(1));   TEST_EQUAL(y.str(), "10");
    TRY(x = 10);   TRY(y = x.pow(2));   TEST_EQUAL(y.str(), "100");
    TRY(x = 10);   TRY(y = x.pow(3));   TEST_EQUAL(y.str(), "1000");
    TRY(x = 10);   TRY(y = x.pow(4));   TEST_EQUAL(y.str(), "10000");
    TRY(x = 10);   TRY(y = x.pow(5));   TEST_EQUAL(y.str(), "100000");
    TRY(x = 10);   TRY(y = x.pow(6));   TEST_EQUAL(y.str(), "1000000");
    TRY(x = 10);   TRY(y = x.pow(7));   TEST_EQUAL(y.str(), "10000000");
    TRY(x = 10);   TRY(y = x.pow(8));   TEST_EQUAL(y.str(), "100000000");
    TRY(x = 10);   TRY(y = x.pow(9));   TEST_EQUAL(y.str(), "1000000000");
    TRY(x = 10);   TRY(y = x.pow(10));  TEST_EQUAL(y.str(), "10000000000");
    TRY(x = 10);   TRY(y = x.pow(11));  TEST_EQUAL(y.str(), "100000000000");
    TRY(x = 10);   TRY(y = x.pow(12));  TEST_EQUAL(y.str(), "1000000000000");
    TRY(x = 10);   TRY(y = x.pow(13));  TEST_EQUAL(y.str(), "10000000000000");
    TRY(x = 10);   TRY(y = x.pow(14));  TEST_EQUAL(y.str(), "100000000000000");
    TRY(x = 10);   TRY(y = x.pow(15));  TEST_EQUAL(y.str(), "1000000000000000");
    TRY(x = 10);   TRY(y = x.pow(16));  TEST_EQUAL(y.str(), "10000000000000000");
    TRY(x = 10);   TRY(y = x.pow(17));  TEST_EQUAL(y.str(), "100000000000000000");
    TRY(x = 10);   TRY(y = x.pow(18));  TEST_EQUAL(y.str(), "1000000000000000000");
    TRY(x = 10);   TRY(y = x.pow(19));  TEST_EQUAL(y.str(), "10000000000000000000");
    TRY(x = 10);   TRY(y = x.pow(20));  TEST_EQUAL(y.str(), "100000000000000000000");
    TRY(x = 10);   TRY(y = x.pow(21));  TEST_EQUAL(y.str(), "1000000000000000000000");
    TRY(x = 10);   TRY(y = x.pow(22));  TEST_EQUAL(y.str(), "10000000000000000000000");
    TRY(x = 10);   TRY(y = x.pow(23));  TEST_EQUAL(y.str(), "100000000000000000000000");
    TRY(x = 10);   TRY(y = x.pow(24));  TEST_EQUAL(y.str(), "1000000000000000000000000");
    TRY(x = 10);   TRY(y = x.pow(25));  TEST_EQUAL(y.str(), "10000000000000000000000000");
    TRY(x = 10);   TRY(y = x.pow(26));  TEST_EQUAL(y.str(), "100000000000000000000000000");
    TRY(x = 10);   TRY(y = x.pow(27));  TEST_EQUAL(y.str(), "1000000000000000000000000000");
    TRY(x = 10);   TRY(y = x.pow(28));  TEST_EQUAL(y.str(), "10000000000000000000000000000");
    TRY(x = 10);   TRY(y = x.pow(29));  TEST_EQUAL(y.str(), "100000000000000000000000000000");
    TRY(x = 10);   TRY(y = x.pow(30));  TEST_EQUAL(y.str(), "1000000000000000000000000000000");
    TRY(x = -10);  TRY(y = x.pow(0));   TEST_EQUAL(y.str(), "1");
    TRY(x = -10);  TRY(y = x.pow(1));   TEST_EQUAL(y.str(), "-10");
    TRY(x = -10);  TRY(y = x.pow(2));   TEST_EQUAL(y.str(), "100");
    TRY(x = -10);  TRY(y = x.pow(3));   TEST_EQUAL(y.str(), "-1000");
    TRY(x = -10);  TRY(y = x.pow(4));   TEST_EQUAL(y.str(), "10000");
    TRY(x = -10);  TRY(y = x.pow(5));   TEST_EQUAL(y.str(), "-100000");
    TRY(x = -10);  TRY(y = x.pow(6));   TEST_EQUAL(y.str(), "1000000");
    TRY(x = -10);  TRY(y = x.pow(7));   TEST_EQUAL(y.str(), "-10000000");
    TRY(x = -10);  TRY(y = x.pow(8));   TEST_EQUAL(y.str(), "100000000");
    TRY(x = -10);  TRY(y = x.pow(9));   TEST_EQUAL(y.str(), "-1000000000");
    TRY(x = -10);  TRY(y = x.pow(10));  TEST_EQUAL(y.str(), "10000000000");
    TRY(x = -10);  TRY(y = x.pow(11));  TEST_EQUAL(y.str(), "-100000000000");
    TRY(x = -10);  TRY(y = x.pow(12));  TEST_EQUAL(y.str(), "1000000000000");
    TRY(x = -10);  TRY(y = x.pow(13));  TEST_EQUAL(y.str(), "-10000000000000");
    TRY(x = -10);  TRY(y = x.pow(14));  TEST_EQUAL(y.str(), "100000000000000");
    TRY(x = -10);  TRY(y = x.pow(15));  TEST_EQUAL(y.str(), "-1000000000000000");
    TRY(x = -10);  TRY(y = x.pow(16));  TEST_EQUAL(y.str(), "10000000000000000");
    TRY(x = -10);  TRY(y = x.pow(17));  TEST_EQUAL(y.str(), "-100000000000000000");
    TRY(x = -10);  TRY(y = x.pow(18));  TEST_EQUAL(y.str(), "1000000000000000000");
    TRY(x = -10);  TRY(y = x.pow(19));  TEST_EQUAL(y.str(), "-10000000000000000000");
    TRY(x = -10);  TRY(y = x.pow(20));  TEST_EQUAL(y.str(), "100000000000000000000");
    TRY(x = -10);  TRY(y = x.pow(21));  TEST_EQUAL(y.str(), "-1000000000000000000000");
    TRY(x = -10);  TRY(y = x.pow(22));  TEST_EQUAL(y.str(), "10000000000000000000000");
    TRY(x = -10);  TRY(y = x.pow(23));  TEST_EQUAL(y.str(), "-100000000000000000000000");
    TRY(x = -10);  TRY(y = x.pow(24));  TEST_EQUAL(y.str(), "1000000000000000000000000");
    TRY(x = -10);  TRY(y = x.pow(25));  TEST_EQUAL(y.str(), "-10000000000000000000000000");
    TRY(x = -10);  TRY(y = x.pow(26));  TEST_EQUAL(y.str(), "100000000000000000000000000");
    TRY(x = -10);  TRY(y = x.pow(27));  TEST_EQUAL(y.str(), "-1000000000000000000000000000");
    TRY(x = -10);  TRY(y = x.pow(28));  TEST_EQUAL(y.str(), "10000000000000000000000000000");
    TRY(x = -10);  TRY(y = x.pow(29));  TEST_EQUAL(y.str(), "-100000000000000000000000000000");
    TRY(x = -10);  TRY(y = x.pow(30));  TEST_EQUAL(y.str(), "1000000000000000000000000000000");

}
