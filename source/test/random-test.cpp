#include "rs-sci/random.hpp"
#include "rs-sci/statistics.hpp"
#include "rs-unit-test.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <map>
#include <string>
#include <vector>

using namespace RS::Sci;

void test_rs_sci_random_pcg64() {

    static const std::vector<uint64_t> expect1 = {
        0xcf7d'be68'4e0c'4045ull, 0x1564'2875'dfe1'e67cull, 0x32f0'49df'2f50'd811ull, 0x98c1'd0a1'63e1'f856ull,
        0x0743'e58f'0360'd766ull, 0xfee9'e4fc'0347'9bf6ull, 0xf7ce'cb16'2663'9417ull, 0x2df8'59db'9576'2826ull,
        0x1c3f'9381'2e14'8ea4ull, 0x8af3'831f'2bb8'5205ull, 0x18a8'332d'd6ff'efc9ull, 0xf4d9'e8a9'f8a2'4f80ull,
        0x93ae'319f'c35b'2252ull, 0x72db'88f7'e0fc'64e7ull, 0x33c3'afd9'5136'5990ull, 0xb466'b761'b08a'33e0ull,
        0x819b'd59c'9546'97d9ull, 0x1e12'dfcf'3603'd13cull, 0xb65d'9c1f'4263'1174ull, 0x879c'9407'8269'79cdull,
        0x2e28'0ad2'288f'501cull, 0x1c4a'7b23'9437'9f32ull, 0x7f0a'705c'9e02'c72dull, 0xbe49'b435'ae70'f9daull,
        0xc685'41ce'0c31'758dull, 0x906d'8d98'5d14'140full, 0xe376'87a6'39e5'9e4bull, 0x4c86'f84e'1bd6'f7a6ull,
        0x8d94'ae5b'c2ef'4fffull, 0x399f'88cd'fc35'c9b8ull, 0xbd2b'35c1'1c2e'bda4ull, 0xfd3b'3cc7'91f4'c41bull,
        0x4cee'57ea'75a2'c553ull, 0x3ee4'0429'e17e'08ccull, 0xd4e8'b915'a77f'55d2ull, 0x17b0'c80b'5f47'7bbdull,
        0x32ea'b935'0284'9a9aull, 0xe459'97b6'4cd4'b70dull, 0x344b'89a2'783b'3d44ull, 0x0dd1'cfdd'02d3'6391ull,
        0x3fac'997d'8735'0aa6ull, 0x42c0'87c9'45dd'1658ull, 0x0742'24ac'0bb4'ca7full, 0xec19'189c'e139'33f5ull,
        0xdb6f'4363'7724'3bc7ull, 0xdd86'c642'78e5'7554ull, 0x97a0'66f7'2130'10c5ull, 0x51fe'e8d4'06fb'0559ull,
        0x1be0'4809'd240'd333ull, 0xdb42'6032'6880'20a3ull, 0x4a23'd545'5f31'7b2cull, 0x10ef'6b20'5c4b'4f76ull,
        0xa44a'b230'4e80'c901ull, 0xda79'6dab'0f57'0ff7ull, 0xebbe'0cde'e293'2dfdull, 0xe658'e297'513b'80beull,
        0xb2fc'bccc'4873'715bull, 0x0fbf'3bb2'ff94'dadeull, 0xf5c9'52ff'7541'4f85ull, 0x94fa'ed41'788e'c226ull,
        0xb699'cd1d'bc63'9cd9ull, 0xc402'a50c'737e'0a62ull, 0x95fa'c59a'4269'c9baull, 0x84ee'24cf'64cb'80a4ull,
    };

    static const std::vector<uint64_t> expect2 = {
        0x7e68'4885'6818'e376ull, 0xc7a3'ca18'7fec'4f97ull, 0x4bf5'9a39'22b1'5522ull, 0x8424'4cd3'df75'a1eaull,
        0xadbf'c289'c379'431eull, 0xb86e'b9b4'b36d'a00eull, 0x474b'9deb'd7a7'061cull, 0x3354'0fe3'b963'9b67ull,
        0x4625'c32b'c26b'36bfull, 0x7021'1a03'b27e'ee0full, 0x6877'db4a'31b9'0b48ull, 0x7161'475b'7d80'ef34ull,
        0x9b48'966a'b8d2'3125ull, 0x6859'd41c'6031'442cull, 0xb190'0069'cfdc'4a61ull, 0xc186'f98c'1f49'd15bull,
        0x2a22'1618'a716'6525ull, 0x6045'f3cc'7d9b'36fbull, 0x5e1c'6a28'a062'c630ull, 0x0fa1'e306'4314'2ee7ull,
        0xd3c8'1d7f'6f9a'914bull, 0xa53e'a651'4fb6'2041ull, 0x2db6'd388'd913'd064ull, 0xef30'01ba'12c1'8df1ull,
        0x79a0'baf7'3c99'79d7ull, 0xcada'9fb5'525c'abdfull, 0x69a1'7a68'7c33'c840ull, 0x6222'6119'de06'c52dull,
        0x95dd'4f9f'110a'9364ull, 0x53fc'5e57'07f2'93c2ull, 0x0613'6b7d'84ba'1ac6ull, 0xd130'4cd1'3f46'22e9ull,
        0x6208'fe06'6947'1579ull, 0xe8bc'14ef'466a'8241ull, 0x2886'5467'fc3a'c6e8ull, 0x4aa8'8ba5'0ab2'a873ull,
        0x1eca'26ce'a602'cad8ull, 0xe42d'feb7'96ca'b0e0ull, 0xd8df'1f37'3279'c71aull, 0x9e16'4fe3'9fa9'1d64ull,
        0x1c00'eed2'8930'bdf9ull, 0x05e4'73b7'aaa7'ae1dull, 0xb335'6dd9'fde7'c6bfull, 0xd2f3'3ea8'f019'75f7ull,
        0x35e4'c439'c899'3246ull, 0x4e93'dab0'21b5'6544ull, 0x92f7'4cf4'f789'c20dull, 0x1bac'6300'fa68'5808ull,
        0xbe35'bb87'5dbe'449aull, 0x87ef'616c'cab2'c820ull, 0x85fb'bbe9'ad49'0394ull, 0xd961'76fa'0e02'0ca1ull,
        0x826d'ddfa'd7c6'c56cull, 0x9781'30b7'471b'a6f3ull, 0x5de5'17ad'6dda'439dull, 0xba1f'b254'6d0d'f742ull,
        0x2468'efff'49b6'21c6ull, 0xfb40'6713'3c55'6926ull, 0x6c50'0e69'3ac7'a271ull, 0x1481'512e'2093'b0feull,
        0x08a8'4c90'76d8'e712ull, 0x8655'ad7a'fe94'3fe1ull, 0x184e'7e05'6017'4c53ull, 0x28f8'ca20'ed34'daeeull,
    };

    Pcg64 rng;
    uint64_t x = 0;

    for (auto y: expect1) {
        TRY(x = rng());
        TEST_EQUAL(x, y);
    }

    TRY(rng.advance(10000));

    for (auto y: expect2) {
        TRY(x = rng());
        TEST_EQUAL(x, y);
    }

    TRY(rng.advance(-10128));

    for (auto y: expect1) {
        TRY(x = rng());
        TEST_EQUAL(x, y);
    }

}

void test_rs_sci_random_uniform_integer_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    UniformInteger<int> dist(5);
    std::map<int, int> census;
    int x = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        ++census[x];
    }

    TEST_EQUAL(census.size(), 5u);

    TEST_NEAR(census[0] / double(iterations), 0.2, 0.001);
    TEST_NEAR(census[1] / double(iterations), 0.2, 0.001);
    TEST_NEAR(census[2] / double(iterations), 0.2, 0.001);
    TEST_NEAR(census[3] / double(iterations), 0.2, 0.001);
    TEST_NEAR(census[4] / double(iterations), 0.2, 0.001);

    TRY(dist = UniformInteger(1, 5));
    census.clear();

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        ++census[x];
    }

    TEST_EQUAL(census.size(), 5u);

    TEST_NEAR(census[1] / double(iterations), 0.2, 0.001);
    TEST_NEAR(census[2] / double(iterations), 0.2, 0.001);
    TEST_NEAR(census[3] / double(iterations), 0.2, 0.001);
    TEST_NEAR(census[4] / double(iterations), 0.2, 0.001);
    TEST_NEAR(census[5] / double(iterations), 0.2, 0.001);

}

void test_rs_sci_random_uniform_real_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    UniformReal<double> dist;
    Statistics<double> stats;
    double x = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        stats(x);
    }

    TEST_NEAR(stats.mean(), 0.5, 0.001);
    TEST_NEAR(stats.sd(), 0.2887, 0.001);
    TEST_NEAR(stats.min(), 0, 0.001);
    TEST_NEAR(stats.max(), 1, 0.001);

    TRY(dist = UniformReal<double>(-100, 100));
    stats.clear();

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        stats(x);
    }

    TEST_NEAR(stats.mean(), 0, 0.1);
    TEST_NEAR(stats.sd(), 57.74, 0.1);
    TEST_NEAR(stats.min(), -100, 0.1);
    TEST_NEAR(stats.max(), 100, 0.1);

}

void test_rs_sci_random_bernoulli_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    BernoulliDistribution dist;
    bool b = false;
    int n = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(b = dist(rng));
        n += int(b);
    }

    double p = double(n) / iterations;
    TEST_NEAR(p, 0.5, 0.001);

    TRY(dist = BernoulliDistribution(1, 10));
    n = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(b = dist(rng));
        n += int(b);
    }

    p = double(n) / iterations;
    TEST_NEAR(p, 0.1, 0.001);

    TRY(dist = BernoulliDistribution(0.9));
    n = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(b = dist(rng));
        n += int(b);
    }

    p = double(n) / iterations;
    TEST_NEAR(p, 0.9, 0.001);

}

void test_rs_sci_random_normal_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    NormalDistribution<double> norm;
    Statistics<double> stats;
    double x = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(x = norm(rng));
        stats(x);
    }

    TEST_NEAR(stats.mean(), 0, 0.001);
    TEST_NEAR(stats.sd(), 1, 0.001);

    TRY(norm = NormalDistribution<double>(100, 50));
    stats.clear();

    for (int i = 0; i < iterations; ++i) {
        TRY(x = norm(rng));
        stats(x);
    }

    TEST_NEAR(stats.mean(), 100, 0.1);
    TEST_NEAR(stats.sd(), 50, 0.1);

}

void test_rs_sci_random_log_normal_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    LogNormal<double> dist;
    Statistics<double> stats;
    double x = 0;
    double lx = 0;

    TEST_EQUAL(dist.m(), 0);
    TEST_EQUAL(dist.s(), 1);
    TEST_EQUAL(dist.median(), 1);
    TEST_NEAR(dist.s_factor(), 2.718'282, 1e-6);

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        lx = std::log(x);
        stats(lx);
    }

    TEST_NEAR(stats.mean(), 0, 0.001);
    TEST_NEAR(stats.sd(), 1, 0.001);

    dist = LogNormal<double>(2, 1, LogMode::decimal);
    stats.clear();

    TEST_NEAR(dist.m(), 4.605'170, 1e-6);
    TEST_NEAR(dist.s(), 2.302'585, 1e-6);
    TEST_NEAR(dist.median(), 100, 1e-6);
    TEST_NEAR(dist.s_factor(), 10, 1e-6);

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        lx = std::log(x);
        stats(lx);
    }

    TEST_NEAR(stats.mean(), 4.605, 0.01);
    TEST_NEAR(stats.sd(), 2.303, 0.01);

}

void test_rs_sci_random_log_uniform_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    LogUniform<double> dist(10, 100'000);
    Statistics<double> stats;
    std::array<int, 4> count = {{0,0,0,0}};
    double x = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        stats(x);
        if (x < 100)
            ++count[0];
        else if (x < 1'000)
            ++count[1];
        else if (x < 10'000)
            ++count[2];
        else
            ++count[3];
    }

    double fraction[4];
    for (int i = 0; i < 4; ++i)
        fraction[i] = count[i] / double(iterations);

    TEST_NEAR(fraction[0], 0.25, 0.001);
    TEST_NEAR(fraction[1], 0.25, 0.001);
    TEST_NEAR(fraction[2], 0.25, 0.001);
    TEST_NEAR(fraction[3], 0.25, 0.001);
    TEST_NEAR(stats.min(), 10, 0.01);
    TEST_NEAR(stats.max(), 100'000, 100);

}

void test_rs_sci_random_constrained_distribution() {

    using constrained_int = ConstrainedDistribution<UniformInteger<int>>;

    Pcg64 rng(42);
    constrained_int ci(1, 10, -20, 20);
    int min = 9999;
    int max = -9999;
    int x = 0;

    TEST_EQUAL(ci.min(), 1);
    TEST_EQUAL(ci.max(), 10);

    for (int i = 0; i < 100'000; ++i) {
        TRY(x = ci(rng));
        min = std::min(min, x);
        max = std::max(max, x);
    }

    TEST_EQUAL(min, 1);
    TEST_EQUAL(max, 10);

}

void test_rs_sci_random_weighted_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    WeightedChoice<std::string> wd;
    std::map<std::string, int> census;

    TEST_EQUAL(wd.total_weight(), 0);

    TRY(wd.add(1, "a"));
    TRY(wd.add(2, "b"));
    TRY(wd.add(3, "c"));
    TRY(wd.add(4, "d"));
    TRY(wd.add(5, "e"));
    TRY(wd.add(1, "f", "g", "h", "i", "j"));

    TEST_EQUAL(wd.total_weight(), 16);

    const std::string* sp = nullptr;

    for (int i = 0; i < iterations; ++i) {
        TRY(sp = &wd(rng));
        ++census[*sp];
    }

    TEST_EQUAL(census.size(), 10u);

    TEST_NEAR(census["a"] / double(iterations), 0.0625, 0.001);
    TEST_NEAR(census["b"] / double(iterations), 0.1250, 0.001);
    TEST_NEAR(census["c"] / double(iterations), 0.1875, 0.001);
    TEST_NEAR(census["d"] / double(iterations), 0.2500, 0.001);
    TEST_NEAR(census["e"] / double(iterations), 0.3125, 0.001);
    TEST_NEAR(census["f"] / double(iterations), 0.0125, 0.001);
    TEST_NEAR(census["g"] / double(iterations), 0.0125, 0.001);
    TEST_NEAR(census["h"] / double(iterations), 0.0125, 0.001);
    TEST_NEAR(census["i"] / double(iterations), 0.0125, 0.001);
    TEST_NEAR(census["j"] / double(iterations), 0.0125, 0.001);

    TRY((wd = {
        {10, "A"},
        {20, "B"},
        {30, "C"},
        {40, "D"},
        {50, "E"},
        {10, "F", "G", "H", "I", "J"},
    }));

    TEST_EQUAL(wd.total_weight(), 160);

    census.clear();

    for (int i = 0; i < iterations; ++i) {
        TRY(sp = &wd(rng));
        ++census[*sp];
    }

    TEST_EQUAL(census.size(), 10u);

    TEST_NEAR(census["A"] / double(iterations), 0.0625, 0.001);
    TEST_NEAR(census["B"] / double(iterations), 0.1250, 0.001);
    TEST_NEAR(census["C"] / double(iterations), 0.1875, 0.001);
    TEST_NEAR(census["D"] / double(iterations), 0.2500, 0.001);
    TEST_NEAR(census["E"] / double(iterations), 0.3125, 0.001);
    TEST_NEAR(census["F"] / double(iterations), 0.0125, 0.001);
    TEST_NEAR(census["G"] / double(iterations), 0.0125, 0.001);
    TEST_NEAR(census["H"] / double(iterations), 0.0125, 0.001);
    TEST_NEAR(census["I"] / double(iterations), 0.0125, 0.001);
    TEST_NEAR(census["J"] / double(iterations), 0.0125, 0.001);

}
