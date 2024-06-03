#include <math/BigInt.hpp>
#include <Framework.hpp>
#include <Assertions.inl>

using namespace gerryfudd::math;
using namespace gerryfudd::test;

TEST(abs_value_big_int) {
  unsigned int magnitudea[] = {9728, 1921}, magnitudeb[] = {38921};
  BigInt a(magnitudea, 2, true), b(magnitudeb, 1, false);
  assert_equal<BigInt>(a.abs(), BigInt(magnitudea, 2, false));
  assert_equal<BigInt>(b.abs(), BigInt(magnitudeb, 1, false));
}