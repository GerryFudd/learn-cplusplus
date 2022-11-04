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

    BigInt::BigInt (unsigned int magnitude[], unsigned int length, bool sgn) {
        magnitude_pointer = new unsigned int [length];
        for (int i = 0; i < length; i++) {
            magnitude_pointer[i] = magnitude[i];
        }
        magnitude_length = length;
        sign = sgn;
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
            throw new NotImplemented("opposite signs.");
        }

        unsigned long current_sum;
        unsigned int overflow = 0, next_value = 0;
        unsigned short result_length = max(magnitude_length, other.magnitude_length);
        unsigned int result_magnitude[result_length];
        for (int i = 0; i < max(magnitude_length, other.magnitude_length); i++) {
            current_sum = overflow;
            if (i < magnitude_length) {
                current_sum += *(magnitude_pointer + i);
            }
            if (i < other.magnitude_length) {
                current_sum += *(other.magnitude_pointer + i);
            }
            overflow = current_sum / 0x100000000;
            next_value = current_sum % 0x100000000;
            result_magnitude[i] = next_value;
        }
        return BigInt(result_magnitude, result_length, sign);
    }
}
