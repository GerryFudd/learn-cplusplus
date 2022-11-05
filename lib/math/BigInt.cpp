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
    const unsigned long LONG_MASK = 0x00000000ffffffff;

    BigInt::BigInt () {
        magnitude_pointer = new unsigned int [1];
        magnitude_pointer[0] = 0;
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

    BigInt::~BigInt () {
        delete []magnitude_pointer;
    }

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
    
    BigInt BigInt::do_add(unsigned int * other_magnitude, unsigned short other_length) {
        unsigned int * larger_mag;
        unsigned short larger_length, smaller_length;
        if (magnitude_length > other_length) {
            larger_mag = magnitude_pointer;
            larger_length = magnitude_length;

            smaller_length = other_length;
        } else {
            larger_mag = other_magnitude;
            larger_length = other_length;

            smaller_length = magnitude_length;
        }
        unsigned int result_magnitude[larger_length + 1];
        unsigned long current_sum = 0;
        unsigned short cursor = 0;


        while (cursor < smaller_length) {
            current_sum = (*(magnitude_pointer + cursor) & LONG_MASK) + (*(other_magnitude + cursor) & LONG_MASK) + (current_sum >> 32);
            result_magnitude[cursor] = (unsigned int) current_sum;
            cursor++;
        }
        while (cursor < larger_length) {
            current_sum = (*(larger_mag + cursor) & LONG_MASK) + (current_sum >> 32);
            cursor++;
        }

        if ((current_sum >> 32) > 0) {
            result_magnitude[cursor] = 1;
            cursor++;
        }
        return BigInt(result_magnitude, cursor, sign);
    }

    BigInt BigInt::sub_from_larger(unsigned int * larger_mag, unsigned short larger_len, unsigned int * smaller_mag, unsigned short smaller_len, bool sign) {
        unsigned short cursor = 0;
        unsigned short result_length = 0;
        unsigned int result_magnitude[larger_len];
        unsigned short overflow = 0;

        while (cursor < smaller_len)
        {
            result_magnitude[cursor] = *(larger_mag + cursor) - *(smaller_mag + cursor) - overflow;
            if (*(larger_mag + cursor) < *(smaller_mag + cursor)) {
                overflow = 1;
            } else {
                overflow = 0;
            }
            cursor++;
        }
        while (overflow == 1) {
            if (*(larger_mag + cursor) != 0) {
                overflow = 0;
            }
            result_magnitude[cursor] = *(larger_mag + cursor) - 1;
            cursor ++;
        }
        while (cursor < larger_len) {
            result_magnitude[cursor] = *(larger_mag + cursor);
            cursor++;
        }
        while (result_magnitude[cursor - 1] == 0) {
            cursor--;
        }
        return BigInt(result_magnitude, cursor, sign);
    }

    BigInt BigInt::do_sub(unsigned int * other_magnitude, unsigned short other_length) {
        bool this_has_larger_magnitude;
        if (magnitude_length == other_length) {
            unsigned short comparison_index = magnitude_length - 1;
            while (*(magnitude_pointer + comparison_index) == *(other_magnitude + comparison_index))
            {
                if (comparison_index == 0) {
                    // The values are equal so their sum is zero
                    return BigInt();
                }
                comparison_index--;
            }

            
            this_has_larger_magnitude = *(magnitude_pointer + comparison_index) > *(other_magnitude + comparison_index);
        } else {
            this_has_larger_magnitude = magnitude_length > other_length;
        }
        if (this_has_larger_magnitude) {
            return BigInt::sub_from_larger(magnitude_pointer, magnitude_length, other_magnitude, other_length, sign);
        }
        return BigInt::sub_from_larger(other_magnitude, other_length, magnitude_pointer, magnitude_length, !sign);
    }

    BigInt BigInt::operator+ (const BigInt& other) {
        if (sign != other.sign) {
            return do_sub(other.magnitude_pointer, other.magnitude_length);
        }
        return do_add(other.magnitude_pointer, other.magnitude_length);
    }

    BigInt BigInt::operator- (const BigInt& other) {
        if (sign != other.sign) {
            return do_add(other.magnitude_pointer, other.magnitude_length);
        }
        return do_sub(other.magnitude_pointer, other.magnitude_length);
    }
}
