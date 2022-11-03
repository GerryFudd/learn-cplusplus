#define BOOST_TEST_MODULE BigInt test
#include <boost/test/included/unit_test.hpp>
#include "math/BigInt.hpp"

using namespace std;

BOOST_AUTO_TEST_CASE(constructor_runs)
{
  BigInt test_int((unsigned int []) { 4789 }, 1, false);
}

BOOST_AUTO_TEST_CASE(to_string_test)
{
  BigInt string_int((unsigned int []) { 4789 }, 1, false);
  BOOST_ASSERT(string_int.as_string() == "4789");
}

BOOST_AUTO_TEST_CASE(to_string_test_negative)
{
  BigInt string_int((unsigned int []) { 4789 }, 1, true);
  BOOST_ASSERT(string_int.as_string() == "-4789");
}
