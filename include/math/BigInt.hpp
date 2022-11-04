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
    public:
        BigInt();
        BigInt (unsigned int [], unsigned short, bool);
        string as_decimal_string();
        BigInt operator + (const BigInt&);
        BigInt operator - (const BigInt&);
        bool equals (const BigInt&);
    };



    class NotImplemented: public exception {
        const char * message;
    public:
        NotImplemented(string);
        virtual const char * what();
    };
}
#endif