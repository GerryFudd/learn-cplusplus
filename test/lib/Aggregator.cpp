#include <Aggregator.hpp>
#include <sstream>

using namespace std;

namespace dexenjaeger {
    namespace test {
        Aggregator * Aggregator::singleton = new Aggregator();
        Aggregator::Aggregator() {
            tests = new array_utils::appendable<Test>;
        }
        Aggregator::~Aggregator() {
            delete tests;
        }

        void Aggregator::add(Test test) {
            (*(*Aggregator::singleton).tests).append(test);
        }

        int Aggregator::run_all() {
            array_utils::appendable<Test> the = *(*Aggregator::singleton).tests;
            cout << "Running " << the.length() << " tests." << endl;
            int result = 0;
            string failures;
            stringstream info_buffer, failure_buffer;
            for (unsigned short i = 0; i < the.length(); i++) {
                if (the.get(i).run(i + 1, info_buffer, failure_buffer)) {
                    result++;
                    failures.append("\n")
                        .append(info_buffer.str())
                        .append(failure_buffer.str());
                }
                cout << info_buffer.str();
                info_buffer.str(string());
                failure_buffer.str(string());
            }
            cout << "Executed all tests with " << result << " failures" << endl;
            if (result > 0) {
                cerr << failures << endl;
            }
            return result;
        }
    }
}
