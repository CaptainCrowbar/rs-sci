#include "rs-sci/approximate.hpp"
#include "rs-regex/regex.hpp"
#include <cmath>
#include <cstdlib>
#include <stdexcept>

using namespace RS::Format;
using namespace RS::RE;

namespace RS::Sci {

    Approx::Approx(const std::string& str, parse mode) {
        // TODO
        (void)str;
        (void)mode;
    }

    std::string Approx::str(FormatSpec spec) const {
        spec.default_prec(10);
        char error_format = spec.find_option("pPq"); // p = V±E, P = V+/-E, q = V(E)
        // TODO
        (void)spec;
        (void)error_format;
        return {};
    }

}
