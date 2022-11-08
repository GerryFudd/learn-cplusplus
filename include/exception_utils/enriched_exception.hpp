#ifndef ENRICHED_EXCEPTION_TYPE
#define ENRICHED_EXCEPTION_TYPE
#include <exception>
#include <iostream>
#include <string>

using namespace std;

namespace dexenjaeger {
    namespace exception_utils {
        class enriched_exception: public exception {
            const char * message;
            const char * stack_trace;
        public:
            enriched_exception(const char *);
            enriched_exception(string);
            ~enriched_exception();
            virtual const char * what();

            friend ostream& operator<<(ostream&,const enriched_exception&);
        };
    }
}

#endif