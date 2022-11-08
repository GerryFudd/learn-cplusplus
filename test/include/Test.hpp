#ifndef TEST_TYPE
#define TEST_TYPE

#include <exception_utils/enriched_exception.hpp>
#include <array_utils/stringish.hpp>

using namespace std;

namespace dexenjaeger {
    namespace test {
        class AssertionFailure: public exception_utils::enriched_exception {
        public:
            AssertionFailure(const char *);
            AssertionFailure(string);
        };

        class Test {
            const char * name;
            void (*exec)();
            void null_exec();
        public:
            Test();
            Test(const char *, void (*exec)());
            bool run(unsigned short,ostream&,ostream&);
        };
    }
}
#endif
