#ifndef BIGINT_DEF
#define BIGINT_DEF
#include <string>

using namespace std;

namespace math {
    class BigInt {
        static const unsigned short KARATSUBA_THRESHOLD;
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
    public:
        BigInt();
        ~BigInt();
        BigInt (unsigned int [], unsigned short, bool);
        BigInt (unsigned int);
        string as_decimal_string();
        BigInt operator + (const BigInt&);
        BigInt operator - (const BigInt&);
        BigInt operator - ();
        BigInt abs();
        bool equals (const BigInt&);

        BigInt operator * (const BigInt&);
    };



    class NotImplemented: public exception {
        const char * message;
    public:
        NotImplemented(string);
        virtual const char * what();
    };
}
#endif