#include <math/BigInt.hpp>
#include <Framework.hpp>

using namespace dexenjaeger::math;
using namespace dexenjaeger::test;

TEST(abs_value_big_int) {
  BigInt a((unsigned int[]){9728, 1921}, 2, true), b((unsigned int[]){38921}, 1, false);
  assert_equal<BigInt>(a.abs(), BigInt((unsigned int[]){9728, 1921}, 2, false));
  assert_equal<BigInt>(b.abs(), BigInt((unsigned int[]){38921}, 1, false));
}