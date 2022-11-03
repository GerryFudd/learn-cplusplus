#include <string>
#include <math/BigInt.hpp>

namespace std {
    BigInt::BigInt (unsigned int magnitude[], unsigned int length, bool disc) {
        magnitude_pointer = magnitude;
        magnitude_length = length;
        discriminator = disc;
    };
    string BigInt::as_string() {
        string result = "";
        if (discriminator) {
            result += "-";
        }
        if (magnitude_length == 1) {
            result += to_string(*magnitude_pointer);
        }
        return result;
    }
}
