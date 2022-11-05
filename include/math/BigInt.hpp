#ifndef BIGINT_DEF
#define BIGINT_DEF
#include <string>

using namespace std;

namespace math {
    class BigInt {
        // Pointer to unsigned int so that magnitude can be variable size
        unsigned int * magnitude_pointer;
        unsigned short magnitude_length;
        // True indicates that the underlying int is negative
        bool sign;
        BigInt do_add(unsigned int *, unsigned short);
        BigInt do_sub(unsigned int *, unsigned short);
        static BigInt sub_from_larger(unsigned int *, unsigned short, unsigned int *, unsigned short, bool);
        BigInt mult (const BigInt&, bool);
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