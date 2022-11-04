#include <string>
#include <math/BigInt.hpp>

using namespace std;

namespace math {
    const unsigned int decimal_conversion_base = 1000000000;
    const unsigned int conversion_quotient = 4;
    const unsigned int conversion_remainder = 294967296;
    BigInt::BigInt () {
        unsigned int magnitude = 0;
        magnitude_pointer = &magnitude;
        magnitude_length = 1;
        discriminator = false;
    }

    BigInt::BigInt (unsigned int magnitude[], unsigned int length, bool disc) {
        magnitude_pointer = magnitude;
        magnitude_length = length;
        discriminator = disc;
    };

    string BigInt::as_decimal_string() {
        string result = "";
        unsigned int last_unprocessed_place = magnitude_length - 1;
        unsigned int unprocessed_magnitude[magnitude_length];
        for (int i = 0; i < magnitude_length; i++) {
            unprocessed_magnitude[i] = *(magnitude_pointer + i);
        }
        unsigned long new_last_place_value, previous_remainder, current_quotient, current_remainder, current_place;
        while (last_unprocessed_place > 0) {
            new_last_place_value = unprocessed_magnitude[last_unprocessed_place] / decimal_conversion_base;
            previous_remainder = unprocessed_magnitude[last_unprocessed_place] % decimal_conversion_base;
            unprocessed_magnitude[last_unprocessed_place] = new_last_place_value;
            current_place = last_unprocessed_place;
            while (current_place > 0) {
                current_place--;
                current_quotient = unprocessed_magnitude[current_place] / decimal_conversion_base;
                current_remainder = unprocessed_magnitude[current_place] % decimal_conversion_base;
                unprocessed_magnitude[current_place] = previous_remainder * conversion_quotient + current_quotient
                    + (previous_remainder * conversion_remainder + current_remainder) / decimal_conversion_base;
                previous_remainder = (previous_remainder * conversion_remainder + current_remainder) % decimal_conversion_base;
            }
            string remainder_as_string = to_string(previous_remainder);
            string leading_zeros;
            if (remainder_as_string.length() < 9) {
                leading_zeros = string(9 - remainder_as_string.length(), '0');
            } else {
                leading_zeros = "";
            }
            result = leading_zeros + remainder_as_string + result;
            if (new_last_place_value == 0) {
                last_unprocessed_place--;
            }
        }
        if (unprocessed_magnitude[0] > 0) {
            result = to_string(unprocessed_magnitude[0]) + result;
        }
        if (result == "") {
            result = "0";
        } else if (discriminator) {
            result = "-" + result;
        }
        return result;
    }
}
