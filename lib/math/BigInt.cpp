#include <algorithm>
#include <exception>
#include <iostream>
#include <sstream>
#include <math/BigInt.hpp>

using namespace std;

namespace gerryfudd::math {
    const unsigned int decimal_conversion_base = 1000000000;
    const unsigned int conversion_quotient = 0x4;
    const unsigned int conversion_remainder = 0x1194d800;

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

    // ********** BEGIN constructors & destructors **********
    BigInt::BigInt (): magnitude{}, sign{false} {}

    BigInt::BigInt (unsigned int v, bool sign): sign{sign} {
        magnitude.push_back(v);
    }

    BigInt::BigInt (unsigned int v): BigInt::BigInt(v, false) {}

    BigInt::BigInt (unsigned int magnitude_pointer[], unsigned short magnitude_length, bool sign): sign{sign}, magnitude{} {
        while (magnitude.size() < magnitude_length) {
            magnitude.push_back(magnitude_pointer[magnitude.size()]);
        }
    }

    BigInt::BigInt (vector<unsigned int> magnitude, bool sign):magnitude{magnitude}, sign{sign} {}
    // ********** END constructors & destructors **********

    // ********** BEGIN string **********
    string BigInt::as_decimal_string() const {
        if (magnitude.size() == 0) {
            return "0";
        }
        string result = "";
        unsigned short last_unprocessed_place = magnitude.size() - 1;
        unsigned int unprocessed_magnitude[magnitude.size()];
        for (int i = 0; i < magnitude.size(); i++) {
            unprocessed_magnitude[i] = magnitude[i];
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

    string BigInt::as_hex_string() const {
        // Collect result in a stream
        stringstream hex_stream;
        
        hex_stream << *this;

        return hex_stream.str();
    }
    
    ostream& operator<<(ostream& os, const BigInt& item) {
        if (item.magnitude.size() == 0) {
            os << "0x0";
            return os;
        }
        // If negative, start with that sign
        if (item.sign) {
            os << '-';
        }
        // Append hex symbol and first block
        os << "0x" << hex << item.magnitude.back();
        unsigned int block;
        unsigned short shift_by;
        // Start appending full blocks of eight characters at the second block.
        for (int i = item.magnitude.size() - 2; i >= 0; i--) {
            block = item.magnitude[i];
            // Shifting by 4 bits removes the smallest hex value from the string
            // Shift by 7, then 6, etc... and append a leading 0 until there's a non-zero hex value.
            shift_by = 28;
            while (shift_by != 0 && block >> shift_by == 0) {
                os << '0';
                shift_by -= 4;
            }
            os << hex << block;
        }

        return os;
    }
    // ********** END string **********

    // ********** BEGIN self **********
    BigInt BigInt::operator- () {
        return BigInt(magnitude, !sign);
    }

    BigInt BigInt::abs () {
        return BigInt(magnitude, false);
    }
    // ********** END self **********

    // ********** BEGIN bitwise-ish **********
    BigInt BigInt::shift(int distance) {
        if (magnitude.size() == 1 && magnitude[0] == 0) {
            return BigInt();
        }
        vector<unsigned int> result;
        if (distance <= 0) {
            if (magnitude.size() <= -distance) {
                return BigInt();
            }
            for (int i = -distance; i < magnitude.size(); i++) {
                result.push_back(magnitude[i]);
            }
        } else {
            result.resize(distance);
            for (int i = 0; i < magnitude.size(); i++) {
                result.push_back(magnitude[i]);
            }
        }
        return BigInt(result, sign);
    }
    // ********** END bitwise-ish **********

    // ********** BEGIN comparison **********
    bool operator== (const BigInt& first, const BigInt& second) {
        if (first.sign != second.sign) {
            return false;
        }

        if (first.magnitude.size() != second.magnitude.size()) {
            return false;
        }

        for (int i = 0; i < first.magnitude.size(); i++) {
            if (first.magnitude[i] != second.magnitude[i]) {
                return false;
            }
        }
        return true;
    }
    // ********** END comparison **********

    // ********** BEGIN sum **********
    BigInt BigInt::do_add(vector<unsigned int> other_magnitude) {
        vector<unsigned int> result_magnitude;

        unsigned long current_sum = 0;

        while (result_magnitude.size() < magnitude.size() || result_magnitude.size() < other_magnitude.size()) {
            current_sum = current_sum >> 32;
            if (result_magnitude.size() < magnitude.size()) {
                current_sum += (unsigned long) magnitude[result_magnitude.size()];
            }
            if (result_magnitude.size() < other_magnitude.size()) {
                current_sum += (unsigned long) other_magnitude[result_magnitude.size()];
            }
            result_magnitude.push_back((unsigned int) current_sum);
        }

        if ((current_sum >> 32) > 0) {
            result_magnitude.push_back(1);
        }
        return BigInt(result_magnitude, sign);
    }

    BigInt BigInt::operator+ (const BigInt& other) {
        if (sign != other.sign) {
            return do_sub(other.magnitude);
        }
        return do_add(other.magnitude);
    }
    // ********** END sum **********

    // ********** BEGIN difference **********
    BigInt BigInt::sub_from_larger(vector<unsigned int> larger_magnitude, vector<unsigned int> smaller_magnitude, bool sign) {
        vector<unsigned int> result_magnitude;
        unsigned int current_place_value;
        unsigned short overflow = 0;

        while (result_magnitude.size() < smaller_magnitude.size())
        {
            current_place_value = larger_magnitude[result_magnitude.size()] - smaller_magnitude[result_magnitude.size()] - overflow;
            if (
                (overflow == 1 && larger_magnitude[result_magnitude.size()] == smaller_magnitude[result_magnitude.size()])
                || larger_magnitude[result_magnitude.size()] < smaller_magnitude[result_magnitude.size()]
            ) {
                overflow = 1;
            } else {
                overflow = 0;
            }
            result_magnitude.push_back(current_place_value);
        }
        while (overflow == 1) {
            if (larger_magnitude[result_magnitude.size()] != 0) {
                overflow = 0;
            }
            result_magnitude.push_back(larger_magnitude[result_magnitude.size()] - 1);
        }
        while (result_magnitude.size() < larger_magnitude.size()) {
            result_magnitude.push_back(larger_magnitude[result_magnitude.size()]);
        }
        while (result_magnitude.back() == 0) {
            result_magnitude.pop_back();
        }
        return BigInt(result_magnitude, sign);
    }

    BigInt BigInt::do_sub(vector<unsigned int> other_magnitude) {
        bool this_has_larger_magnitude;
        if (magnitude.size() == other_magnitude.size()) {
            unsigned short comparison_index = magnitude.size() - 1;
            while (magnitude[comparison_index] == other_magnitude[comparison_index])
            {
                if (comparison_index == 0) {
                    // The values are equal so their sum is zero
                    return BigInt();
                }
                comparison_index--;
            }

            
            this_has_larger_magnitude = magnitude[comparison_index] > other_magnitude[comparison_index];
        } else {
            this_has_larger_magnitude = magnitude.size() > other_magnitude.size();
        }
        if (this_has_larger_magnitude) {
            return BigInt::sub_from_larger(magnitude, other_magnitude, sign);
        }
        return BigInt::sub_from_larger(other_magnitude, magnitude, !sign);
    }

    BigInt BigInt::operator- (const BigInt& other) {
        if (sign != other.sign) {
            return do_add(other.magnitude);
        }
        return do_sub(other.magnitude);
    }
    // ********** END difference **********

    // ********** BEGIN product **********
    BigInt BigInt::multiply_to_len(vector<unsigned int> mag_one, vector<unsigned int> mag_two, bool sign) {
        vector<unsigned int> result_magnitude;
        result_magnitude.resize(mag_one.size() + mag_two.size());

        unsigned long current_val, overflow = 0;
        for (int i = 0; i < mag_one.size(); i++) {
            current_val = ((unsigned long)mag_one[i])
                    * ((unsigned long)mag_two.front())
                    + overflow;
            result_magnitude[i] = (unsigned int) current_val;
            overflow = current_val >> 32;
        }
        result_magnitude[mag_one.size()] = (unsigned int) overflow;
        for (int j = 1; j < mag_two.size(); j++) {
            overflow = 0;
            for (int k = 0; k < mag_one.size(); k++) {
                current_val = ((unsigned long)mag_one[k])
                    * ((unsigned long)mag_two[j])
                    + ((unsigned long)result_magnitude[j + k])
                    + overflow;
                result_magnitude[j + k] = (unsigned int) current_val;
                overflow = current_val >> 32;
            }
            result_magnitude[mag_one.size() + j] = overflow;
        }
        while (result_magnitude.back() == 0)
        {
            result_magnitude.pop_back();
            if (result_magnitude.size() == 0) {
                return BigInt();
            }
        }
        return BigInt(result_magnitude, sign);
    }

    BigInt BigInt::multiply_by_long(vector<unsigned int> magnitude, unsigned long val, bool sign) {
        vector<unsigned int> result;
        unsigned long current, overflow = 0;

        for (int i = 0; i < magnitude.size(); i++) {
            current = ((unsigned long) magnitude[i]) * val + overflow;
            result.push_back((unsigned int) current);
            overflow = current >> 32;
        }
        result.push_back(overflow);
        while (result.back() == 0) {
            result.pop_back();
            if (result.size() == 0) {
                return BigInt();
            }
        }
        return BigInt(result, sign);
    }

    // ****** BEGIN Karitsuba ******
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

    BigInt BigInt::get_upper(const BigInt& other, unsigned short index) {
        if (other.magnitude.size() <= index + 1) {
            return BigInt();
        }

        vector<unsigned int> result;
        for (int i = index; i < other.magnitude.size(); i++) {
            result.push_back(other.magnitude[i]);
        }
        return BigInt(result, other.sign);
    }

    BigInt BigInt::get_lower(const BigInt& other, unsigned short index) {
        if (other.magnitude.size() <= index) {
            return BigInt(other.magnitude, other.sign);
        }

        vector<unsigned int> result = other.magnitude;
        result.resize(index);
        return BigInt(result, other.sign);
    }

    BigInt BigInt::multiply_karatsuba(const BigInt& other) {
        unsigned short half_len = ((unsigned int)max(magnitude.size(), other.magnitude.size()) + 1) / 2;
        // tl = this % 2^(32*half_len)
        BigInt tl = BigInt::get_lower(*this, half_len);
        // tu = this // 2^(32*half_len)
        BigInt tu = BigInt::get_upper(*this, half_len);
        // this = tu*2^(32*half_len) + tl

        // ol = other % 2^(32*half_len)
        BigInt ol = BigInt::get_lower(other, half_len);
        // ou = other // 2^(32*half_len)
        BigInt ou = BigInt::get_upper(other, half_len);
        // other = ou*2^(32*half_len) + ol

        BigInt uu = tu * ou;
        BigInt ll = tl * ol;

        //           mid = (tu + tl) * (ou + ol) - uu - ll
        //               = tu * ol + tl * ou
        BigInt mid = (tu + tl) * (ou + ol) - uu - ll;

        // this * other = (tu*2^(32*half_len) + tl)*(ou*2^(32*half_len) + ol)
        //              = uu*2^(32*half_len*2) + (tu*ol + tl*ou)*2^(32*half_len) + ll
        // return uu.shift(half_len * 2) + (mid - uu - ll).shift(half_len) + ll;
        return uu.shift(half_len << 1) + mid.shift(half_len) + ll;
    }
    // ****** END Karitsuba ******

    BigInt BigInt::mult(const BigInt& other, bool is_recursion) {
        if (magnitude.size() == 0 || other.magnitude.size() == 0) {
            return BigInt();
        }
        if (magnitude.size() < KARATSUBA_THRESHOLD || other.magnitude.size() < KARATSUBA_THRESHOLD) {
            if (other.magnitude.size() == 1) {
                return BigInt::multiply_by_long(magnitude, other.magnitude[0], sign != other.sign);
            }

            if (magnitude.size() == 1) {
                return BigInt::multiply_by_long(other.magnitude, magnitude.front(), sign != other.sign);
            }

            return BigInt::multiply_to_len(magnitude, other.magnitude, sign != other.sign);
        }
        if ((magnitude.size() < TOOM_COOK_THRESHOLD) && (other.magnitude.size() < TOOM_COOK_THRESHOLD)) {
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
    // ********** END product **********
}
