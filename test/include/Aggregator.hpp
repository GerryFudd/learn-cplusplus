#ifndef AGGREGATOR_TYPE
#define AGGREGATOR_TYPE

#include <Test.hpp>

namespace test {
    class Aggregator {
        static Aggregator* singleton;
        unsigned short count;
        Test * tests;
        static Aggregator * get();
        void add_(Test);
    public:
        Aggregator();
        ~Aggregator();
        static void add(Test);
        static int run_all();
    };
}
#endif
