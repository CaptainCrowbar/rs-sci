#include "rs-sci/random.hpp"
#include "rs-sci/statistics.hpp"
#include "rs-graphics-core/vector.hpp"
#include "rs-unit-test.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <map>
#include <numeric>
#include <string>
#include <vector>

using namespace RS::Graphics::Core;
using namespace RS::Sci;
using namespace RS::Sci::Detail;

void test_rs_sci_random_siphash() {

    static constexpr std::array<uint64_t, 64> vectors_sip64 = {{
        0x726f'db47'dd0e'0e31ull, 0x74f8'39c5'93dc'67fdull, 0x0d6c'8009'd9a9'4f5aull, 0x8567'6696'd7fb'7e2dull,
        0xcf27'94e0'2771'87b7ull, 0x1876'5564'cd99'a68dull, 0xcbc9'466e'58fe'e3ceull, 0xab02'00f5'8b01'd137ull,
        0x93f5'f579'9a93'2462ull, 0x9e00'82df'0ba9'e4b0ull, 0x7a5d'bbc5'94dd'b9f3ull, 0xf4b3'2f46'226b'ada7ull,
        0x751e'8fbc'860e'e5fbull, 0x14ea'5627'c084'3d90ull, 0xf723'ca90'8e7a'f2eeull, 0xa129'ca61'49be'45e5ull,
        0x3f2a'cc7f'57c2'9bdbull, 0x699a'e9f5'2cbe'4794ull, 0x4bc1'b3f0'968d'd39cull, 0xbb6d'c91d'a779'61bdull,
        0xbed6'5cf2'1aa2'ee98ull, 0xd0f2'cbb0'2e3b'67c7ull, 0x9353'6795'e3a3'3e88ull, 0xa80c'038c'cd5c'cec8ull,
        0xb8ad'50c6'f649'af94ull, 0xbce1'92de'8a85'b8eaull, 0x17d8'35b8'5bbb'15f3ull, 0x2f2e'6163'076b'cfadull,
        0xde4d'aaac'a71d'c9a5ull, 0xa6a2'5066'8795'6571ull, 0xad87'a353'5c49'ef28ull, 0x32d8'92fa'd841'c342ull,
        0x7127'512f'72f2'7cceull, 0xa7f3'2346'f959'78e3ull, 0x12e0'b01a'bb05'1238ull, 0x15e0'34d4'0fa1'97aeull,
        0x314d'ffbe'0815'a3b4ull, 0x0279'90f0'2962'3981ull, 0xcadc'd4e5'9ef4'0c4dull, 0x9abf'd876'6a33'735cull,
        0x0e3e'a96b'5304'a7d0ull, 0xad0c'42d6'fc58'5992ull, 0x1873'06c8'9bc2'15a9ull, 0xd4a6'0abc'f379'2b95ull,
        0xf935'451d'e4f2'1df2ull, 0xa953'8f04'1975'5787ull, 0xdb9a'cddf'f56c'a510ull, 0xd06c'98cd'5c09'75ebull,
        0xe612'a3cb'9ecb'a951ull, 0xc766'e62c'fcad'af96ull, 0xee64'435a'9752'fe72ull, 0xa192'd576'b245'165aull,
        0x0a87'87bf'8ecb'74b2ull, 0x81b3'e73d'20b4'9b6full, 0x7fa8'220b'a3b2'eceaull, 0x2457'31c1'3ca4'2499ull,
        0xb78d'bfaf'3a8d'83bdull, 0xea1a'd565'322a'1a0bull, 0x60e6'1c23'a379'5013ull, 0x6606'd7e4'4628'2b93ull,
        0x6ca4'ecb1'5c5f'91e1ull, 0x9f62'6da1'5c96'25f3ull, 0xe51b'3860'8ef2'5f57ull, 0x958a'324c'eb06'4572ull,
    }};

    static constexpr uint64_t key0 = 0x0706050403020100ull;
    static constexpr uint64_t key1 = 0x0f0e0d0c0b0a0908ull;

    SipHash sip(key0, key1);
    std::array<uint8_t, 64> in;
    std::iota(in.begin(), in.end(), uint8_t(0));
    uint64_t out;

    for (int i = 0; i < 64; ++i) {
        TRY(out = sip(in.data(), i));
        TEST_EQUAL(out, vectors_sip64[i]);
    }

}

void test_rs_sci_random_lcg_generators() {

    static constexpr int iterations = 1'000'000;

    static constexpr double max32 = double(~ uint32_t(0));
    static constexpr double max64 = double(~ uint64_t(0));
    static constexpr double mean32 = max32 / 2;
    static constexpr double mean64 = max64 / 2;
    static const double sd32 = std::sqrt((std::ldexp(1.0, 64) - 1) / 12);
    static const double sd64 = std::sqrt((std::ldexp(1.0, 128) - 1) / 12);

    Lcg32 rng32(42);
    Statistics<double> stats;
    uint32_t x = 0;

    for (int i = 0; i < iterations; ++i) {
        TRY(x = rng32());
        stats(x);
    }

    TEST_NEAR(stats.mean(), mean32, 1e7);
    TEST_NEAR(stats.sd(), sd32, 1e7);

    Lcg64 rng64(42);
    uint64_t y = 0;
    stats.clear();

    for (int i = 0; i < iterations; ++i) {
        TRY(y = rng64());
        stats(y);
    }

    TEST_NEAR(stats.mean(), mean64, 1e16);
    TEST_NEAR(stats.sd(), sd64, 1e16);

}

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

void test_rs_sci_random_splitmix64() {

    static const std::vector<uint64_t> expect = {
        0xe220'a839'7b1d'cdafull, 0x6e78'9e6a'a1b9'65f4ull, 0x06c4'5d18'8009'454full, 0xf88b'b8a8'724c'81ecull,
        0x1b39'896a'51a8'749bull, 0x53cb'9f0c'747e'a2eaull, 0x2c82'9abe'1f45'32e1ull, 0xc584'133a'c916'ab3cull,
        0x3ee5'7890'41c9'8ac3ull, 0xf3b8'488c'368c'b0a6ull, 0x657e'ecdd'3cb1'3d09ull, 0xc2d3'26e0'055b'def6ull,
        0x8621'a03f'e0bb'db7bull, 0x8e1f'7555'983a'a92full, 0xb54e'0f16'00cc'4d19ull, 0x84bb'3f97'971d'80abull,
        0x7d29'825c'7552'1255ull, 0xc3cf'1710'2b7f'7f86ull, 0x3466'e9a0'8391'4f64ull, 0xd81a'8d2b'5a44'85acull,
        0xdb01'602b'100b'9ed7ull, 0xa903'8a92'1825'f10dull, 0xedf5'f1d9'0dca'2f6aull, 0x5449'6ad6'7bd2'634cull,
        0xdd7c'01d4'f540'7269ull, 0x935e'82f1'db4c'4f7bull, 0x69b8'2ebc'9223'3300ull, 0x40d2'9eb5'7de1'd510ull,
        0xa2f0'9dab'b45c'6316ull, 0xee52'1d7a'0f4d'3872ull, 0xf169'52ee'72f3'454full, 0x377d'35de'a8e4'0225ull,
        0x0c7d'e806'4963'bab0ull, 0x0558'2d37'111a'c529ull, 0xd254'741f'599d'c6f7ull, 0x6963'0f75'93d1'08c3ull,
        0x417e'f961'81da'a383ull, 0x3c3c'41a3'b433'43a1ull, 0x6e19'905d'cbe5'31dfull, 0x4fa9'fa73'2485'1729ull,
        0x84eb'4454'a792'922aull, 0x134f'7096'9181'75ceull, 0x07dc'930b'3022'78a8ull, 0x12c0'15a9'7019'e937ull,
        0xcc06'c316'52eb'f438ull, 0xecee'6563'0a69'1e37ull, 0x3e84'ecb1'763e'79adull, 0x690e'd476'743a'ae49ull,
        0x7746'15d7'b1a1'f2e1ull, 0x22b3'53f0'4f4f'52daull, 0xe3dd'd86b'a71a'5eb1ull, 0xdf26'8ade'b651'3356ull,
        0x2098'eb73'd436'7d77ull, 0x03d6'8453'23ce'3c71ull, 0xc952'c562'0043'c714ull, 0x9b19'6bca'844f'1705ull,
        0x3026'0345'dd9e'0ec1ull, 0xcf44'8a58'82bb'9698ull, 0xf4a5'78dc'cbc8'7656ull, 0xbfde'aed9'a17b'3c8full,
        0xed79'402d'1d5c'5d7bull, 0x55f0'70ab'1cbb'f170ull, 0x3e00'a349'29a8'8f1dull, 0xe255'b237'b8bb'18fbull,
    };

    SplitMix64 rng;
    uint64_t x = 0;

    for (auto y: expect) {
        TRY(x = rng());
        TEST_EQUAL(x, y);
    }

}

void test_rs_sci_random_xoshiro256ss() {

    static const std::vector<uint64_t> expect = {
        0x99ec'5f36'cb75'f2b4ull, 0xbf6e'1f78'4956'452aull, 0x1a5f'849d'4933'e6e0ull, 0x6aa5'94f1'262d'2d2cull,
        0xbba5'ad4a'1f84'2e59ull, 0xffef'8375'd9eb'cacaull, 0x6c16'0dee'd2f5'4c98ull, 0x8920'ad64'8fc3'0a3full,
        0xdb03'2c0b'a753'9731ull, 0xeb3a'475a'3e74'9a3dull, 0x1d42'993f'a43f'2a54ull, 0x1136'1bf5'26a1'4bb5ull,
        0x1b4f'07a5'ab3d'8e9cull, 0xa7a3'257f'6986'db7full, 0x7efd'aa95'605d'fc9cull, 0x4bde'97c0'a78e'aab8ull,
        0xb455'eac4'3518'666cull, 0x304d'bf6c'0673'0690ull, 0x8cbe'7776'598a'798cull, 0x0ecb'df7f'fcd7'27e5ull,
        0x4ff5'2157'533f'e270ull, 0x7e61'475b'8724'2f2eull, 0x5255'8c68'a931'6824ull, 0xa0bd'00c5'9247'1176ull,
        0xfc9b'83a3'a0c6'3b9eull, 0x4d78'6c0f'0a8b'88efull, 0xa524'73c4'f62f'2338ull, 0xe9dc'0037'db25'd6d9ull,
        0xfce5'eba9'd250'94c3ull, 0xe3db'e61e'e2d6'4b51ull, 0x23f6'2e43'2b12'72dfull, 0x4ac7'443a'342c'4913ull,
        0xc31c'f1a9'658c'1991ull, 0x290c'97ff'ce91'8b1dull, 0xf544'55e0'2e90'636aull, 0xf577'4575'8bb8'f33full,
        0xe5e1'b685'1228'23d9ull, 0x2c16'cde0'fd80'97ecull, 0x3cde'bc44'a5bc'1936ull, 0x6833'bafa'723c'2dbdull,
        0xb6fa'6c4b'a1d3'd39eull, 0xe5b9'32b6'56c2'edc3ull, 0x09cf'0b61'2161'5c9full, 0x214e'25d5'7fc6'36d5ull,
        0xcf3d'1721'806e'2537ull, 0xcf79'6fc6'335d'dc02ull, 0x353c'8b86'489b'0322ull, 0xfc48'6582'2547'b6aaull,
        0xe8c9'3d84'ee8b'3f8cull, 0xd1b4'2120'a323'f2d6ull, 0xa73a'11d2'47ff'36b2ull, 0xae42'2369'58bb'a58cull,
        0xb622'679e'2aff'cf3aull, 0xcc3b'ab00'60f6'45f4ull, 0x2e01'e45c'78f0'daa7ull, 0x0856'6c5f'16be'948aull,
        0x73be'ac21'87e1'f640ull, 0x8e90'3d75'2c1b'5d6eull, 0x5b34'6810'94d7'511dull, 0x70eb'ad38'2047'f5c1ull,
        0xeae5'ca14'48d4'e9ccull, 0x3d2d'6277'5b63'1bd5ull, 0x8cb7'2ebc'5b4f'7dc3ull, 0x099c'2939'ea69'0a80ull,
    };

    Xoshiro rng;
    uint64_t x = 0;

    for (auto y: expect) {
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

    TEST_NEAR(census[0] / double(iterations), 0.2, 0.002);
    TEST_NEAR(census[1] / double(iterations), 0.2, 0.002);
    TEST_NEAR(census[2] / double(iterations), 0.2, 0.002);
    TEST_NEAR(census[3] / double(iterations), 0.2, 0.002);
    TEST_NEAR(census[4] / double(iterations), 0.2, 0.002);

    TRY(dist = UniformInteger(1, 5));
    census.clear();

    for (int i = 0; i < iterations; ++i) {
        TRY(x = dist(rng));
        ++census[x];
    }

    TEST_EQUAL(census.size(), 5u);

    TEST_NEAR(census[1] / double(iterations), 0.2, 0.002);
    TEST_NEAR(census[2] / double(iterations), 0.2, 0.002);
    TEST_NEAR(census[3] / double(iterations), 0.2, 0.002);
    TEST_NEAR(census[4] / double(iterations), 0.2, 0.002);
    TEST_NEAR(census[5] / double(iterations), 0.2, 0.002);

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

    dist = LogNormal<double>(2, 1, LogMode::common);
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

void test_rs_sci_random_poisson_distribution() {

    static constexpr int iterations = 1'000'000;

    PoissonDistribution<int> poi;
    Statistics<double> stats;
    Pcg64 rng(42);
    int x = 0;

    TRY(poi = PoissonDistribution<int>(4));
    TEST_EQUAL(poi.mean(), 4);
    TEST_EQUAL(poi.sd(), 2);

    for (int i = 0; i < iterations; ++i) {
        TRY(x = poi(rng));
        stats(x);
    }

    TEST_NEAR(stats.mean(), poi.mean(), 0.002);
    TEST_NEAR(stats.sd(), poi.sd(), 0.002);

    TRY(poi = PoissonDistribution<int>(100));
    TEST_EQUAL(poi.mean(), 100);
    TEST_EQUAL(poi.sd(), 10);

    stats.clear();

    for (int i = 0; i < iterations; ++i) {
        TRY(x = poi(rng));
        stats(x);
    }

    TEST_NEAR(stats.mean(), poi.mean(), 0.2);
    TEST_NEAR(stats.sd(), poi.sd(), 0.2);

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

void test_rs_sci_random_choice_distribution() {

    static constexpr int iterations = 1'000'000;

    Pcg64 rng(42);
    RandomChoice<char> choice;
    std::map<char, int> census;
    std::string alpha = "abcdefghij";
    char c;

    TRY(choice = RandomChoice<char>(alpha));
    TEST_EQUAL(choice.size(), 10u);

    for (int i = 0; i < iterations; ++i) {
        TRY(c = choice(rng));
        ++census[c];
    }

    TEST_EQUAL(census.size(), 10u);

    TEST_NEAR(census['a'] / double(iterations), 0.1, 0.001);
    TEST_NEAR(census['b'] / double(iterations), 0.1, 0.001);
    TEST_NEAR(census['c'] / double(iterations), 0.1, 0.001);
    TEST_NEAR(census['d'] / double(iterations), 0.1, 0.001);
    TEST_NEAR(census['e'] / double(iterations), 0.1, 0.001);
    TEST_NEAR(census['f'] / double(iterations), 0.1, 0.001);
    TEST_NEAR(census['g'] / double(iterations), 0.1, 0.001);
    TEST_NEAR(census['h'] / double(iterations), 0.1, 0.001);
    TEST_NEAR(census['i'] / double(iterations), 0.1, 0.001);
    TEST_NEAR(census['j'] / double(iterations), 0.1, 0.001);

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

void test_rs_sci_random_vectors() {

    static constexpr int iterations = 1'000'000;
    static constexpr Double3 apex(5, 10, 15);

    Pcg64 rng(42);
    RandomVector<double, 3> rv(apex);
    Double3 low(100.0);
    Double3 high(-100.0);
    Double3 sum(0.0);
    Double3 sum2(0.0);
    Double3 v;

    for (int i = 0; i < iterations; ++i) {
        TRY(v = rv(rng));
        sum += v;
        for (int j = 0; j < 3; ++j) {
            low[j] = std::min(low[j], v[j]);
            high[j] = std::max(high[j], v[j]);
            sum2[j] += v[j] * v[j];
        }
    }

    Double3 mean = sum / iterations;
    Double3 sd;

    for (int i = 0; i < 3; ++i)
        sd[i] = std::sqrt(sum2[i] / iterations - mean[i] * mean[i]);

    TEST(low[0] > 0);
    TEST(low[1] > 0);
    TEST(low[2] > 0);
    TEST(high[0] < 5);
    TEST(high[1] < 10);
    TEST(high[2] < 15);
    TEST_NEAR(mean[0], 2.5, 0.01);
    TEST_NEAR(mean[1], 5, 0.01);
    TEST_NEAR(mean[2], 7.5, 0.01);
    TEST_NEAR(sd[0], 1.443376, 0.01);
    TEST_NEAR(sd[1], 2.886751, 0.01);
    TEST_NEAR(sd[2], 4.330127, 0.01);

    SymmetricRandomVector<double, 3> srv(apex);
    low = Double3(100);
    high = Double3(-100);
    sum = Double3::null();
    sum2 = Double3::null();

    for (int i = 0; i < iterations; ++i) {
        TRY(v = srv(rng));
        sum += v;
        for (int j = 0; j < 3; ++j) {
            low[j] = std::min(low[j], v[j]);
            high[j] = std::max(high[j], v[j]);
            sum2[j] += v[j] * v[j];
        }
    }

    mean = sum / iterations;

    for (int i = 0; i < 3; ++i)
        sd[i] = std::sqrt(sum2[i] / iterations - mean[i] * mean[i]);

    TEST(low[0] > -5);
    TEST(low[1] > -10);
    TEST(low[2] > -15);
    TEST(high[0] < 5);
    TEST(high[1] < 10);
    TEST(high[2] < 15);
    TEST_NEAR(mean[0], 0, 0.01);
    TEST_NEAR(mean[1], 0, 0.01);
    TEST_NEAR(mean[2], 0, 0.01);
    TEST_NEAR(sd[0], 2.886751, 0.01);
    TEST_NEAR(sd[1], 5.773503, 0.01);
    TEST_NEAR(sd[2], 8.660254, 0.01);

}

namespace {

    template <int N>
    void random_point_in_sphere_test() {

        static constexpr int iterations = 100'000;

        Pcg64 rng(42);
        RandomPointInSphere<double, N> gen(10);
        auto count_sides = Vector<double, N>();
        auto total = Vector<double, N>();
        Vector<double, N> point;
        double count_inner = 0;

        for (int i = 0; i < iterations; ++i) {
            TRY(point = gen(rng));
            for (int j = 0; j < N; ++j)
                if (point[j] > 0)
                    ++count_sides[j];
            if (point.r() < 5)
                ++count_inner;
            total += point;
        }

        double epsilon = 10 / std::sqrt(double(iterations));
        double expect_inner = std::pow(2.0, - double(N));

        for (auto& c: count_sides) {
            c /= iterations;
            TEST_NEAR(c, 0.5, epsilon);
        }

        count_inner /= iterations;
        total /= iterations;

        TEST_NEAR(count_inner, expect_inner, epsilon);
        TEST_NEAR(total.r(), 0, epsilon);

    }

    template <int N>
    void random_direction_test() {

        static constexpr int iterations = 100'000;

        Pcg64 rng(42);
        RandomDirection<double, N> gen;
        auto count_sides = Vector<double, N>();
        auto total = Vector<double, N>();
        Vector<double, N> point;

        for (int i = 0; i < iterations; ++i) {
            TRY(point = gen(rng));
            TEST_NEAR(point.r(), 1, 1e-6);
            for (int j = 0; j < N; ++j)
                if (point[j] > 0)
                    ++count_sides[j];
            total += point;
        }

        double epsilon = 2 / std::sqrt(double(iterations));

        for (auto& c: count_sides) {
            c /= iterations;
            TEST_NEAR(c, 0.5, epsilon);
        }

        total /= iterations;

        TEST_NEAR(total.r(), 0, 2 * epsilon); // more variance in surface points

    }

}

void test_rs_sci_random_point_in_sphere() {

    random_point_in_sphere_test<1>();
    random_point_in_sphere_test<2>();
    random_point_in_sphere_test<3>();
    random_point_in_sphere_test<4>();
    random_point_in_sphere_test<5>();
    random_point_in_sphere_test<6>();
    random_point_in_sphere_test<7>();
    random_point_in_sphere_test<8>();
    random_point_in_sphere_test<9>();
    random_point_in_sphere_test<10>();

}

void test_rs_sci_random_direction() {

    random_direction_test<1>();
    random_direction_test<2>();
    random_direction_test<3>();
    random_direction_test<4>();
    random_direction_test<5>();
    random_direction_test<6>();
    random_direction_test<7>();
    random_direction_test<8>();
    random_direction_test<9>();
    random_direction_test<10>();

}
