#include <Test.hpp>
#include <execinfo.h>
#include <libunwind.h>
#include <cxxabi.h>

using namespace std;

namespace dexenjaeger {
    namespace test {
        AssertionFailure::AssertionFailure(array_utils::mem_text mem_mes): message{mem_mes.text} {};

        const char * AssertionFailure::what() {
            return message;
        }

        ostream& operator<<(ostream& out, const AssertionFailure& failure) {
            out << "\n  " << failure.message;
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
            }
            return out;
        }

        Test::Test(): name{""} {}
        Test::Test (const char * name, void (*exec)()): name{name}, exec{exec} {}

        bool Test::run(unsigned short ordinal, ostream& info, ostream& err) {
            try {
                (*exec)();
                info << ordinal << ". " << name << " SUCCESS" << endl;
                return false;
            } catch (AssertionFailure e) {
                string as_string = string(name);
                info << ordinal << ". " << as_string << " FAILED" << endl;
                err << e << endl;

                return true;
            }
        }
    }
}
