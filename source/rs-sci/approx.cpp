#include "rs-sci/approx.hpp"
#include "rs-format/format.hpp"
#include "rs-format/string.hpp"
#include "rs-regex/regex.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <stdexcept>

using namespace RS::Format;
using namespace RS::RE;

namespace RS::Sci {

    namespace {

        int dec_scale(double x) {
            // Return the exponent of the largest power of 10 <= |x|
            if (x == 0)
                return 0;
            else
                return int(std::floor(std::log10(std::abs(x))));
        }

    }

    Approx::Approx(const std::string& str, parse mode) {

        if (mode == parse::exact) {
            value_ = to_double(str);
            return;
        }

        static constexpr const char* sig_part = R"([+-]?(?:\d+(?:\.\d*)?|\.\d+))";
        static constexpr const char* exp_part = R"((?:[Ee][+-]?\d+)?)";
        static const Regex pair_pattern(std::string()
            + "(" + sig_part + exp_part + ")"
            + R"(\s*(?:±|\+/?-)\s*)"
            + "(" + sig_part + exp_part + ")",
            Regex::full | Regex::optimize);
        static const Regex ulps_pattern(std::string()
            + "(" + sig_part + ")"
            + R"(\((\d+)\))"
            + "(" + exp_part + ")",
            Regex::full | Regex::optimize);
        static const Regex id_pattern(R"(\(|±|\+/?-)", Regex::optimize);

        auto match = id_pattern(str);

        if (! match) {

            // Significant digits format

            value_ = to_double(str);

            if (value_ != 0) {
                auto dpos = std::find_if(str.begin(), str.end(), ascii_isdigit);
                auto epos = std::find_if(dpos, str.end(), [] (char c) { return c == 'E' || c == 'e'; });
                int digits = int(std::count_if(dpos, epos, ascii_isdigit));
                int scale = dec_scale(value_);
                double error = 5 * std::pow(10.0, double(scale - digits));
                set_error(error);
            }

        } else if (*match.begin() == '(') {

            // Ulps format: significand (ulps) [exponent]

            match = ulps_pattern(str);

            if (! match)
                throw std::invalid_argument("Invalid approximate value: " + quote(str));

            std::string sig(match[1]);
            std::string ulp(match[2]);
            std::string exp(match[3]);

            value_ = to_double(sig + exp);
            auto error = to_double(ulp + exp);
            size_t dot = sig.find('.');
            if (dot != npos) {
                auto sig_decimals = sig.size() - dot - 1;
                error *= std::pow(10.0, - double(sig_decimals));
            }
            set_error(error);

        } else {

            // Paired format: value ± error

            match = pair_pattern(str);

            if (! match)
                throw std::invalid_argument("Invalid approximate value: " + quote(str));

            std::string vstr(match[1]);
            std::string estr(match[2]);
            value_ = to_double(vstr);
            set_error(to_double(estr));

        }

    }

    std::string Approx::str(FormatSpec spec) const {

        // Options supported

        // e = Use a comma for the decimal point (European convention)
        // p = Use paired format: V±E
        // P = Use paired format: V+/-E
        // q = Use ulps format: V(E) (default)
        // r = Use rounded format: V
        // S = Always show a sign on the exponent
        // s = Always show a sign

        // Grouping and strip-zeroes options are not supported

        spec.no_option("AaCcEUuWwz");
        spec.default_prec(10);

        if (is_exact())
            return format_object(value_, spec);

        char error_format = spec.find_option("Ppqr");

        if (error_format == 'P' || error_format == 'p') {

            // Paired format: V±E

            // TODO

            return {};

        } else if (error_format == 'r') {

            // Rounded format: V

            // TODO

            return {};

        } else {

            // Ulps format: V(E) (default)

            static const FormatSpec ulp_spec("f0");

            int vscale = dec_scale(value_);
            int escale = dec_scale(error_);
            int vdigits = std::max(vscale - escale, 0) + 2;
            int edigits = vdigits + escale - vscale;
            auto vspec = spec;
            vspec.set_prec(vdigits);
            auto str = format_object(value_, vspec);
            double ulps = error_ * std::pow(10.0, double(edigits - escale - 1));
            auto ustr = '(' + format_object(ulps, ulp_spec) + ')';
            size_t epos = str.find_first_of("Ee");
            if (epos == npos)
                str += ustr;
            else
                str.insert(epos, ustr);

            return str;

        }

    }

}
