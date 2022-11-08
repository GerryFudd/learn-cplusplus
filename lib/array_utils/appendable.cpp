#include <array_utils/appendable.inl>


namespace dexenjaeger {
    namespace array_utils {
        NullArrayException::NullArrayException(const char * message): exception_utils::enriched_exception(message) {}
        NullArrayException::NullArrayException(string message): exception_utils::enriched_exception(message) {}
    }
}
