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

BOOST_AUTO_TEST_CASE(sum_with_multiple_blocks_and_overflow)
{
  BigInt a((unsigned int []){0x80700230, 0xa0475d1e}, 2, false), b((unsigned int []){0xc00e00f2, 0x5fffffff}, 2, false);
  BigInt result = a + b;
  BOOST_TEST(result.equals(BigInt((unsigned int []){0x407e0322, 0x475d1e, 0x1}, 3, false)), result.as_decimal_string() + " should equal 18466831181568410402\n");
}

BOOST_AUTO_TEST_CASE(sum_with_several_orders_of_magnitude)
{
  BigInt a((unsigned int []){0x80700230, 0xa0475d1e, 0x43875002, 0x213}, 4, false), b((unsigned int []){0xc00e00f2, 0x5fffffff}, 2, false);
  BigInt result = a + b;
  BOOST_TEST(result.equals(BigInt((unsigned int []){0x407e0322, 0x475d1e, 0x43875003, 0x213}, 4, false)), result.as_decimal_string() + " should equal 18466831181568410402\n");
}

BOOST_AUTO_TEST_CASE(sum_with_opposite_signs)
{
  BigInt a((unsigned int []){14}, 1, true), b((unsigned int []){11}, 1, false);
  BigInt result = a + b;
  BOOST_TEST(result.equals(BigInt((unsigned int []){3}, 1, true)), result.as_decimal_string() + " should equal -3\n");
}

BOOST_AUTO_TEST_CASE(subtract_with_opposite_signs)
{
  BigInt a((unsigned int []){38}, 1, true), b((unsigned int []){22}, 1, false);
  BigInt result = a - b;
  BOOST_TEST(result.equals(BigInt((unsigned int []){60}, 1, true)), result.as_decimal_string() + " should equal -60\n");
}

BOOST_AUTO_TEST_CASE(subtract_with_multiple_blocks)
{
  BigInt a((unsigned int []){0x2473, 0x7429, 0x0, 0x2}, 4, false), b((unsigned int []){0x824, 0xffffffff}, 2, false);
  BigInt result = a - b;
  BOOST_TEST(result.equals(BigInt((unsigned int []){0x1c4f, 0x742a, 0xffffffff, 0x1}, 4, false)), result.as_decimal_string() + " should equal 158456325010082058837115804751\n");
}

BOOST_AUTO_TEST_CASE(subtract_with_several_orders_of_magnitude_difference)
{
  BigInt a((unsigned int []){0x2473, 0x7429, 0x0, 0x2}, 4, false), b((unsigned int []){0x824, 0x3a10}, 2, false);
  BigInt result = a - b;
  BOOST_TEST(result.equals(BigInt((unsigned int []){0x1c4f, 0x3a19, 0x0, 0x2}, 4, false)), result.as_decimal_string() + " should equal 158456325010082058837115804751\n");
}

BOOST_AUTO_TEST_CASE(negate_big_int) {
  BigInt a((unsigned int[]){9728, 1921}, 2, true), b((unsigned int[]){38921}, 1, false);
  BOOST_TEST((-a).equals(BigInt((unsigned int[]){9728, 1921}, 2, false)));
  BOOST_TEST((-b).equals(BigInt((unsigned int[]){38921}, 1, true)));
}

BOOST_AUTO_TEST_CASE(abs_value_big_int) {
  BigInt a((unsigned int[]){9728, 1921}, 2, true), b((unsigned int[]){38921}, 1, false);
  BOOST_TEST((a.abs()).equals(BigInt((unsigned int[]){9728, 1921}, 2, false)));
  BOOST_TEST((b.abs()).equals(BigInt((unsigned int[]){38921}, 1, false)));
}

BOOST_AUTO_TEST_CASE(multiply_by_zero) {
  BigInt a((unsigned int[]){9728, 1921}, 2, false), z;

  BOOST_TEST((a * z).equals(z));
  BOOST_TEST((z * a).equals(z));
}

BOOST_AUTO_TEST_CASE(multiply_by_one) {
  BigInt a((unsigned int[]){9728, 1921}, 2, false), e{(unsigned int)1};

  BOOST_TEST((a * e).equals(a));
  BOOST_TEST((e * a).equals(a));
}

BOOST_AUTO_TEST_CASE(multiply_by_int) {
  BigInt a((unsigned int[]){0xffffffff, 0xffffffff}, 2, false),
    b{(unsigned int)0xffffffff},
    c((unsigned int[]){0x1, 0xffffffff, 0xfffffffe}, 3, false);
  BigInt ab = a * b;
  BigInt ba = b * a;
  BOOST_TEST((ab).equals(c), ab.as_decimal_string() + " should equal " + c.as_decimal_string());
  BOOST_TEST((ba).equals(c), ba.as_decimal_string() + " should equal " + c.as_decimal_string());
}

BOOST_AUTO_TEST_CASE(multiply_with_multiple_blocks_and_overflow) {
  BigInt a((unsigned int[]){0xa3010210, 0x1129, 0xa3}, 3, false),
    b((unsigned int[]){0x31872f01, 0x1b21, 0x20e0, 0x720e170}, 4, true);
  BigInt product = a * b;
  BOOST_TEST(product.equals(BigInt(
    (unsigned int[]){0xf8d1f210, 0x26a7d113, 0x4c085984, 0xe78675f1, 0x57db7049, 0x89ef8aca, 0x4}, 7, true
  )), product.as_decimal_string() + " should equal -28490580746921185856651605176401520623848415388879901815312");
}
