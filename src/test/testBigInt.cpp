#define BOOST_TEST_MODULE BigInt test
#include <boost/test/included/unit_test.hpp>
#include "math/BigInt.hpp"

using namespace std;

BOOST_AUTO_TEST_CASE(to_string_test)
{
  BigInt test_int((unsigned int []) { 4789 }, 1, false);
  BOOST_ASSERT(test_int.as_decimal_string() == "4789");
}

BOOST_AUTO_TEST_CASE(to_string_test_negative)
{
  BigInt test_int((unsigned int []) { 4789 }, 1, true);
  BOOST_ASSERT(test_int.as_decimal_string() == "-4789");
}

BOOST_AUTO_TEST_CASE(to_string_test_zero)
{
  BigInt test_int;
  BOOST_ASSERT(test_int.as_decimal_string() == "0");
}

BOOST_AUTO_TEST_CASE(to_string_test_large)
{
  BigInt test_int((unsigned int []){0, 1}, 2, false);
  string result = test_int.as_decimal_string();
  BOOST_TEST(result == "4294967296", result + " should equal 4294967296\n");
}

BOOST_AUTO_TEST_CASE(to_string_test_large_complex)
{
  BigInt test_int((unsigned int []){4294967295, 4294967295}, 2, true);
  string result = test_int.as_decimal_string();
  BOOST_TEST(result == "-18446744073709551615", result + " should equal -18446744073709551615\n");
}
