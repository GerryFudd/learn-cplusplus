#include <stdexcept>

namespace ASSERT
{
    void assertTrue(bool tested) {
        if (!tested) {
            throw std::runtime_error("Value is false.");
        }
    }
}
