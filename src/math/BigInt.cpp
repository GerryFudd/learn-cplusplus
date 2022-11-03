#include <string>

namespace std {
    class BigInt {
        // Pointer to unsigned int so that magnitude can be variable size
        unsigned int * magnitude_pointer;
        unsigned int magnitude_length;
        // True indicates that the underlying int is negative
        bool discriminator;
    public:
        BigInt (unsigned int magnitude[], unsigned int length, bool disc) {
            magnitude_pointer = magnitude;
            magnitude_length = length;
            discriminator = disc;
        };
        string as_string() {
            string result = "";
            if (magnitude_length == 1) {
                result += to_string(*magnitude_pointer);
            }
            return result;
        }
    };
}
