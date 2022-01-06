// This file is generated by the rs-update-tests script

#include "rs-unit-test.hpp"

int main(int argc, char** argv) {

    RS::UnitTest::begin_tests(argc, argv);

    // version-test.cpp
    UNIT_TEST(rs_sci_version)

    // algorithms-test.cpp
    UNIT_TEST(rs_sci_algorithms_binomial_coefficients)
    UNIT_TEST(rs_sci_algorithms_interpolation)
    UNIT_TEST(rs_sci_algorithms_interpolated_map)
    UNIT_TEST(rs_sci_algorithms_interpolated_map_lin_log)
    UNIT_TEST(rs_sci_algorithms_interpolated_map_log_log)
    UNIT_TEST(rs_sci_algorithms_cubic_spline_map)
    UNIT_TEST(rs_sci_algorithms_precision_sum)
    UNIT_TEST(rs_sci_algorithms_line_integral)
    UNIT_TEST(rs_sci_algorithms_volume_integral)
    UNIT_TEST(rs_sci_algorithms_find_optimum)

    // constants-test.cpp
    UNIT_TEST(rs_sci_constants)

    // formula-test.cpp
    UNIT_TEST(rs_sci_formula_sphere_geometry)
    UNIT_TEST(rs_sci_formula_mass_density)
    UNIT_TEST(rs_sci_formula_surface_gravity)
    UNIT_TEST(rs_sci_formula_orbital_period)
    UNIT_TEST(rs_sci_formula_hill_sphere)
    UNIT_TEST(rs_sci_formula_stellar_luminosity)
    UNIT_TEST(rs_sci_formula_stellar_lifetime)
    UNIT_TEST(rs_sci_formula_schwarzschild_radius)
    UNIT_TEST(rs_sci_formula_magnitude_luminosity)
    UNIT_TEST(rs_sci_formula_bolometric_correction)
    UNIT_TEST(rs_sci_formula_planet_temperature)

    // rational-test.cpp
    UNIT_TEST(rs_sci_rational_basics)
    UNIT_TEST(rs_sci_rational_reduction)
    UNIT_TEST(rs_sci_rational_arithmetic)
    UNIT_TEST(rs_sci_rational_properties)
    UNIT_TEST(rs_sci_rational_comparison)
    UNIT_TEST(rs_sci_rational_mixed)
    UNIT_TEST(rs_sci_rational_parsing)
    UNIT_TEST(rs_sci_rational_hash)

    // random-test.cpp
    UNIT_TEST(rs_sci_random_lcg_generators)
    UNIT_TEST(rs_sci_random_pcg64)
    UNIT_TEST(rs_sci_random_uniform_integer_distribution)
    UNIT_TEST(rs_sci_random_uniform_real_distribution)
    UNIT_TEST(rs_sci_random_bernoulli_distribution)
    UNIT_TEST(rs_sci_random_normal_distribution)
    UNIT_TEST(rs_sci_random_log_normal_distribution)
    UNIT_TEST(rs_sci_random_log_uniform_distribution)
    UNIT_TEST(rs_sci_random_constrained_distribution)
    UNIT_TEST(rs_sci_random_weighted_distribution)

    // statistics-test.cpp
    UNIT_TEST(rs_sci_statistics_univariate)
    UNIT_TEST(rs_sci_statistics_bivariate)
    UNIT_TEST(rs_sci_statistics_combination)

    // unit-test.cpp

    return RS::UnitTest::end_tests();

}
