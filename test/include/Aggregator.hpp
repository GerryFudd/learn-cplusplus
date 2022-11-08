#ifndef AGGREGATOR_TYPE
#define AGGREGATOR_TYPE

#include <Test.hpp>
#include <array_utils/appendable.inl>

namespace dexenjaeger {
    namespace test {
        class Aggregator {
            static Aggregator* singleton;
            array_utils::appendable<Test> * tests;
        public:
            Aggregator();
            ~Aggregator();
            static void add(Test);
            static int run_all();
        };
    }
}
#endif
