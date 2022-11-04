#define BOOST_TEST_MODULE BigInt test
#include <boost/test/included/unit_test.hpp>
#include "math/BigInt.hpp"

using namespace std;
using namespace math;

BOOST_AUTO_TEST_CASE(to_string_test)
{
  BigInt test_int((unsigned int []) { 4789 }, 1, false);
  BOOST_TEST(test_int.as_decimal_string() == "4789");
}

BOOST_AUTO_TEST_CASE(to_string_test_negative)
{
  BigInt test_int((unsigned int []) { 4789 }, 1, true);
  BOOST_TEST(test_int.as_decimal_string() == "-4789");
}

BOOST_AUTO_TEST_CASE(to_string_test_zero)
{
  BigInt test_int;
  BOOST_TEST(test_int.as_decimal_string() == "0");
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

BOOST_AUTO_TEST_CASE(to_string_test_large_with_intermediate_zeros)
{
  BigInt test_int((unsigned int []){1410065409, 2}, 2, false);
  string result = test_int.as_decimal_string();
  BOOST_TEST(result == "10000000001", result + " should equal 10000000001\n");
}

BOOST_AUTO_TEST_CASE(simple_sum)
{
  BigInt a((unsigned int []){56}, 1, false), b((unsigned int []){109}, 1, false);
  BigInt result = a + b;
  BOOST_TEST(result.equals(BigInt((unsigned int []){165}, 1, false)), result.as_decimal_string() + " should equal 165\n");
}

BOOST_AUTO_TEST_CASE(negative_number_sum)
{
  BigInt a((unsigned int []){56}, 1, true), b((unsigned int []){109}, 1, true);
  BigInt result = a + b;
  BOOST_TEST(result.equals(BigInt((unsigned int []){165}, 1, true)), result.as_decimal_string() + " should equal -165\n");
}

BOOST_AUTO_TEST_CASE(sum_with_overflow)
{
  BigInt a((unsigned int []){0x80000000}, 1, false), b((unsigned int []){0x80000000}, 1, false);
  BigInt result = a + b;
  BOOST_TEST(result.equals(BigInt((unsigned int []){0, 1}, 2, false)), result.as_decimal_string() + " should equal 4294967296\n");
}
