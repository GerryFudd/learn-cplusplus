
class BigInt {
        // Pointer to unsigned int so that magnitude can be variable size
        unsigned int * magnitude_pointer;
        unsigned int magnitude_length;
        // True indicates that the underlying int is negative
        bool discriminator;
    public:
        BigInt (unsigned int[], unsigned int, bool);
};

BigInt::BigInt (unsigned int magnitude[], unsigned int length, bool disc) {
    magnitude_pointer = magnitude;
    magnitude_length = length;
    discriminator = disc;
}
