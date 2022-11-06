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

    /**
     * The threshold value for using Karatsuba multiplication.  If the number
     * of ints in both mag arrays are greater than this number, then
     * Karatsuba multiplication will be used.   This value is found
     * experimentally to work well.
     */
    const unsigned short BigInt::KARATSUBA_THRESHOLD = 80;

    /**
     * The threshold value for using Karatsuba squaring.  If the number
     * of ints in the number are larger than this value,
     * Karatsuba squaring will be used.   This value is found
     * experimentally to work well.
     */
    // const unsigned short BigInt::KARATSUBA_SQUARE_THRESHOLD = 128;

    /**
     * The threshold value for using 3-way Toom-Cook multiplication.
     * If the number of ints in each mag array is greater than the
     * Karatsuba threshold, and the number of ints in at least one of
     * the mag arrays is greater than this threshold, then Toom-Cook
     * multiplication will be used.
     */
    const unsigned short BigInt::TOOM_COOK_THRESHOLD = 240;

    /**
     * The threshold value for using Toom-Cook squaring.  If the number
     * of ints in the number are larger than this value,
     * Toom-Cook squaring will be used.   This value is found
     * experimentally to work well.
     */
    // const unsigned short BigInt::TOOM_COOK_SQUARE_THRESHOLD = 216;

    BigInt::BigInt () {
        magnitude_pointer = new unsigned int [1];
        magnitude_pointer[0] = 0;
        magnitude_length = 1;
        sign = false;
    }

    BigInt::BigInt (unsigned int v) {
        magnitude_pointer = new unsigned int [1];
        magnitude_pointer[0] = v;
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

    string BigInt::as_hex_string() {
        string result = "0x";
        char hex_block[9];
        unsigned int block;
        for (int i = magnitude_length - 1; i >= 0; i--) {
        snprintf(hex_block, 9, "%x", *(magnitude_pointer + i));
        string current_block = string(hex_block);
        if (result.length() > 2) {
            result.append(string(8-current_block.length(), '0'));
        }
        result.append(current_block);
        }
        if (result == "0x") {
            return "0x0";
        }

        if (sign) {
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
            current_sum = ((unsigned long)*(magnitude_pointer + cursor)) + ((unsigned long)*(other_magnitude + cursor)) + (current_sum >> 32);
            result_magnitude[cursor] = (unsigned int) current_sum;
            cursor++;
        }
        while (cursor < larger_length) {
            current_sum = ((unsigned long)*(larger_mag + cursor)) + (current_sum >> 32);
            result_magnitude[cursor] = (unsigned int) current_sum;
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

    BigInt BigInt::operator- () {
        return BigInt(magnitude_pointer, magnitude_length, !sign);
    }

    BigInt BigInt::abs () {
        return BigInt(magnitude_pointer, magnitude_length, false);
    }

    BigInt BigInt::multiply_by_long(unsigned int * magnitude, unsigned short len, unsigned long val, bool sign) {
        unsigned int result[len + 1];
        unsigned long current, overflow = 0;

        for (int i = 0; i < len; i++) {
            current = ((unsigned long) *(magnitude + i)) * val + overflow;
            result[i] = (unsigned int) current;
            overflow = current >> 32;
        }
        result[len] = overflow;
        unsigned short result_len = len + 1;
        while (result[result_len - 1] == 0) {
            if (result_len == 1) {
                return BigInt();
            }
            result_len--;
        }
        return BigInt(result, result_len, sign);
    }

    BigInt BigInt::multiply_to_len(unsigned int * mag_one, unsigned short len_one, unsigned int * mag_two, unsigned short len_two, bool sign) {
        unsigned short result_length = len_one + len_two;
        unsigned int result_magnitude[result_length];
        for (int i = 0; i < result_length; i++) {
            result_magnitude[i] = 0;
        }

        unsigned long current_val, overflow = 0;
        for (int i = 0; i < len_one; i++) {
            current_val = ((unsigned long)*(mag_one + i))
                    * ((unsigned long)*mag_two)
                    + overflow;
            result_magnitude[i] = (unsigned int) current_val;
            overflow = current_val >> 32;
        }
        result_magnitude[len_one] = (unsigned int) overflow;
        for (int j = 1; j < len_two; j++) {
            overflow = 0;
            for (int k = 0; k < len_one; k++) {
                current_val = ((unsigned long)*(mag_one + k))
                    * ((unsigned long)*(mag_two + j))
                    + ((unsigned long)result_magnitude[j + k])
                    + overflow;
                result_magnitude[j + k] = (unsigned int) current_val;
                overflow = current_val >> 32;
            }
            result_magnitude[len_one + j] = overflow;
        }
        while (result_magnitude[result_length - 1] == 0)
        {
            if (result_length == 0) {
                return BigInt();
            }
            result_length--;
        }
        return BigInt(result_magnitude, result_length, sign);
    }

    BigInt BigInt::get_upper(const BigInt& other, unsigned short index) {
        if (other.magnitude_length <= index + 1) {
            return BigInt();
        }

        return BigInt(other.magnitude_pointer + index, other.magnitude_length - index, other.sign);
    }

    BigInt BigInt::get_lower(const BigInt& other, unsigned short index) {
        if (other.magnitude_length <= index) {
            return BigInt(other.magnitude_pointer, other.magnitude_length, other.sign);
        }

        return BigInt(other.magnitude_pointer, index, other.sign);
    }

    BigInt BigInt::shift(int distance) {
        if (magnitude_length == 1 && *magnitude_pointer == 0) {
            return BigInt();
        }
        if (distance <= 0) {
            if (magnitude_length <= -distance) {
                return BigInt();
            }
            return BigInt(magnitude_pointer - distance, magnitude_length + distance, sign);
        }
        unsigned int * result = new unsigned int[distance + magnitude_length];
        for (int i = 0; i < distance + magnitude_length; i++) {
            if (i < distance) {
                result[i] = 0;
            } else {
                result[i] = *(magnitude_pointer + i - distance);
            }
        }
        return BigInt(result, (unsigned short) distance + magnitude_length, sign);
    }

    BigInt BigInt::multiply_karatsuba(const BigInt& other) {
        unsigned short half_len = ((unsigned int)max(magnitude_length, other.magnitude_length) + 1) / 2;
        BigInt tl = BigInt::get_lower(*this, half_len);
        BigInt tu = BigInt::get_upper(*this, half_len);

        BigInt ol = other.get_lower(other, half_len);
        BigInt ou = other.get_upper(other, half_len);

        BigInt uu = tu * ou;
        BigInt ll = tl * ol;
        BigInt ul = tu * ol;
        BigInt lu = tl * ou;

        return uu.shift(half_len * 2) + ul.shift(half_len) + lu.shift(half_len) + ll;
    }

    BigInt BigInt::mult(const BigInt& other, bool is_recursion) {
        if (
            (magnitude_length == 1 && *magnitude_pointer == 0) 
            || (other.magnitude_length == 1 && *other.magnitude_pointer == 0)
        ) {
            return BigInt();
        }
        if (magnitude_length < KARATSUBA_THRESHOLD || other.magnitude_length < KARATSUBA_THRESHOLD) {
            if (other.magnitude_length == 1) {
                return BigInt::multiply_by_long(magnitude_pointer, magnitude_length, *other.magnitude_pointer, sign != other.sign);
            }

            if (magnitude_length == 1) {
                return BigInt::multiply_by_long(other.magnitude_pointer, other.magnitude_length, *magnitude_pointer, sign != other.sign);
            }

            return BigInt::multiply_to_len(magnitude_pointer, magnitude_length, other.magnitude_pointer, other.magnitude_length, sign != other.sign);
        }
        if ((magnitude_length < TOOM_COOK_THRESHOLD) && (other.magnitude_length < TOOM_COOK_THRESHOLD)) {
            return multiply_karatsuba(other);
        }
        return BigInt();
    }

    // private static void reportOverflow() {
    //     throw new ArithmeticException("BigInteger would overflow supported range");
    // }
//             if (!isRecursion) {
//                 // The bitLength() instance method is not used here as we
//                 // are only considering the magnitudes as non-negative. The
//                 // Toom-Cook multiplication algorithm determines the sign
//                 // at its end from the two signum values.
//                 if ((long)bitLength(mag, mag.length) +
//                     (long)bitLength(val.mag, val.mag.length) >
//                     32L*MAX_MAG_LENGTH) {
//                     reportOverflow();
//                 }
//             }

//             return multiplyToomCook3(this, val);
    
    BigInt BigInt::operator* (const BigInt& other) {
        return mult(other, false);
    }
}
