#ifndef BIGINT_DEF
#define BIGINT_DEF
#include <string>

namespace std {
    class BigInt {
        // Pointer to unsigned int so that magnitude can be variable size
        unsigned int * magnitude_pointer;
        unsigned int magnitude_length;
        // True indicates that the underlying int is negative
        bool discriminator;
    public:
        BigInt();
        BigInt (unsigned int [], unsigned int, bool);
        string as_string();
    };
}
#endif