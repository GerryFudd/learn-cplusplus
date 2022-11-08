#ifndef TEST_TYPE
#define TEST_TYPE

#include <iostream>
#include <exception>
#include <sstream>
#include <string>

using namespace std;

namespace test {
    struct mem_text {
        mem_text(stringstream&);
        mem_text(string);
        char * text;
        unsigned short length;
    };
    
    class AssertionFailure: public exception {
        const char * message;
    public:
        AssertionFailure(mem_text);
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
        bool run(unsigned short, ostream&,ostream&);
    };
}
#endif
