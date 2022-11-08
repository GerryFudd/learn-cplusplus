#include <Test.hpp>
#include <execinfo.h>
#include <libunwind.h>
#include <cxxabi.h>

using namespace std;

namespace dexenjaeger {
    namespace test {
        AssertionFailure::AssertionFailure(const char * message): exception_utils::enriched_exception(message) {}
        AssertionFailure::AssertionFailure(string message): exception_utils::enriched_exception(message) {}

        Test::Test(): name{""} {}
        Test::Test (const char * name, void (*exec)()): name{name}, exec{exec} {}

        bool Test::run(unsigned short ordinal, ostream& info, ostream& err) {
            try {
                (*exec)();
                info << ordinal << ". " << name << " SUCCESS" << endl;
                return false;
            } catch (const AssertionFailure& e) {
                info << ordinal << ". " << name << " FAILED" << endl;
                err << e << endl;

                return true;
            }
        }
    }
}
