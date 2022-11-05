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

    BigInt BigInt::mult(const BigInt& other, bool is_recursion) {
        if (
            (magnitude_length == 1 && *magnitude_pointer == 0) 
            || (other.magnitude_length == 1 && *other.magnitude_pointer == 0)
        ) {
            return BigInt();
        }
        if (magnitude_length == 1 && *magnitude_pointer == 1) {
            return BigInt(other.magnitude_pointer, other.magnitude_length, sign != other.sign);
        }
        if (other.magnitude_length == 1 && *other.magnitude_pointer == 1) {
            return BigInt(magnitude_pointer, magnitude_length, sign != other.sign);
        }
        unsigned short result_length = magnitude_length + other.magnitude_length + 2;
        unsigned int result_magnitude[result_length];
        for (int i = 0; i < result_length; i++) {
            result_magnitude[i] = 0;
        }

        unsigned short t_cursor = 0, o_cursor, cursor_next;
        unsigned long current_val;
        while (t_cursor < magnitude_length) {
            o_cursor = 0;
            while (o_cursor < other.magnitude_length) {
                cursor_next = 0;
                current_val = ((unsigned long)*(magnitude_pointer + t_cursor))
                    * ((unsigned long)*(other.magnitude_pointer + o_cursor))
                    + ((unsigned long) result_magnitude[t_cursor + o_cursor]);

                result_magnitude[t_cursor + o_cursor] = (unsigned int) current_val;
                current_val = current_val >> 32;
                while (current_val != 0) {
                    cursor_next++;
                    current_val += result_magnitude[t_cursor + o_cursor + cursor_next];
                    result_magnitude[t_cursor + o_cursor + cursor_next] = (unsigned int) current_val;
                    current_val = current_val >> 32;
                }
                o_cursor++;
            }
            t_cursor++;
        }
        while (result_magnitude[result_length - 1] == 0)
        {
            if (result_length == 0) {
                return BigInt();
            }
            result_length--;
        }
        return BigInt(result_magnitude, result_length, sign != other.sign);
    }

// /**
//  * Returns a BigInteger whose value is {@code (this * val)}.  If
//  * the invocation is recursive certain overflow checks are skipped.
//  *
//  * @param  val value to be multiplied by this BigInteger.
//  * @param  isRecursion whether this is a recursive invocation
//  * @return {@code this * val}
//  */
// private BigInteger multiply(BigInteger val, boolean isRecursion) {
//     if (val.signum == 0 || signum == 0)
//         return ZERO;

//     int xlen = mag.length;

//     if (val == this && xlen > MULTIPLY_SQUARE_THRESHOLD) {
//         return square();
//     }

//     int ylen = val.mag.length;

//     if ((xlen < KARATSUBA_THRESHOLD) || (ylen < KARATSUBA_THRESHOLD)) {
//         int resultSign = signum == val.signum ? 1 : -1;
//         if (val.mag.length == 1) {
//             return multiplyByInt(mag,val.mag[0], resultSign);
//         }
//         if (mag.length == 1) {
//             return multiplyByInt(val.mag,mag[0], resultSign);
//         }
//         int[] result = multiplyToLen(mag, xlen,
//                                         val.mag, ylen, null);
//         result = trustedStripLeadingZeroInts(result);
//         return new BigInteger(result, resultSign);
//     } else {
//         if ((xlen < TOOM_COOK_THRESHOLD) && (ylen < TOOM_COOK_THRESHOLD)) {
//             return multiplyKaratsuba(this, val);
//         } else {
//             //
//             // In "Hacker's Delight" section 2-13, p.33, it is explained
//             // that if x and y are unsigned 32-bit quantities and m and n
//             // are their respective numbers of leading zeros within 32 bits,
//             // then the number of leading zeros within their product as a
//             // 64-bit unsigned quantity is either m + n or m + n + 1. If
//             // their product is not to overflow, it cannot exceed 32 bits,
//             // and so the number of leading zeros of the product within 64
//             // bits must be at least 32, i.e., the leftmost set bit is at
//             // zero-relative position 31 or less.
//             //
//             // From the above there are three cases:
//             //
//             //     m + n    leftmost set bit    condition
//             //     -----    ----------------    ---------
//             //     >= 32    x <= 64 - 32 = 32   no overflow
//             //     == 31    x >= 64 - 32 = 32   possible overflow
//             //     <= 30    x >= 64 - 31 = 33   definite overflow
//             //
//             // The "possible overflow" condition cannot be detected by
//             // examning data lengths alone and requires further calculation.
//             //
//             // By analogy, if 'this' and 'val' have m and n as their
//             // respective numbers of leading zeros within 32*MAX_MAG_LENGTH
//             // bits, then:
//             //
//             //     m + n >= 32*MAX_MAG_LENGTH        no overflow
//             //     m + n == 32*MAX_MAG_LENGTH - 1    possible overflow
//             //     m + n <= 32*MAX_MAG_LENGTH - 2    definite overflow
//             //
//             // Note however that if the number of ints in the result
//             // were to be MAX_MAG_LENGTH and mag[0] < 0, then there would
//             // be overflow. As a result the leftmost bit (of mag[0]) cannot
//             // be used and the constraints must be adjusted by one bit to:
//             //
//             //     m + n >  32*MAX_MAG_LENGTH        no overflow
//             //     m + n == 32*MAX_MAG_LENGTH        possible overflow
//             //     m + n <  32*MAX_MAG_LENGTH        definite overflow
//             //
//             // The foregoing leading zero-based discussion is for clarity
//             // only. The actual calculations use the estimated bit length
//             // of the product as this is more natural to the internal
//             // array representation of the magnitude which has no leading
//             // zero elements.
//             //
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
//         }
//     }
// }
    
    BigInt BigInt::operator* (const BigInt& other) {
        return mult(other, false);
    }
}
