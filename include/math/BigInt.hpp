#ifndef BIGINT_DEF
#define BIGINT_DEF
#include <string>

using namespace std;

namespace dexenjaeger {
    namespace math {
        class BigInt {
            static const unsigned short KARATSUBA_THRESHOLD;
            // static const unsigned short KARATSUBA_SQUARE_THRESHOLD;
            static const unsigned short TOOM_COOK_THRESHOLD;
            // static const unsigned short TOOM_COOK_SQUARE_THRESHOLD;
            // Pointer to unsigned int so that magnitude can be variable size
            unsigned int * magnitude_pointer;
            unsigned short magnitude_length;
            // True indicates that the underlying int is negative
            bool sign;
            BigInt do_add(unsigned int *, unsigned short);
            BigInt do_sub(unsigned int *, unsigned short);
            static BigInt sub_from_larger(unsigned int *, unsigned short, unsigned int *, unsigned short, bool);
            BigInt mult (const BigInt&, bool);
            static BigInt multiply_by_long(unsigned int *, unsigned short, unsigned long, bool);
            static BigInt multiply_to_len(unsigned int *, unsigned short, unsigned int *, unsigned short, bool);
            static BigInt get_lower(const BigInt&, unsigned short);
            static BigInt get_upper(const BigInt&, unsigned short);
            BigInt shift(int);
            BigInt multiply_karatsuba(const BigInt&);
        public:
            BigInt();
            ~BigInt();
            BigInt (unsigned int [], unsigned short, bool);
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

        class NotImplemented: public exception {
            const char * message;
        public:
            NotImplemented(string);
            virtual const char * what();
        };
    }
}
#endif