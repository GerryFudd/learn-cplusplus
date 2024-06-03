#ifndef BIGINT_DEF
#define BIGINT_DEF
#include <string>
#include <vector>

using namespace std;

namespace gerryfudd::math {
    class BigInt {
        static const unsigned short KARATSUBA_THRESHOLD;
        // static const unsigned short KARATSUBA_SQUARE_THRESHOLD;
        static const unsigned short TOOM_COOK_THRESHOLD;
        // static const unsigned short TOOM_COOK_SQUARE_THRESHOLD;
        // Pointer to unsigned int so that magnitude can be variable size
        vector<unsigned int> magnitude;
        // True indicates that the underlying int is negative
        bool sign;
        BigInt do_add(vector<unsigned int>);
        BigInt do_sub(vector<unsigned int>);
        static BigInt sub_from_larger(vector<unsigned int>, vector<unsigned int>, bool);
        BigInt mult (const BigInt&, bool);
        static BigInt multiply_by_long(vector<unsigned int>, unsigned long, bool);
        static BigInt multiply_to_len(vector<unsigned int>, vector<unsigned int>, bool);
        static BigInt get_lower(const BigInt&, unsigned short);
        static BigInt get_upper(const BigInt&, unsigned short);
        BigInt shift(int);
        BigInt multiply_karatsuba(const BigInt&);
    public:
        BigInt();
        BigInt (vector<unsigned int>, bool);
        BigInt (unsigned int [], unsigned short, bool);
        BigInt (unsigned int, bool);
        BigInt (unsigned int);
        string as_decimal_string() const;
        string as_hex_string () const;
        BigInt operator + (const BigInt&);
        BigInt operator - (const BigInt&);
        BigInt operator - ();
        BigInt abs();

        BigInt operator * (const BigInt&);

        friend bool operator== (const BigInt&, const BigInt&);
        friend ostream& operator<<(ostream&, const BigInt&);
    };
}
#endif