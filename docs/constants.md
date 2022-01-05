# Constants

_[Scientific Library by Ross Smith](index.html)_

```c++
#include "rs-sci/constants.hpp"
namespace RS::Sci;
```

The values quoted here for the constants are approximate; the values actually
supplied are accurate to the precision of the type (for mathematical
constants) or to the best available precision (for physical constants).

## Contents

* TOC
{:toc}

## Mathematical constants

```c++
#define RS_DEFINE_CONSTANT(name, value) \
    constexpr float name ## _f = value ## f; \
    constexpr double name ## _d = value; \
    constexpr long double name ## _ld = value ## l; \
    template <typename T> constexpr T name ## _c = T(name ## _ld);
#define RS_DEFINE_CONSTANT_2(name, symbol, value) \
    constexpr float name ## _f = value ## f; \
    constexpr float symbol ## _f = name ## _f; \
    constexpr double name ## _d = value; \
    constexpr double symbol ## _d = name ## _d; \
    constexpr long double name ## _ld = value ## l; \
    constexpr long double symbol ## _ld = name ## _ld; \
    template <typename T> constexpr T name ## _c = T(name ## _ld); \
    template <typename T> constexpr T symbol ## _c = name ## _c<T>;
```

Define a floating point constant, in several forms. This generates a set of
three named constants for the standard floating point types (or six if both a
name and symbol are used), and a variable template.

The list of mathematical constants is the union of the Posix and C++20 lists;
spelling is mostly taken from C++, except that I have used a different suffix
convention to avoid collisions.

| Constant           | Definition          | Value     |
| --------           | ----------          | -----     |
| `e`                | e                   | 2.718282  |
| `ln2`              | log<sub>e</sub> 2   | 0.693147  |
| `ln10`             | log<sub>e</sub> 10  | 2.302585  |
| `log2e`            | log<sub>2</sub> e   | 1.442695  |
| `log10e`           | log<sub>10</sub> e  | 0.434294  |
| `pi`               | π                   | 3.141593  |
| `pi_over_2`        | π/2                 | 1.570796  |
| `pi_over_4`        | π/4                 | 0.785398  |
| `inv_pi`           | 1/π                 | 0.318310  |
| `inv_sqrtpi`       | 1/√π                | 0.564190  |
| `two_over_pi`      | 2/π                 | 0.636620  |
| `two_over_sqrtpi`  | 2/√π                | 1.128379  |
| `sqrt2`            | √2                  | 1.414214  |
| `sqrt3`            | √3                  | 1.732051  |
| `inv_sqrt2`        | 1/√2                | 0.707107  |
| `inv_sqrt3`        | 1/√3                | 0.577350  |
| `egamma`           | γ (Euler constant)  | 0.577216  |
| `phi`              | φ (golden ratio)    | 1.618034  |

## Conversion factors

| Constant         | Value                   |
| --------         | -----                   |
| `arcsec`         | 4.84814e-6 rad          |
| `arcmin`         | 2.90888e-4 rad          |
| `degree`         | 0.0174533 rad           |
| `inch`           | 0.0254 m                |
| `foot`           | 0.3048 m                |
| `yard`           | 0.9144 m                |
| `mile`           | 1609.344 m              |
| `nautical_mile`  | 1852 m                  |
| `ounce`          | 0.0283495 kg            |
| `pound`          | 0.453592 kg             |
| `short_ton`      | 907.185 kg              |
| `long_ton`       | 1016.05 kg              |
| `pound_force`    | 4.44822 N               |
| `erg`            | 10<sup>-7</sup> J       |
| `foot_pound`     | 1.35582 J               |
| `calorie`        | 4.184 J                 |
| `kilocalorie`    | 4184 J                  |
| `ton_tnt`        | 4.184×10<sup>9</sup> J  |
| `horsepower`     | 745.700 W               |
| `mmHg`           | 133.322 Pa              |
| `atmosphere`     | 101325 Pa               |
| `zero_celsius`   | 273.15 K                |

# Physical constants

Sources:

* [CODATA recommended values](https://physics.nist.gov/cuu/Constants/)

Values related to quantity of substance are given in kilomoles to avoid
intrusive factors of 1000 in expressions.

| Constant                     | Symbol       | Value                                                     |
| --------                     | ------       | -----                                                     |
| `alpha_particle_mass`        | `m_alpha`    | 6.64466e-27 kg                                            |
| `atomic_mass_constant`       | `m_u`        | 1.66054e-27 kg                                            |
| `avogadro_constant`          | `N_A`        | 6.02214e26 kmol<sup>-1</sup>                              |
| `boltzmann_constant`         | `k`          | 1.38065e-23 J/K                                           |
| `electron_mass`              | `m_e`        | 9.10938e-31 kg                                            |
| `elementary_charge`          | `ec`         | 1.60218e-19 C                                             |
| `fine_structure_constant`    | `alpha`      | 7.29735e-3 dimensionless                                  |
| `first_radiation_constant`   | `c_1`        | 3.74177e-16 W m<sup>2</sup>                               |
| `gravitational_constant`     | `G`          | 6.67428e-11 m<sup>3</sup> kg<sup>-1</sup> s<sup>-2</sup>  |
| `josephson_constant`         | `K_J`        | 4.83598e14 Hz V<sup>-1</sup>                              |
| `molar_gas_constant`         | `R`          | 8314.46 J kmol<sup>-1</sup> K<sup>-1</sup>                |
| `neutron_mass`               | `m_n`        | 1.67493e-27 kg                                            |
| `planck_constant`            | `h`          | 6.62607e-34 J s                                           |
| `proton_mass`                | `m_p`        | 1.67262e-27 kg                                            |
| `rydberg_constant`           | `R_inf`      | 1.09737e7 m<sup>-1</sup>                                  |
| `second_radiation_constant`  | `c_2`        | 0.0143878e-2 m K                                          |
| `speed_of_light`             | `c`          | 2.99792e8 m/s                                             |
| `stefan_boltzmann_constant`  | `sigma`      | 5.67037e-8 W m<sup>-2</sup> K<sup>-4</sup>                |
| `vacuum_impedance`           | `Z_0`        | 376.730 Ω                                                 |
| `vacuum_permeability`        | `mu_0`       | 1.25664e-6 N A<sup>-2</sup>                               |
| `vacuum_permittivity`        | `epsilon_0`  | 8.85419e-12 F m<sup>-1</sup>                              |
| `von_klitzing_constant`      | `R_K`        | 25812.8. Ω                                                |
| `wien_constant`              | `b_W`        | 2.89777e-3 m K                                            |

## Astronomical constants

Sources:

* [IAU current best estimates](https://iau-a3.gitlab.io/NSFA/NSFA_cbe.html)
* [IERS useful constants](https://hpiers.obspm.fr/eop-pc/models/constants.html)

| Constant                          | Symbol      | Value                     |
| --------                          | ------      | -----                     |
| `astronomical_unit`               | `au`        | 1.49598e11 m              |
| `light_year`                      | `ly`        | 9.46073e15 m              |
| `parsec`                          | `pc`        | 3.08568e16 m              |
| `earth_mass`                      | `M_earth`   | 5.97219e24 kg             |
| `earth_radius`                    | `R_earth`   | 6.37814e6 m               |
| `standard_gravity`                | `g_0`       | 9.80665 m s<sup>-2</sup>  |
| `julian_day`                      | `jd`        | 86400 s                   |
| `sidereal_day`                    | `d_sid`     | 86164.1 s                 |
| `tropical_year`                   | `y_trop`    | 3.15569e7 s               |
| `julian_year`                     | `jy`        | 3.15576e7 s               |
| `sidereal_year`                   | `y_sid`     | 3.15581e7 s               |
| `lunar_mass`                      | `M_moon`    | 7.34581e22 kg             |
| `lunar_radius`                    | `R_moon`    | 1.7381e6 m                |
| `lunar_distance`                  | `a_moon`    | 3.84399e8 m               |
| `sidereal_month`                  | `mon_sid`   | 2.36059e6 s               |
| `synodic_month`                   | `mon_syn`   | 2.55144e6 s               |
| `jupiter_mass`                    | `M_jup`     | 1.89852e27 kg             |
| `jupiter_radius`                  | `R_jup`     | 7.1492e7 m                |
| `solar_mass`                      | `M_sun`     | 1.98842e30 kg             |
| `solar_radius`                    | `R_sun`     | 6.957e8 m                 |
| `solar_temperature`               | `T_sun`     | 5772 K                    |
| `solar_luminosity`                | `L_sun`     | 3.828e26 W                |
| `solar_visual_magnitude`          | `Mv_sun`    | 4.81 mag                  |
| `solar_bolometric_magnitude`      | `Mbol_sun`  | 4.74 mag                  |
| `solar_bolometric_correction`     | `BC_sun`    | -0.07 mag                 |
| `bolometric_luminosity_constant`  | `L_0`       | 3.0128e28 W               |
| `age_of_universe`                 | `t_0`       | 4.3508e17 s               |
