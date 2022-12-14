#include <Framework.hpp>
#include <math/BigInt.hpp>

using namespace dexenjaeger::math;
using namespace dexenjaeger::test;

TEST(simple_sum)
{
  BigInt a((unsigned int []){56}, 1, false), b((unsigned int []){109}, 1, false);
  BigInt result = a + b;
  assert_equal<BigInt>(result, BigInt((unsigned int []){165}, 1, false));
}

TEST(negative_number_sum)
{
  BigInt a((unsigned int []){56}, 1, true), b((unsigned int []){109}, 1, true);
  BigInt result = a + b;
  assert_equal<BigInt>(result, BigInt((unsigned int []){165}, 1, true));
}

TEST(sum_with_overflow)
{
  BigInt a((unsigned int []){0x80000000}, 1, false), b((unsigned int []){0x80000000}, 1, false);
  BigInt result = a + b;
  assert_equal<BigInt>(result, BigInt((unsigned int []){0, 1}, 2, false));;
}

TEST(sum_with_multiple_blocks_and_overflow)
{
  BigInt a((unsigned int []){0x80700230, 0xa0475d1e}, 2, false), b((unsigned int []){0xc00e00f2, 0x5fffffff}, 2, false);
  BigInt result = a + b;
  assert_equal<BigInt>(result, BigInt((unsigned int []){0x407e0322, 0x475d1e, 0x1}, 3, false));;
}

TEST(sum_with_several_orders_of_magnitude)
{
  BigInt a((unsigned int []){0x80700230, 0xa0475d1e, 0x43875002, 0x213}, 4, false), b((unsigned int []){0xc00e00f2, 0x5fffffff}, 2, false);
  BigInt result = a + b;
  assert_equal<BigInt>(result, BigInt((unsigned int []){0x407e0322, 0x475d1e, 0x43875003, 0x213}, 4, false));;
}

TEST(sum_with_opposite_signs)
{
  BigInt a((unsigned int []){14}, 1, true), b((unsigned int []){11}, 1, false);
  BigInt result = a + b;
  assert_equal<BigInt>(result, BigInt((unsigned int []){3}, 1, true));;
}

TEST(subtract_with_opposite_signs)
{
  BigInt a((unsigned int []){38}, 1, true), b((unsigned int []){22}, 1, false);
  BigInt result = a - b;
  assert_equal<BigInt>(result, BigInt((unsigned int []){60}, 1, true));;
}

TEST(subtract_with_multiple_blocks)
{
  BigInt a((unsigned int []){0x2473, 0x7429, 0x0, 0x2}, 4, false), b((unsigned int []){0x824, 0xffffffff}, 2, false);
  BigInt result = a - b;
  assert_equal<BigInt>(result, BigInt((unsigned int []){0x1c4f, 0x742a, 0xffffffff, 0x1}, 4, false));;
}

TEST(subtract_with_several_orders_of_magnitude_difference)
{
  BigInt a((unsigned int []){0x2473, 0x7429, 0x0, 0x2}, 4, false), b((unsigned int []){0x824, 0x3a10}, 2, false);
  BigInt result = a - b;
  assert_equal<BigInt>(result, BigInt((unsigned int []){0x1c4f, 0x3a19, 0x0, 0x2}, 4, false));;
}

TEST(negate_big_int) {
  BigInt a((unsigned int[]){9728, 1921}, 2, true), b((unsigned int[]){38921}, 1, false);
  assert_equal<BigInt>(-a, BigInt((unsigned int[]){9728, 1921}, 2, false));
  assert_equal<BigInt>(-b, BigInt((unsigned int[]){38921}, 1, true));
}

TEST(large_diff) {
  // uu = ffffeff00001
  BigInt uu = BigInt(
    (unsigned int []){
      0x1,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,

      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff
    },
    91, false
  );
  // ll = ffffffe0000001
  BigInt ll = BigInt(
    (unsigned int []){
      0x1,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,

      0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0x0,        0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,

      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,

      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff
    },
    102, false
  );
  // pr = 201ffff9fe00004
  BigInt pr = BigInt(
    (unsigned int []){
      0x4,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      
      0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xfffffff9, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      
      0xffffffff, 0x1,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0x0,        0x0,        0x2
    },
    103, false
  );
  // 201ffff9fe00004 //   pr
  //  - ffffeff00001 // - uu
  // ---------------
  // 200ffffaff00003
  BigInt first_diff = pr - uu;
  BigInt first_expected = BigInt(
    (unsigned int []) {
      0x3,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      
      0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xfffffffa, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0x0,        0x0,        0x2
    },
    103, false
  );
  assert_equal<BigInt>(first_diff, first_expected);;

  // expected = 100ffffcff00002
  BigInt second_expected = BigInt(
    (unsigned int []){
      0x2,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      
      0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xfffffffc, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      
      0xffffffff, 0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0x0,        0x0,        0x1
    },
    103, false
  );
  BigInt second_diff = first_expected - ll;
  assert_equal<BigInt>(second_diff, second_expected);;
}

TEST(very_large_sums) {
  // Add the top and bottom halves of a 102 block int with f's
  BigInt a((unsigned int[]){
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,

    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff,
  }, 51, false);
  BigInt b((unsigned int[]){
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,

    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff,
  }, 51, false);
  BigInt first_sum = a + b;
  assert_equal<BigInt>(first_sum, BigInt(
    (unsigned int []){
      0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,

      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0x1
      },
      52, false
    )
  );
  // Add the top 40 and bottom 51 of a 91 block int with f's
  BigInt c((unsigned int[]){
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,

    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
  }, 40, false);
  BigInt d((unsigned int[]){
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,

    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff,
  }, 51, false);
  BigInt second_sum = c + d;
  assert_equal<BigInt>(second_sum, BigInt(
    (unsigned int []){
      0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,

      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
      0x0,        0x1
      },
      52, false
    )
  );
  // Add the top and bottom halves of a 204 block int with f's
  BigInt f((unsigned int[]){
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,

    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,

    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,

    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff,
  }, 102, false);
  BigInt g((unsigned int[]){
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,

    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,

    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,

    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff,
  }, 102, false);
  BigInt last_sum = f + g;
  assert_equal<BigInt>(last_sum, BigInt(
    (unsigned int []){
      0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,

      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,

      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,

      0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
      0xffffffff, 0xffffffff, 0x1
      },
      103, false
    )
  );
}
