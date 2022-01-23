#include "rs-sci/mp-integer.hpp"
#include "rs-format/format.hpp"
#include "rs-unit-test.hpp"
#include <string>
#include <vector>

using namespace RS::Format;
using namespace RS::Sci;
using namespace RS::Sci::Literals;

namespace {

    using ByteVector = std::vector<uint8_t>;

    std::string hexdump(const ByteVector& v) {
        std::string s(reinterpret_cast<const char*>(v.data()), v.size());
        return format_string(s, "x");
    }

}

void test_rs_sci_mp_integer_unsigned_conversion_integers() {

    MPN x;
    std::string s;

    TEST_EQUAL(uint64_t(x), 0u);
    TEST_EQUAL(double(x), 0);
    TRY(s = x.str(2));   TEST_EQUAL(s, "0");
    TRY(s = x.str(10));  TEST_EQUAL(s, "0");
    TRY(s = x.str(16));  TEST_EQUAL(s, "0");
    TRY(s = x.str(36));  TEST_EQUAL(s, "0");

    TRY(x = 0x12345678ul);
    TEST_EQUAL(x.bits(), 29u);
    TEST_EQUAL(uint64_t(x), 0x12345678ul);
    TEST_EQUAL(double(x), 305419896);
    TRY(s = x.str(2));   TEST_EQUAL(s, "10010001101000101011001111000");
    TRY(s = x.str(10));  TEST_EQUAL(s, "305419896");
    TRY(s = x.str(16));  TEST_EQUAL(s, "12345678");
    TRY(s = x.str(36));  TEST_EQUAL(s, "51u7i0");

    TRY(x = 0x123456789abcdef0ull);
    TEST_EQUAL(x.bits(), 61u);
    TEST_EQUAL(uint64_t(x), 0x123456789abcdef0ull);
    TEST_NEAR(double(x), 1.311768e18, 1e12);
    TRY(s = x.str(2));   TEST_EQUAL(s, "1001000110100010101100111100010011010101111001101111011110000");
    TRY(s = x.str(10));  TEST_EQUAL(s, "1311768467463790320");
    TRY(s = x.str(16));  TEST_EQUAL(s, "123456789abcdef0");
    TRY(s = x.str(36));  TEST_EQUAL(s, "9ys742pps3qo");

}

void test_rs_sci_mp_integer_unsigned_conversion_strings() {

    MPN x;
    std::string s;

    TRY(x = MPN("110011000011110111111011111001011100011101100011001111101111100000001000101111100010101", 2));
    TEST_NEAR(double(x), 1.234568e26, 1e20);
    TRY(s = x.str(2));   TEST_EQUAL(s, "110011000011110111111011111001011100011101100011001111101111100000001000101111100010101");
    TRY(s = x.str(10));  TEST_EQUAL(s, "123456789123456789123456789");
    TRY(s = x.str(16));  TEST_EQUAL(s, "661efdf2e3b19f7c045f15");

    TRY(x = MPN("123456789123456789123456789", 10));
    TEST_NEAR(double(x), 1.234568e26, 1e20);
    TRY(s = x.str(2));   TEST_EQUAL(s, "110011000011110111111011111001011100011101100011001111101111100000001000101111100010101");
    TRY(s = x.str(10));  TEST_EQUAL(s, "123456789123456789123456789");
    TRY(s = x.str(16));  TEST_EQUAL(s, "661efdf2e3b19f7c045f15");

    TRY(x = MPN("661efdf2e3b19f7c045f15", 16));
    TEST_NEAR(double(x), 1.234568e26, 1e20);
    TRY(s = x.str(2));   TEST_EQUAL(s, "110011000011110111111011111001011100011101100011001111101111100000001000101111100010101");
    TRY(s = x.str(10));  TEST_EQUAL(s, "123456789123456789123456789");
    TRY(s = x.str(16));  TEST_EQUAL(s, "661efdf2e3b19f7c045f15");

    TRY(x = MPN("123456789123456789123456789", 0));
    TEST_NEAR(double(x), 1.234568e26, 1e20);
    TRY(s = x.str(2));   TEST_EQUAL(s, "110011000011110111111011111001011100011101100011001111101111100000001000101111100010101");
    TRY(s = x.str(10));  TEST_EQUAL(s, "123456789123456789123456789");
    TRY(s = x.str(16));  TEST_EQUAL(s, "661efdf2e3b19f7c045f15");

    TRY(x = MPN("0x661efdf2e3b19f7c045f15", 0));
    TEST_NEAR(double(x), 1.234568e26, 1e20);
    TRY(s = x.str(2));   TEST_EQUAL(s, "110011000011110111111011111001011100011101100011001111101111100000001000101111100010101");
    TRY(s = x.str(10));  TEST_EQUAL(s, "123456789123456789123456789");
    TRY(s = x.str(16));  TEST_EQUAL(s, "661efdf2e3b19f7c045f15");

}

void test_rs_sci_mp_integer_unsigned_conversion_base_strings() {

    MPN x;
    std::string s;

    TRY(x = MPN("1", 2));                                     TRY(s = x.str());  TEST_EQUAL(s, "1");
    TRY(x = MPN("12", 3));                                    TRY(s = x.str());  TEST_EQUAL(s, "5");
    TRY(x = MPN("123", 4));                                   TRY(s = x.str());  TEST_EQUAL(s, "27");
    TRY(x = MPN("1234", 5));                                  TRY(s = x.str());  TEST_EQUAL(s, "194");
    TRY(x = MPN("12345", 6));                                 TRY(s = x.str());  TEST_EQUAL(s, "1865");
    TRY(x = MPN("123456", 7));                                TRY(s = x.str());  TEST_EQUAL(s, "22875");
    TRY(x = MPN("1234567", 8));                               TRY(s = x.str());  TEST_EQUAL(s, "342391");
    TRY(x = MPN("12345678", 9));                              TRY(s = x.str());  TEST_EQUAL(s, "6053444");
    TRY(x = MPN("123456789", 10));                            TRY(s = x.str());  TEST_EQUAL(s, "123456789");
    TRY(x = MPN("123456789a", 11));                           TRY(s = x.str());  TEST_EQUAL(s, "2853116705");
    TRY(x = MPN("123456789ab", 12));                          TRY(s = x.str());  TEST_EQUAL(s, "73686780563");
    TRY(x = MPN("123456789abc", 13));                         TRY(s = x.str());  TEST_EQUAL(s, "2103299351334");
    TRY(x = MPN("123456789abcd", 14));                        TRY(s = x.str());  TEST_EQUAL(s, "65751519677857");
    TRY(x = MPN("123456789abcde", 15));                       TRY(s = x.str());  TEST_EQUAL(s, "2234152501943159");
    TRY(x = MPN("123456789abcdef", 16));                      TRY(s = x.str());  TEST_EQUAL(s, "81985529216486895");
    TRY(x = MPN("123456789abcdefg", 17));                     TRY(s = x.str());  TEST_EQUAL(s, "3231407272993502984");
    TRY(x = MPN("123456789abcdefgh", 18));                    TRY(s = x.str());  TEST_EQUAL(s, "136146740744970718253");
    TRY(x = MPN("123456789abcdefghi", 19));                   TRY(s = x.str());  TEST_EQUAL(s, "6106233505124424657789");
    TRY(x = MPN("123456789abcdefghij", 20));                  TRY(s = x.str());  TEST_EQUAL(s, "290464265927977839335179");
    TRY(x = MPN("123456789abcdefghijk", 21));                 TRY(s = x.str());  TEST_EQUAL(s, "14606467545964956303452810");
    TRY(x = MPN("123456789abcdefghijkl", 22));                TRY(s = x.str());  TEST_EQUAL(s, "774212873841767703847271481");
    TRY(x = MPN("123456789abcdefghijklm", 23));               TRY(s = x.str());  TEST_EQUAL(s, "43141462809603124037923621715");
    TRY(x = MPN("123456789abcdefghijklmn", 24));              TRY(s = x.str());  TEST_EQUAL(s, "2521239653781255433741174806887");
    TRY(x = MPN("123456789abcdefghijklmno", 25));             TRY(s = x.str());  TEST_EQUAL(s, "154197642309049519503282176123724");
    TRY(x = MPN("123456789abcdefghijklmnop", 26));            TRY(s = x.str());  TEST_EQUAL(s, "9849791328331451697274678861440325");
    TRY(x = MPN("123456789abcdefghijklmnopq", 27));           TRY(s = x.str());  TEST_EQUAL(s, "655956343554789600515162175472115225");
    TRY(x = MPN("123456789abcdefghijklmnopqr", 28));          TRY(s = x.str());  TEST_EQUAL(s, "45467109894723422308055868660308101251");
    TRY(x = MPN("123456789abcdefghijklmnopqrs", 29));         TRY(s = x.str());  TEST_EQUAL(s, "3275109889236238692043148233075275671534");
    TRY(x = MPN("123456789abcdefghijklmnopqrst", 30));        TRY(s = x.str());  TEST_EQUAL(s, "244817041729665873959571938168846611177169");
    TRY(x = MPN("123456789abcdefghijklmnopqrstu", 31));       TRY(s = x.str());  TEST_EQUAL(s, "18965749034136928842900714476732571951160815");
    TRY(x = MPN("123456789abcdefghijklmnopqrstuv", 32));      TRY(s = x.str());  TEST_EQUAL(s, "1520813358304789717173449357665226867487963103");
    TRY(x = MPN("123456789abcdefghijklmnopqrstuvw", 33));     TRY(s = x.str());  TEST_EQUAL(s, "126084023523204128749625029233070835487633968144");
    TRY(x = MPN("123456789abcdefghijklmnopqrstuvwx", 34));    TRY(s = x.str());  TEST_EQUAL(s, "10795811667005157035274610194040730536839248769117");
    TRY(x = MPN("123456789abcdefghijklmnopqrstuvwxy", 35));   TRY(s = x.str());  TEST_EQUAL(s, "953726210513969460165905219215837671424690827366389");
    TRY(x = MPN("123456789abcdefghijklmnopqrstuvwxyz", 36));  TRY(s = x.str());  TEST_EQUAL(s, "86846823611197163108337531226495015298096208677436155");

}

void test_rs_sci_mp_integer_unsigned_conversion_double() {

    MPN x;
    std::string s;

    TRY(x = MPN::from_double(0.0));            TRY(s = x.str());  TEST_EQUAL(s, "0");
    TRY(x = MPN::from_double(0.99));           TRY(s = x.str());  TEST_EQUAL(s, "0");
    TRY(x = MPN::from_double(1.0));            TRY(s = x.str());  TEST_EQUAL(s, "1");
    TRY(x = MPN::from_double(123456789.0));    TRY(s = x.str());  TEST_EQUAL(s, "123456789");
    TRY(x = MPN::from_double(1.23456789e40));  TRY(s = x.str());  TEST_MATCH(s, "^12345678\\d{33}$");

}

void test_rs_sci_mp_integer_unsigned_arithmetic() {

    MPN x, y, z, q, r;
    std::string s;

    TRY(x = 0);
    TEST_EQUAL(x.bits(), 0u);
    TRY(s = x.str(2));   TEST_EQUAL(s, "0");
    TRY(s = x.str(10));  TEST_EQUAL(s, "0");
    TRY(s = x.str(16));  TEST_EQUAL(s, "0");

    TRY(y = x + 15);
    TEST_EQUAL(y.bits(), 4u);
    TRY(s = to_string(y));  TEST_EQUAL(s, "15");
    TRY(s = y.str(16));     TEST_EQUAL(s, "f");

    TRY(y = 15 - x);
    TEST_EQUAL(y.bits(), 4u);
    TRY(s = to_string(y));  TEST_EQUAL(s, "15");
    TRY(s = y.str(16));     TEST_EQUAL(s, "f");

    TRY(x = 0x123456789abcdef0ull);
    TRY(y = 0xffffffffffffffffull);
    TEST_EQUAL(x.bits(), 61u);
    TEST_EQUAL(y.bits(), 64u);
    TRY(s = to_string(x));  TEST_EQUAL(s, "1311768467463790320");
    TRY(s = to_string(y));  TEST_EQUAL(s, "18446744073709551615");

    TRY(z = x + 15);
    TEST_EQUAL(z.bits(), 61u);
    TRY(s = to_string(z));  TEST_EQUAL(s, "1311768467463790335");
    TRY(s = z.str(16));     TEST_EQUAL(s, "123456789abcdeff");

    TRY(z = x + y);
    TEST_EQUAL(z.bits(), 65u);
    TRY(s = to_string(z));  TEST_EQUAL(s, "19758512541173341935");
    TRY(s = z.str(16));     TEST_EQUAL(s, "1123456789abcdeef");

    TRY(z = y - 15);
    TEST_EQUAL(z.bits(), 64u);
    TRY(s = to_string(z));  TEST_EQUAL(s, "18446744073709551600");
    TRY(s = z.str(16));     TEST_EQUAL(s, "fffffffffffffff0");

    TRY(z = y - x);
    TEST_EQUAL(z.bits(), 64u);
    TRY(s = to_string(z));  TEST_EQUAL(s, "17134975606245761295");
    TRY(s = z.str(16));     TEST_EQUAL(s, "edcba9876543210f");

    TRY(z = x * y);
    TEST_EQUAL(z.bits(), 125u);
    TRY(s = to_string(z));  TEST_EQUAL(s, "24197857203266734862169780735577366800");
    TRY(s = z.str(16));     TEST_EQUAL(s, "123456789abcdeefedcba98765432110");

    TRY(x = MPN("123456789123456789123456789123456789123456789"));
    TRY(y = MPN("123456789123456789123456789123456789"));
    TRY(z = x - y);
    TEST_EQUAL(z, MPN("123456789000000000000000000000000000000000000"));
    TRY(y = MPN("123456789123456789123456789123456789000000000"));
    TRY(z = x - y);
    TEST_EQUAL(z, MPN("123456789"));
    TRY(x = MPN("123456789123456789123456789123456789123456789"));
    TRY(y = MPN("1357913579135791357913579"));
    TRY(z = x - y);
    TEST_EQUAL(z, MPN("123456789123456789122098875544320997765543210"));

    TRY(x = MPN("123456789123456789123456789123456789123456789"));
    TRY(y = MPN("123456789"));
    TRY(q = x / y);
    TRY(r = x % y);
    TEST_EQUAL(q, MPN("1000000001000000001000000001000000001"));
    TEST_EQUAL(r, MPN("0"));
    TRY(y = MPN("987654321"));
    TRY(q = x / y);
    TRY(r = x % y);
    TEST_EQUAL(q, MPN("124999998985937499000175780249997801"));
    TEST_EQUAL(r, MPN("725308668"));
    TRY(y = MPN("987654321987654321987654321987654321987654321"));
    TRY(q = x / y);
    TRY(r = x % y);
    TEST_EQUAL(q, MPN("0"));
    TEST_EQUAL(r, MPN("123456789123456789123456789123456789123456789"));
    TRY(y = {});

}

void test_rs_sci_mp_integer_unsigned_arithmetic_powers() {

    MPN x, y;
    std::string s;

    TRY(x = 0);   TRY(y = x.pow(0));   TEST_EQUAL(y.str(), "1");
    TRY(x = 0);   TRY(y = x.pow(1));   TEST_EQUAL(y.str(), "0");
    TRY(x = 0);   TRY(y = x.pow(2));   TEST_EQUAL(y.str(), "0");
    TRY(x = 0);   TRY(y = x.pow(3));   TEST_EQUAL(y.str(), "0");
    TRY(x = 1);   TRY(y = x.pow(0));   TEST_EQUAL(y.str(), "1");
    TRY(x = 1);   TRY(y = x.pow(1));   TEST_EQUAL(y.str(), "1");
    TRY(x = 1);   TRY(y = x.pow(2));   TEST_EQUAL(y.str(), "1");
    TRY(x = 1);   TRY(y = x.pow(3));   TEST_EQUAL(y.str(), "1");
    TRY(x = 10);  TRY(y = x.pow(0));   TEST_EQUAL(y.str(), "1");
    TRY(x = 10);  TRY(y = x.pow(1));   TEST_EQUAL(y.str(), "10");
    TRY(x = 10);  TRY(y = x.pow(2));   TEST_EQUAL(y.str(), "100");
    TRY(x = 10);  TRY(y = x.pow(3));   TEST_EQUAL(y.str(), "1000");
    TRY(x = 10);  TRY(y = x.pow(4));   TEST_EQUAL(y.str(), "10000");
    TRY(x = 10);  TRY(y = x.pow(5));   TEST_EQUAL(y.str(), "100000");
    TRY(x = 10);  TRY(y = x.pow(6));   TEST_EQUAL(y.str(), "1000000");
    TRY(x = 10);  TRY(y = x.pow(7));   TEST_EQUAL(y.str(), "10000000");
    TRY(x = 10);  TRY(y = x.pow(8));   TEST_EQUAL(y.str(), "100000000");
    TRY(x = 10);  TRY(y = x.pow(9));   TEST_EQUAL(y.str(), "1000000000");
    TRY(x = 10);  TRY(y = x.pow(10));  TEST_EQUAL(y.str(), "10000000000");
    TRY(x = 10);  TRY(y = x.pow(11));  TEST_EQUAL(y.str(), "100000000000");
    TRY(x = 10);  TRY(y = x.pow(12));  TEST_EQUAL(y.str(), "1000000000000");
    TRY(x = 10);  TRY(y = x.pow(13));  TEST_EQUAL(y.str(), "10000000000000");
    TRY(x = 10);  TRY(y = x.pow(14));  TEST_EQUAL(y.str(), "100000000000000");
    TRY(x = 10);  TRY(y = x.pow(15));  TEST_EQUAL(y.str(), "1000000000000000");
    TRY(x = 10);  TRY(y = x.pow(16));  TEST_EQUAL(y.str(), "10000000000000000");
    TRY(x = 10);  TRY(y = x.pow(17));  TEST_EQUAL(y.str(), "100000000000000000");
    TRY(x = 10);  TRY(y = x.pow(18));  TEST_EQUAL(y.str(), "1000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(19));  TEST_EQUAL(y.str(), "10000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(20));  TEST_EQUAL(y.str(), "100000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(21));  TEST_EQUAL(y.str(), "1000000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(22));  TEST_EQUAL(y.str(), "10000000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(23));  TEST_EQUAL(y.str(), "100000000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(24));  TEST_EQUAL(y.str(), "1000000000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(25));  TEST_EQUAL(y.str(), "10000000000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(26));  TEST_EQUAL(y.str(), "100000000000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(27));  TEST_EQUAL(y.str(), "1000000000000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(28));  TEST_EQUAL(y.str(), "10000000000000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(29));  TEST_EQUAL(y.str(), "100000000000000000000000000000");
    TRY(x = 10);  TRY(y = x.pow(30));  TEST_EQUAL(y.str(), "1000000000000000000000000000000");

}

void test_rs_sci_mp_integer_unsigned_bit_operations() {

    MPN x, y, z;
    std::string s;

    TEST_EQUAL(x.bits_set(), 0u);
    TEST(x.is_even());
    TEST(! x.is_odd());

    TRY(x = 0x123456789abcdef0ull);
    TRY(y = 0xffffffffffffffffull);
    TEST_EQUAL(x.bits(), 61u);
    TEST_EQUAL(y.bits(), 64u);
    TEST_EQUAL(x.bits_set(), 32u);
    TEST_EQUAL(y.bits_set(), 64u);
    TEST(x.is_even());
    TEST(! x.is_odd());
    TEST(! y.is_even());
    TEST(y.is_odd());
    TRY(s = to_string(x));  TEST_EQUAL(s, "1311768467463790320");
    TRY(s = to_string(y));  TEST_EQUAL(s, "18446744073709551615");

    TRY(z = x & y);
    TEST_EQUAL(z.bits(), 61u);
    TRY(s = to_string(z));  TEST_EQUAL(s, "1311768467463790320");
    TRY(s = z.str(16));  TEST_EQUAL(s, "123456789abcdef0");

    TRY(z = x | y);
    TEST_EQUAL(z.bits(), 64u);
    TRY(s = to_string(z));  TEST_EQUAL(s, "18446744073709551615");
    TRY(s = z.str(16));  TEST_EQUAL(s, "ffffffffffffffff");

    TRY(z = x ^ y);
    TEST_EQUAL(z.bits(), 64u);
    TRY(s = to_string(z));  TEST_EQUAL(s, "17134975606245761295");
    TRY(s = z.str(16));  TEST_EQUAL(s, "edcba9876543210f");

    TRY(z = x >> 0);   TRY(s = z.str(16));  TEST_EQUAL(s, "123456789abcdef0");
    TRY(z = x >> 1);   TRY(s = z.str(16));  TEST_EQUAL(s, "91a2b3c4d5e6f78");
    TRY(z = x >> 2);   TRY(s = z.str(16));  TEST_EQUAL(s, "48d159e26af37bc");
    TRY(z = x >> 3);   TRY(s = z.str(16));  TEST_EQUAL(s, "2468acf13579bde");
    TRY(z = x >> 31);  TRY(s = z.str(16));  TEST_EQUAL(s, "2468acf1");
    TRY(z = x >> 32);  TRY(s = z.str(16));  TEST_EQUAL(s, "12345678");
    TRY(z = x >> 33);  TRY(s = z.str(16));  TEST_EQUAL(s, "91a2b3c");
    TRY(z = x >> 58);  TRY(s = z.str(16));  TEST_EQUAL(s, "4");
    TRY(z = x >> 59);  TRY(s = z.str(16));  TEST_EQUAL(s, "2");
    TRY(z = x >> 60);  TRY(s = z.str(16));  TEST_EQUAL(s, "1");
    TRY(z = x >> 61);  TRY(s = z.str(16));  TEST_EQUAL(s, "0");
    TRY(z = x >> 62);  TRY(s = z.str(16));  TEST_EQUAL(s, "0");
    TRY(z = x >> 63);  TRY(s = z.str(16));  TEST_EQUAL(s, "0");
    TRY(z = x >> 64);  TRY(s = z.str(16));  TEST_EQUAL(s, "0");
    TRY(z = x >> 65);  TRY(s = z.str(16));  TEST_EQUAL(s, "0");

    TRY(z = x << 0);   TRY(s = z.str(16));  TEST_EQUAL(s, "123456789abcdef0");
    TRY(z = x << 1);   TRY(s = z.str(16));  TEST_EQUAL(s, "2468acf13579bde0");
    TRY(z = x << 2);   TRY(s = z.str(16));  TEST_EQUAL(s, "48d159e26af37bc0");
    TRY(z = x << 3);   TRY(s = z.str(16));  TEST_EQUAL(s, "91a2b3c4d5e6f780");
    TRY(z = x << 4);   TRY(s = z.str(16));  TEST_EQUAL(s, "123456789abcdef00");
    TRY(z = x << 5);   TRY(s = z.str(16));  TEST_EQUAL(s, "2468acf13579bde00");
    TRY(z = x << 6);   TRY(s = z.str(16));  TEST_EQUAL(s, "48d159e26af37bc00");
    TRY(z = x << 7);   TRY(s = z.str(16));  TEST_EQUAL(s, "91a2b3c4d5e6f7800");
    TRY(z = x << 8);   TRY(s = z.str(16));  TEST_EQUAL(s, "123456789abcdef000");
    TRY(z = x << 9);   TRY(s = z.str(16));  TEST_EQUAL(s, "2468acf13579bde000");
    TRY(z = x << 10);  TRY(s = z.str(16));  TEST_EQUAL(s, "48d159e26af37bc000");
    TRY(z = x << 11);  TRY(s = z.str(16));  TEST_EQUAL(s, "91a2b3c4d5e6f78000");
    TRY(z = x << 12);  TRY(s = z.str(16));  TEST_EQUAL(s, "123456789abcdef0000");
    TRY(z = x << 13);  TRY(s = z.str(16));  TEST_EQUAL(s, "2468acf13579bde0000");
    TRY(z = x << 14);  TRY(s = z.str(16));  TEST_EQUAL(s, "48d159e26af37bc0000");
    TRY(z = x << 15);  TRY(s = z.str(16));  TEST_EQUAL(s, "91a2b3c4d5e6f780000");
    TRY(z = x << 16);  TRY(s = z.str(16));  TEST_EQUAL(s, "123456789abcdef00000");
    TRY(z = x << 17);  TRY(s = z.str(16));  TEST_EQUAL(s, "2468acf13579bde00000");
    TRY(z = x << 18);  TRY(s = z.str(16));  TEST_EQUAL(s, "48d159e26af37bc00000");
    TRY(z = x << 19);  TRY(s = z.str(16));  TEST_EQUAL(s, "91a2b3c4d5e6f7800000");
    TRY(z = x << 20);  TRY(s = z.str(16));  TEST_EQUAL(s, "123456789abcdef000000");

    TRY(x = {});
    TEST(! x.get_bit(0));
    TEST(! x.get_bit(100));

    TRY(x.set_bit(16));
    TEST_EQUAL(x, MPN("0x10000"));
    TEST(! x.get_bit(15));
    TEST(x.get_bit(16));
    TEST(! x.get_bit(17));

    TRY(x.set_bit(80));
    TEST_EQUAL(x, MPN("0x100000000000000010000"));
    TEST(! x.get_bit(79));
    TEST(x.get_bit(80));
    TEST(! x.get_bit(81));

    TRY(x.set_bit(80, false));
    TEST_EQUAL(x, MPN("0x10000"));
    TEST(! x.get_bit(80));

    TRY(x.flip_bit(80));
    TEST_EQUAL(x, MPN("0x100000000000000010000"));
    TEST(x.get_bit(80));

    TRY(x.flip_bit(80));
    TEST_EQUAL(x, MPN("0x10000"));
    TEST(! x.get_bit(80));

}

void test_rs_sci_mp_integer_unsigned_byte_operations() {

    MPN a, b;
    ByteVector v;

    TEST_EQUAL(a.bytes(), 0u);

    TRY(a = MPN("0x12"));                    TEST_EQUAL(a.bytes(), 1u);
    TRY(a = MPN("0x1234"));                  TEST_EQUAL(a.bytes(), 2u);
    TRY(a = MPN("0x123456"));                TEST_EQUAL(a.bytes(), 3u);
    TRY(a = MPN("0x12345678"));              TEST_EQUAL(a.bytes(), 4u);
    TRY(a = MPN("0x123456789a"));            TEST_EQUAL(a.bytes(), 5u);
    TRY(a = MPN("0x123456789abc"));          TEST_EQUAL(a.bytes(), 6u);
    TRY(a = MPN("0x123456789abcde"));        TEST_EQUAL(a.bytes(), 7u);
    TRY(a = MPN("0x123456789abcdef1"));      TEST_EQUAL(a.bytes(), 8u);
    TRY(a = MPN("0x123456789abcdef123"));    TEST_EQUAL(a.bytes(), 9u);
    TRY(a = MPN("0x123456789abcdef12345"));  TEST_EQUAL(a.bytes(), 10u);

    TEST_EQUAL(a.get_byte(0), 0x45);
    TEST_EQUAL(a.get_byte(1), 0x23);
    TEST_EQUAL(a.get_byte(2), 0xf1);
    TEST_EQUAL(a.get_byte(3), 0xde);
    TEST_EQUAL(a.get_byte(4), 0xbc);
    TEST_EQUAL(a.get_byte(5), 0x9a);
    TEST_EQUAL(a.get_byte(6), 0x78);
    TEST_EQUAL(a.get_byte(7), 0x56);
    TEST_EQUAL(a.get_byte(8), 0x34);
    TEST_EQUAL(a.get_byte(9), 0x12);
    TEST_EQUAL(a.get_byte(10), 0u);
    TEST_EQUAL(a.get_byte(11), 0u);
    TEST_EQUAL(a.get_byte(12), 0u);
    TEST_EQUAL(a.get_byte(13), 0u);
    TEST_EQUAL(a.get_byte(14), 0u);
    TEST_EQUAL(a.get_byte(15), 0u);
    TEST_EQUAL(a.get_byte(16), 0u);

    TRY(a.set_byte(1, 0xff));   TEST_EQUAL(a.str(16), "123456789abcdef1ff45");
    TRY(a.set_byte(3, 0xff));   TEST_EQUAL(a.str(16), "123456789abcfff1ff45");
    TRY(a.set_byte(5, 0xff));   TEST_EQUAL(a.str(16), "12345678ffbcfff1ff45");
    TRY(a.set_byte(7, 0xff));   TEST_EQUAL(a.str(16), "1234ff78ffbcfff1ff45");
    TRY(a.set_byte(9, 0xff));   TEST_EQUAL(a.str(16), "ff34ff78ffbcfff1ff45");
    TRY(a.set_byte(11, 0xff));  TEST_EQUAL(a.str(16), "ff00ff34ff78ffbcfff1ff45");
    TRY(a.set_byte(13, 0xff));  TEST_EQUAL(a.str(16), "ff00ff00ff34ff78ffbcfff1ff45");
    TRY(a.set_byte(15, 0xff));  TEST_EQUAL(a.str(16), "ff00ff00ff00ff34ff78ffbcfff1ff45");

    TRY(a = 0);
    TRY(b = MPN("0x123456789abcdef12345"));

    v.resize(7);

    TRY(a.write_be(v.data(), v.size()));  TEST_EQUAL(hexdump(v), "00 00 00 00 00 00 00");
    TRY(b.write_be(v.data(), v.size()));  TEST_EQUAL(hexdump(v), "78 9a bc de f1 23 45");

    TRY(a.write_le(v.data(), v.size()));  TEST_EQUAL(hexdump(v), "00 00 00 00 00 00 00");
    TRY(b.write_le(v.data(), v.size()));  TEST_EQUAL(hexdump(v), "45 23 f1 de bc 9a 78");

    v = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};

    TRY(a = MPN::read_be(v.data(), v.size()));  TEST_EQUAL(a.str(16), "112233445566778899aabbccddeeff");
    TRY(a = MPN::read_le(v.data(), v.size()));  TEST_EQUAL(a.str(16), "ffeeddccbbaa998877665544332211");

}
