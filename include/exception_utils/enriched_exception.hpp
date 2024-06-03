#ifndef ENRICHED_EXCEPTION_TYPE
#define ENRICHED_EXCEPTION_TYPE
#include <exception>
#include <iostream>
#include <string>

using namespace std;

namespace gerryfudd::exception_utils {
    /*
        This exception accepts either a char pointer or a string in its constructor and automatically
        captures the current stack trace. Its void what() implementation returns the message and it
        streams the message and full stack trace when you use the << operator.
    */ 
    class enriched_exception: public exception {
        string message;
        string stack_trace;
    public:
        enriched_exception(string);
        virtual const char * what();

        friend ostream& operator<<(ostream&,const enriched_exception&);
    };
}

#endif