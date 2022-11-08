#ifndef STRINGISH_LIB
#define STRINGISH_LIB
#include <sstream>
#include <string>

using namespace std;

namespace dexenjaeger {
    namespace array_utils {
        struct mem_text {
            mem_text(stringstream&);
            mem_text(string);
            char * text;
            unsigned short length;
        };
    }
}
#endif
