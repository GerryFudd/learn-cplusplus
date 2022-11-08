#include <sstream>
#include <execinfo.h>
#include <libunwind.h>
#include <cxxabi.h>

#include <exception_utils/enriched_exception.hpp>
#include <array_utils/stringish.hpp>

using namespace std;

namespace dexenjaeger {
    namespace exception_utils {
        enriched_exception::enriched_exception(const char * message): message{message} {
            stringstream out;
            unw_cursor_t cursor;
            unw_context_t context;
            unw_proc_info_t process;

            // Initialize cursor to current frame for local unwinding.
            unw_getcontext(&context);
            unw_init_local(&cursor, &context);

            // Unwind frames one by one, going up the frame stack.
            while (unw_step(&cursor) > 0) {
                unw_word_t offset, pc;
                unw_get_reg(&cursor, UNW_REG_IP, &pc);
                if (pc == 0) {
                    break;
                }
                out << "\n    " << hex << pc << " ";

                char sym[256];
                if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0) {
                    char* print_me = sym;
                    int status;
                    char* demangled = abi::__cxa_demangle(sym, nullptr, nullptr, &status);
                    if (status == 0) {
                        print_me = demangled;
                    }
                    out << print_me;
                } else {
                    out << " -- error: unable to obtain symbol name for this frame\n\t";
                }
                stack_trace = array_utils::mem_text(out).text;
            }
        }
        enriched_exception::enriched_exception(string message): enriched_exception::enriched_exception(array_utils::mem_text(message).text) {}

        enriched_exception::~enriched_exception() {
            delete[] message;
            delete[] stack_trace;
        }
        const char * enriched_exception::what() {
            return message;
        }

        ostream& operator<<(ostream& out, const enriched_exception& ex) {
            out << "\n  " << ex.message << ex.stack_trace;
            return out;
        }
    }
}
