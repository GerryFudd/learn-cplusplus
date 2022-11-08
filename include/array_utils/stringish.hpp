#ifndef STRINGISH_LIB
#define STRINGISH_LIB
#include <sstream>
#include <string>

using namespace std;

namespace dexenjaeger {
    namespace array_utils {
        /*
            This struct takes either a stringstream or a string and converts it to a character pointer
            that points to a persistent block in memory. The length property holds the length of this 
            block in memory.
        */
        struct mem_text {
            mem_text(stringstream&);
            mem_text(string);
            char * text;
            unsigned short length;
        };
    }
}
#endif
