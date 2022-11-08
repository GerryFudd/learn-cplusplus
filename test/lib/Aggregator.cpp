#include <iostream>
#include <sstream>
#include <string>
#include <Aggregator.hpp>

using namespace std;

namespace test {
    Aggregator * Aggregator::singleton = nullptr;
    Aggregator::Aggregator(): count(0) {}
    Aggregator::~Aggregator() {
        if (count > 0) {
            delete[] tests;
        }
    }

    Aggregator * Aggregator::get() {
        if (Aggregator::singleton == nullptr) {
            Aggregator::singleton = new Aggregator();
        }
        return Aggregator::singleton;
    }

    void Aggregator::add_(Test test) {
        Test * updated_tests = new Test[count + 1];
        for (int i = 0; i < count; i++) {
            updated_tests[i] = tests[i];
        }
        updated_tests[count] = test;

        if (count > 0) {
            delete[] tests;
        }
        tests = updated_tests;
        count++;
    }

    void Aggregator::add(Test test) {
        (*Aggregator::get()).add_(test);
    }

    int Aggregator::run_all() {
        Aggregator the = *Aggregator::get();
        cout << "Running " << the.count << " tests." << endl;
        int result = 0;
        string failures;
        stringstream info_buffer, failure_buffer;
        for (unsigned short i = 0; i < the.count; i++) {
            if (the.tests[i].run(i + 1, info_buffer, failure_buffer)) {
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
