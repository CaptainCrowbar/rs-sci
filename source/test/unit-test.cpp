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

    // mp-integer-unsigned-test.cpp
    UNIT_TEST(rs_sci_mp_integer_unsigned_conversion_integers)
    UNIT_TEST(rs_sci_mp_integer_unsigned_conversion_strings)
    UNIT_TEST(rs_sci_mp_integer_unsigned_conversion_base_strings)
    UNIT_TEST(rs_sci_mp_integer_unsigned_conversion_double)
    UNIT_TEST(rs_sci_mp_integer_unsigned_arithmetic)
    UNIT_TEST(rs_sci_mp_integer_unsigned_arithmetic_powers)
    UNIT_TEST(rs_sci_mp_integer_unsigned_bit_operations)
    UNIT_TEST(rs_sci_mp_integer_unsigned_byte_operations)

    // mp-integer-signed-test.cpp
    UNIT_TEST(rs_sci_mp_integer_signed_conversion_integers)
    UNIT_TEST(rs_sci_mp_integer_signed_conversion_strings)
    UNIT_TEST(rs_sci_mp_integer_signed_conversion_base_strings)
    UNIT_TEST(rs_sci_mp_integer_signed_conversion_double)
    UNIT_TEST(rs_sci_mp_integer_signed_arithmetic)
    UNIT_TEST(rs_sci_mp_integer_signed_division)
    UNIT_TEST(rs_sci_mp_integer_signed_large_arithmetic)
    UNIT_TEST(rs_sci_mp_integer_signed_powers)

    // mp-integer-sign-agnostic-test.cpp
    UNIT_TEST(rs_sci_mp_integer_literals)
    UNIT_TEST(rs_sci_mp_integer_core_functions)
    UNIT_TEST(rs_sci_mp_integer_hash_set)

    // mp-integer-rational-test.cpp
    UNIT_TEST(rs_sci_mp_integer_rational_basics)
    UNIT_TEST(rs_sci_mp_integer_rational_reduction)
    UNIT_TEST(rs_sci_mp_integer_rational_arithmetic)
    UNIT_TEST(rs_sci_mp_integer_rational_properties)
    UNIT_TEST(rs_sci_mp_integer_rational_comparison)
    UNIT_TEST(rs_sci_mp_integer_rational_mixed)
    UNIT_TEST(rs_sci_mp_integer_rational_parsing)

    // prime-test.cpp
    UNIT_TEST(rs_sci_prime_iterator_int)
    UNIT_TEST(rs_sci_prime_iterator_fixed_binary)
    UNIT_TEST(rs_sci_prime_iterator_mp_integer)
    UNIT_TEST(rs_sci_prime_primality_int)
    UNIT_TEST(rs_sci_prime_primality_fixed_binary)
    UNIT_TEST(rs_sci_prime_primality_mp_integer)
    UNIT_TEST(rs_sci_prime_next_prev_prime_int)
    UNIT_TEST(rs_sci_prime_next_prev_prime_fixed_binary)
    UNIT_TEST(rs_sci_prime_next_prev_prime_mp_integer)
    UNIT_TEST(rs_sci_prime_factorization_int)
    UNIT_TEST(rs_sci_prime_factorization_fixed_binary)
    UNIT_TEST(rs_sci_prime_factorization_mp_integer)
    UNIT_TEST(rs_sci_prime_list_primes_int)
    UNIT_TEST(rs_sci_prime_list_primes_fixed_binary)
    UNIT_TEST(rs_sci_prime_list_primes_mp_integer)

    // random-hash-test.cpp
    UNIT_TEST(rs_sci_random_siphash)

    // random-lcg-test.cpp
    UNIT_TEST(rs_sci_random_lcg_generators)

    // random-pcg-test.cpp
    UNIT_TEST(rs_sci_random_pcg64)

    // random-xoshiro-test.cpp
    UNIT_TEST(rs_sci_random_splitmix64)
    UNIT_TEST(rs_sci_random_xoshiro256ss)

    // random-discrete-test.cpp
    UNIT_TEST(rs_sci_random_bernoulli_distribution)
    UNIT_TEST(rs_sci_random_uniform_integer_distribution)
    UNIT_TEST(rs_sci_random_poisson_distribution)

    // random-continuous-test.cpp
    UNIT_TEST(rs_sci_random_uniform_real_distribution_properties)
    UNIT_TEST(rs_sci_random_uniform_real_distribution)
    UNIT_TEST(rs_sci_random_log_uniform_distribution)
    UNIT_TEST(rs_sci_random_normal_distribution_properties)
    UNIT_TEST(rs_sci_random_normal_distribution)
    UNIT_TEST(rs_sci_random_log_normal_distribution)

    // random-adapter-test.cpp
    UNIT_TEST(rs_sci_random_constrained_distribution)

    // random-non-arithmetic-test.cpp
    UNIT_TEST(rs_sci_random_choice_distribution)
    UNIT_TEST(rs_sci_random_weighted_distribution)
    UNIT_TEST(rs_sci_random_uuid)

    // random-spatial-test.cpp
    UNIT_TEST(rs_sci_random_vectors)
    UNIT_TEST(rs_sci_random_point_in_sphere)
    UNIT_TEST(rs_sci_random_direction)

    // statistics-test.cpp
    UNIT_TEST(rs_sci_statistics_univariate)
    UNIT_TEST(rs_sci_statistics_bivariate)
    UNIT_TEST(rs_sci_statistics_combination)

    // unit-test.cpp

    return RS::UnitTest::end_tests();

}
