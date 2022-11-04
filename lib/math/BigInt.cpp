#include <string>
#include <algorithm>
#include <exception>
#include <iostream>
#include <math/BigInt.hpp>

using namespace std;

namespace math {

    NotImplemented::NotImplemented(string unimplemented_item) {
        message = ("Not implemented for " + unimplemented_item).c_str();
    }
    
    const char * NotImplemented::what() {
        return message;
    }


    const unsigned int decimal_conversion_base = 1000000000;
    const unsigned int conversion_quotient = 0x4;
    const unsigned int conversion_remainder = 0x1194d800; 
    BigInt::BigInt () {
        unsigned int magnitude = 0;
        magnitude_pointer = &magnitude;
        magnitude_length = 1;
        sign = false;
    }

    BigInt::BigInt (unsigned int magnitude[], unsigned short length, bool sgn) {
        magnitude_pointer = new unsigned int [length];
        for (int i = 0; i < length; i++) {
            magnitude_pointer[i] = magnitude[i];
        }
        magnitude_length = length;
        sign = sgn;
    };

    string BigInt::as_decimal_string() {
        string result = "";
        unsigned short last_unprocessed_place = magnitude_length - 1;
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
        } else if (sign) {
            result = "-" + result;
        }
        return result;
    }

    bool BigInt::equals (const BigInt& other) {
        if (sign != other.sign) {
            return false;
        }

        if (magnitude_length != other.magnitude_length) {
            return false;
        }
        for (int i = 0; i < magnitude_length; i++) {
            if (*(magnitude_pointer + i) != *(other.magnitude_pointer + i)) {
                return false;
            }
        }
        return true;
    }


        // public BigInteger add(BigInteger val) {
        //     if (val.signum == 0)
        //         return this;
        //     if (signum == 0)
        //         return val;
        //     if (val.signum == signum)
        //         return new BigInteger(add(mag, val.mag), signum);

        //     int cmp = compareMagnitude(val);
        //     if (cmp == 0)
        //         return ZERO;
        //     int[] resultMag = (cmp > 0 ? subtract(mag, val.mag)
        //                         : subtract(val.mag, mag));
        //     resultMag = trustedStripLeadingZeroInts(resultMag);

        //     return new BigInteger(resultMag, cmp == signum ? 1 : -1);
        // }
    BigInt BigInt::operator+ (const BigInt& other) {
        if (sign != other.sign) {
            return (*this) - BigInt(other.magnitude_pointer, other.magnitude_length, sign);
        }

        unsigned int overflow = 0, next_value = 0;
        unsigned short result_length = 0;

        unsigned int result_magnitude[max(magnitude_length, other.magnitude_length) + 1];

        while (result_length < magnitude_length || result_length < other.magnitude_length || overflow > 0) {
            unsigned long current_sum = current_sum = overflow;
            if (result_length < magnitude_length) {
                current_sum += *(magnitude_pointer + result_length);
            }
            if (result_length < other.magnitude_length) {
                current_sum += *(other.magnitude_pointer + result_length);
            }
            overflow = current_sum / 0x100000000;
            next_value = current_sum % 0x100000000;
            result_magnitude[result_length] = next_value;
            result_length++;
        }
        if (overflow > 0) {
            result_magnitude[result_length] = overflow;
            result_length++;
        }
        return BigInt(result_magnitude, result_length, sign);
    }

    BigInt BigInt::operator- (const BigInt& other) {
        if (sign != other.sign) {
            return *this + BigInt(other.magnitude_pointer, other.magnitude_length, sign);
        }
        bool this_has_larger_magnitude;
        if (magnitude_length == other.magnitude_length) {
            unsigned short comparison_index = magnitude_length - 1;
            while (*(magnitude_pointer + comparison_index) == *(other.magnitude_pointer + comparison_index))
            {
                if (comparison_index == 0) {
                    // The values are equal so their sum is zero
                    return BigInt();
                }
                comparison_index--;
            }

            
            this_has_larger_magnitude = *(magnitude_pointer + comparison_index) > *(other.magnitude_pointer + comparison_index);
        } else {
            this_has_larger_magnitude = magnitude_length > other.magnitude_length;
        }
        bool result_sign;
        if (this_has_larger_magnitude) {
            result_sign = sign;
        } else {
            result_sign = !sign;
        }
        unsigned short cursor = 0;
        unsigned short result_length = 0;
        unsigned int result_magnitude[magnitude_length];
        unsigned int current_larger, current_smaller;
        bool overflow = false, current_overflow;
        BigInt larger, smaller;
        if (this_has_larger_magnitude) {
            larger = *this;
            smaller = other;
        } else {
            larger = other;
            smaller = *this;
        }
        /*
            Example: 49 - 107 if place values are all mod 10
            larger = 107
            smaller = 49
            result_magnitude = [0, 0, 0]

            iter 1
            cursor = 0
            result_length = 0
            overflow = false

            current_larger = 7
            current_smaller = 9
            current_overflow = true
            result_magnitude = [8, 0, 0]

            iter 2
            cursor = 1
            result_length = 1
            overflow = true

            current_overflow = true
            current_larger = 9
            current_smaller = 4
            result_magnitude = [8, 5, 0]

            iter 3
            cursor = 2
            result_length = 2
            overflow = true

            current_larger = 0
            current_smaller = 0
            current_overflow = false
            result_magnitude = [8, 5, 0]

            while loop terminates

            result_magnitude = [8, 5, 0]
            result_length = 2

            return BigInt([8, 5, 0], 2, true)
        */
        while (cursor < magnitude_length || cursor < other.magnitude_length || overflow)
        {
            // Initialize loop iteration values

            // Larger will always have a value at this index
            current_larger = *(larger.magnitude_pointer + cursor);
            current_overflow = false;
            if (overflow) {
                if (current_larger == 0) {
                    current_overflow = true;
                }
                current_larger--;
            }
            if (cursor < smaller.magnitude_length) {
                current_smaller = *(smaller.magnitude_pointer + cursor);
            } else {
                current_smaller = 0;
            }

            // Do iteration work
            if (current_larger >= current_smaller) {
                result_magnitude[cursor] = current_larger - current_smaller;
            } else {
                // modular arithmetic FTW!
                result_magnitude[cursor] = current_larger - current_smaller;
                current_overflow = true;
            }
            if (result_magnitude[cursor] != 0) {
                result_length = cursor + 1;
            }

            // Prepare for next iteration
            overflow = current_overflow;
            cursor++;
        }
        
        return BigInt(result_magnitude, result_length, result_sign);
    }
}
