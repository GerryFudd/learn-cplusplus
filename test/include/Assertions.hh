#ifndef ASSERTIONS_LIB
#define ASSERTIONS_LIB
#include <Test.hpp>

namespace dexenjaeger {
    namespace test {
        template <class commonType>
        void assert_equal(const commonType& a, const commonType& b)  {
            if (a != b) {
                stringstream capture_message;
                try {
                    capture_message << a << " should equal " << b;
                } catch(...) {
                    throw AssertionFailure(array_utils::mem_text("Values are not equal. Exception encountered while streaming values to this error message. Consider implementing friend ostream& operator<<(ostream&,const T&)"));
                }

                throw AssertionFailure(array_utils::mem_text(capture_message));
            }
        }
    }
}
#endif
