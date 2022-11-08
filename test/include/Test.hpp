#ifndef TEST_TYPE
#define TEST_TYPE

#include <iostream>
#include <exception>
#include <array_utils/stringish.hpp>

using namespace std;

namespace dexenjaeger {
    namespace test {
        class AssertionFailure: public exception {
            const char * message;
        public:
            AssertionFailure(array_utils::mem_text);
            virtual const char * what();
            friend ostream& operator<<(ostream&,const AssertionFailure&);
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
