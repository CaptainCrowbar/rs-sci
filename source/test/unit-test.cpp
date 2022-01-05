// This file is generated by the rs-update-tests script

#include "rs-unit-test.hpp"

int main(int argc, char** argv) {

    RS::UnitTest::begin_tests(argc, argv);

    // version-test.cpp
    UNIT_TEST(rs_sci_version)

    // algorithms-test.cpp
    UNIT_TEST(rs_sci_algorithm_binomial_coefficients)
    UNIT_TEST(rs_sci_algorithm_find_optimum)
    UNIT_TEST(rs_sci_algorithm_precision_sum)

    // constants-test.cpp
    UNIT_TEST(rs_sci_constants)

    // statistics-test.cpp
    UNIT_TEST(rs_sci_statistics_univariate)
    UNIT_TEST(rs_sci_statistics_bivariate)
    UNIT_TEST(rs_sci_statistics_combination)

    // unit-test.cpp

    return RS::UnitTest::end_tests();

}
