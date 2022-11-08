#ifndef FRAMEWORK_DEFS
#define FRAMEWORK_DEFS
#include <Aggregator.hpp>
#include <Test.hpp>

namespace test {
    struct cheater_registrar {
        cheater_registrar(Test t) {
            Aggregator::add(t);
        };
    };
}

#define TEST(name) \
void name(); \
test::Test name ## _test(#name, &name); \
test::cheater_registrar name ## _registered (name ## _test); \
void name()

#endif