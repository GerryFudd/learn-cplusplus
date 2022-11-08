#include <math/BigInt.hpp>
#include <Framework.hpp>
#include <Assertions.hh>

using namespace math;

TEST(abs_value_big_int) {
  BigInt a((unsigned int[]){9728, 1921}, 2, true), b((unsigned int[]){38921}, 1, false);
  test::assert_equal<BigInt>(a.abs(), BigInt((unsigned int[]){9728, 1921}, 2, false));
  test::assert_equal<BigInt>(b.abs(), BigInt((unsigned int[]){38921}, 1, false));
}