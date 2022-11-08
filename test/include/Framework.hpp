#ifndef FRAMEWORK_DEFS
#define FRAMEWORK_DEFS
#include <Aggregator.hpp>
#include <Assertions.hh>

namespace dexenjaeger {
    namespace test {
        struct cheater_registrar {
            cheater_registrar(Test t) {
                Aggregator::add(t);
            };
        };
    }
}

#define TEST(name) \
void name(); \
Test name ## _test(#name, &name); \
cheater_registrar name ## _registered (name ## _test); \
void name()

#endif