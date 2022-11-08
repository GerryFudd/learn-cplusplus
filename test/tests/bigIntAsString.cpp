#include <Framework.hpp>
#include <math/BigInt.hpp>

using namespace dexenjaeger::math;
using namespace dexenjaeger::test;

TEST(as_string)
{
  BigInt test_int((unsigned int []) { 4789 }, 1, false);
  assert_equal<string>(test_int.as_decimal_string(), "4789");
}

TEST(as_string_negative)
{
  BigInt test_int((unsigned int []) { 4789 }, 1, true);
  assert_equal<string>(test_int.as_decimal_string(), "-4789");
}

TEST(to_string_zero)
{
  BigInt test_int;
  assert_equal<string>(test_int.as_decimal_string(), "0");
}

TEST(to_string_test_large)
{
  BigInt test_int((unsigned int []){0, 1}, 2, false);
  string result = test_int.as_decimal_string();
  assert_equal<string>(result, "4294967296");;
}

TEST(to_string_test_large_complex)
{
  BigInt test_int((unsigned int []){4294967295, 4294967295}, 2, true);
  string result = test_int.as_decimal_string();
  assert_equal<string>(result, "-18446744073709551615");;
}

TEST(to_string_test_large_with_intermediate_zeros)
{
  BigInt test_int((unsigned int []){1410065409, 2}, 2, false);
  string result = test_int.as_decimal_string();
  assert_equal<string>(result, "10000000001");;
}

TEST(to_hex_string_test)
{
  BigInt test_int((unsigned int []) { 0x4789 }, 1, false);
  assert_equal<string>(test_int.as_hex_string(), "0x4789");
}

TEST(to_hex_string_test_negative)
{
  BigInt test_int((unsigned int []) { 0x4789 }, 1, true);
  assert_equal<string>(test_int.as_hex_string(), "-0x4789");
}

TEST(to_hex_string_test_zero)
{
  BigInt test_int;
  assert_equal<string>(test_int.as_hex_string(), "0x0");
}

TEST(to_hex_string_test_large)
{
  BigInt test_int((unsigned int []){0, 1}, 2, false);
  string result = test_int.as_hex_string();
  assert_equal<string>(result, "0x100000000");;
}

TEST(to_hex_string_test_large_complex)
{
  BigInt test_int((unsigned int []){4294967295, 4294967295}, 2, true);
  string result = test_int.as_hex_string();
  assert_equal<string>(result, "-0xffffffffffffffff");;
}

TEST(to_hex_string_test_large_with_intermediate_zeros)
{
  BigInt test_int((unsigned int []){0x1, 0x2}, 2, false);
  string result = test_int.as_hex_string();
  assert_equal<string>(result, "0x200000001");;
}
