#include <array_utils/stringish.hpp>

using namespace std;

namespace dexenjaeger {
    namespace array_utils {
        mem_text::mem_text(stringstream& strstr): mem_text::mem_text(strstr.str()) {}
        mem_text::mem_text(string str) {
            length = str.length()+1;

            text = new char[length];
            for (int i = 0; i < length - 1; i++) {
                text[i] = str[i];
            }
            text[length - 1] = 0;
        }
    }
}
