// Copyright 2022 <lwolmer, tfelton, stogg, jgerrick>
#include <limits.h>
#include <time.h>
#include "test.h"
#include "utilities.h"
#include "s21_decimal.h"

START_TEST(s21_add_normal) {
    s21_decimal dec1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    int i1 = -5000, i2 = 5000;
    unsigned int seed = time(NULL);
    for (size_t i = 0; i < 100; i++) {
        i1 += rand_r(&seed) % 1000;
        i2 -= rand_r(&seed) % 1000;
        s21_from_int_to_decimal(i1, &dec1);
        s21_from_int_to_decimal(i2, &dec2);
        s21_decimal sum = s21_add(dec1, dec2);
        int res;
        ck_assert_int_eq(s21_from_decimal_to_int(sum, &res), OPR_SUCCESS);
        ck_assert_int_eq(res, i1 + i2);
    }
}
END_TEST

START_TEST(s21_add_normal_float) {
    s21_decimal dec1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    float i1 = -123.5, i2 = -241.7;
    s21_decimal sum;
    s21_from_float_to_decimal(i1, &dec1);
    s21_from_float_to_decimal(i2, &dec2);
    sum = s21_add(dec1, dec2);
    ck_assert(cmp_decimal_str(sum, "-365.1999969"));
    i1 = -1.552354;
    i2 = -1.125123;
    s21_from_float_to_decimal(i1, &dec1);
    s21_from_float_to_decimal(i2, &dec2);
    sum = s21_add(dec1, dec2);
    ck_assert(cmp_decimal_str(sum, "-2.677477"));
    i1 = 0;
    i2 = -1.125123;
    s21_from_float_to_decimal(i1, &dec1);
    s21_from_float_to_decimal(i2, &dec2);
    sum = s21_add(dec1, dec2);
    ck_assert(cmp_decimal_str(sum, "-1.125123"));
    i1 = 1.125123;
    i2 = 0;
    s21_from_float_to_decimal(i1, &dec1);
    s21_from_float_to_decimal(i2, &dec2);
    sum = s21_add(dec1, dec2);
    ck_assert(cmp_decimal_str(sum, "1.125123"));
    i1 = 1.125123;
    i2 = 1.121253;
    s21_from_float_to_decimal(i1, &dec1);
    s21_from_float_to_decimal(i2, &dec2);
    sum = s21_add(dec1, dec2);
    ck_assert(cmp_decimal_str(sum, "2.246376"));
    i1 = 4231.12;
    i2 = 541.121;
    s21_from_float_to_decimal(i1, &dec1);
    s21_from_float_to_decimal(i2, &dec2);
    sum = s21_add(dec1, dec2);
    ck_assert(cmp_decimal_str(sum, "4772.2410889"));
}
END_TEST

START_TEST(s21_add_zero) {
    s21_decimal dec1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    int i1 = -100;
    int i2 = 100;
    s21_from_int_to_decimal(i1, &dec1);
    s21_from_int_to_decimal(i2, &dec2);
    s21_decimal sum = s21_add(dec1, dec2);
    int res;
    ck_assert_int_eq(s21_from_decimal_to_int(sum, &res), OPR_SUCCESS);
    ck_assert_int_eq(res, i1 + i2);
    sum = s21_add(dec2, dec1);
    ck_assert_int_eq(s21_from_decimal_to_int(sum, &res), OPR_SUCCESS);
    ck_assert_int_eq(res, i2 + i1);
}
END_TEST

START_TEST(s21_add_zero1) {
    s21_decimal dec1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    int i1 = -100;
    int i2 = 0;
    s21_from_int_to_decimal(i1, &dec1);
    s21_from_int_to_decimal(i2, &dec2);
    s21_decimal sum = s21_add(dec1, dec2);
    int res;
    ck_assert_int_eq(s21_from_decimal_to_int(sum, &res), OPR_SUCCESS);
    ck_assert_int_eq(res, i1 - i2);
}
END_TEST

START_TEST(s21_add_limits) {
    s21_decimal dec1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    dec1.bits[DEC_HIGH] = INT_MAX;
    dec1.bits[DEC_MID] = INT_MAX;
    set_bit(&dec1, 1, DEC_MID_BITS-1);
    dec1.bits[DEC_LOW] = INT_MAX;
    set_bit(&dec1, 1, DEC_LOW_BITS-1);
    set_bit(&dec2, 1, DEC_HIGH_BITS-1);
    s21_decimal sum = s21_add(dec1, dec2);
    ck_assert_int_eq(sum.value_type, s21_NORMAL_VALUE);
    ck_assert(cmp_decimal_str(sum, "79228162514264337593543950335"));
}
END_TEST

START_TEST(s21_add_limits_round1) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    dec2.bits[DEC_LOW] = 1;
    set_exp(&dec2, 1);
    s21_decimal sum = s21_add(dec1, dec2);
    ck_assert(cmp_decimal_str(sum, "79228162514264337593543950335"));
}
END_TEST

START_TEST(s21_add_limits_round2) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    dec2.bits[DEC_LOW] = 5;
    set_exp(&dec2, 1);
    s21_decimal sum = s21_add(dec1, dec2);
    ck_assert_int_eq(sum.value_type, s21_INFINITY);
    ck_assert(is_zeroed(sum));
}
END_TEST

START_TEST(s21_add_limits_round3) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    dec2.bits[DEC_LOW] = 5;
    set_exp(&dec2, 1);
    set_bit(&dec2, 1, SIGN_BIT);
    s21_decimal sum = s21_add(dec1, dec2);
    ck_assert(cmp_decimal_str(sum, "79228162514264337593543950335"));
}
END_TEST

START_TEST(s21_add_limits_round4) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    dec2.bits[DEC_LOW] = 49;
    set_exp(&dec2, 2);
    s21_decimal sum = s21_add(dec1, dec2);
    ck_assert(cmp_decimal_str(sum, "79228162514264337593543950335"));
}
END_TEST

START_TEST(s21_add_limits_round5) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    dec1.bits[DEC_LOW] -= 1;
    dec2.bits[DEC_LOW] = 49;
    set_exp(&dec2, 2);
    s21_decimal sum = s21_add(dec1, dec2);
    ck_assert(cmp_decimal_str(sum, "79228162514264337593543950334"));
}
END_TEST

START_TEST(s21_add_limits_round6) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    dec1.bits[DEC_LOW] -= 1;
    dec2.bits[DEC_LOW] = 5;
    set_exp(&dec2, 1);
    s21_decimal sum = s21_add(dec1, dec2);
    ck_assert(cmp_decimal_str(sum, "79228162514264337593543950335"));
}
END_TEST

START_TEST(s21_add_limits_float) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = get_max_decimal();
    set_exp(&dec1, 28);
    set_exp(&dec2, 28);
    s21_decimal sum = s21_add(dec1, dec2);
    ck_assert(cmp_decimal_str(sum, "15.845632502852867518708790067"));
}
END_TEST

START_TEST(s21_add_limits_corners) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = get_max_decimal();
    set_bit(&dec1, 1, SIGN_BIT);
    s21_decimal sum = s21_add(dec1, dec2);
    int res;
    ck_assert_int_eq(s21_from_decimal_to_int(sum, &res), OPR_SUCCESS);
    ck_assert_int_eq(res, 0);
    sum = s21_add(dec2, dec1);
}
END_TEST

START_TEST(s21_add_overflow) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = get_max_decimal();
    s21_decimal sum = s21_add(dec1, dec2);
    ck_assert_int_eq(sum.value_type, s21_INFINITY);
    ck_assert(is_zeroed(sum));
    set_bit(&dec1, 1, SIGN_BIT);
    set_bit(&dec2, 1, SIGN_BIT);
    sum = s21_add(dec1, dec2);
    ck_assert_int_eq(sum.value_type, s21_NEGATIVE_INFINITY);
    ck_assert(is_zeroed(sum));
}
END_TEST

START_TEST(s21_add_infinity_normal) {
    s21_decimal dec1 = init_decimal(s21_INFINITY);
    s21_decimal dec2;
    s21_from_int_to_decimal(10, &dec2);
    s21_decimal sum = s21_add(dec1, dec2);
    ck_assert_int_eq(sum.value_type, s21_INFINITY);
    ck_assert(is_zeroed(sum));
    dec1 = init_decimal(s21_NEGATIVE_INFINITY);
    sum = s21_add(dec1, dec2);
    ck_assert_int_eq(sum.value_type, s21_NEGATIVE_INFINITY);
    ck_assert(is_zeroed(sum));
}
END_TEST

START_TEST(s21_add_infinity_reverse) {
    s21_decimal dec1;
    s21_decimal dec2 = init_decimal(s21_INFINITY);
    s21_from_int_to_decimal(10, &dec1);
    s21_decimal sum = s21_add(dec1, dec2);
    ck_assert_int_eq(sum.value_type, s21_INFINITY);
    ck_assert(is_zeroed(sum));
    dec2 = init_decimal(s21_NEGATIVE_INFINITY);
    sum = s21_add(dec1, dec2);
    ck_assert_int_eq(sum.value_type, s21_NEGATIVE_INFINITY);
    ck_assert(is_zeroed(sum));
}
END_TEST

START_TEST(s21_add_infinity_negative_normal) {
    s21_decimal dec1 = init_decimal(s21_INFINITY);
    s21_decimal dec2;
    s21_from_int_to_decimal(-10, &dec2);
    s21_decimal sum = s21_add(dec1, dec2);
    ck_assert_int_eq(sum.value_type, s21_INFINITY);
    ck_assert(is_zeroed(sum));
    dec1 = init_decimal(s21_NEGATIVE_INFINITY);
    sum = s21_add(dec1, dec2);
    ck_assert_int_eq(sum.value_type, s21_NEGATIVE_INFINITY);
    ck_assert(is_zeroed(sum));
}
END_TEST

START_TEST(s21_add_infinity_infinity) {
    s21_decimal dec1 = init_decimal(s21_INFINITY);
    s21_decimal dec2 = init_decimal(s21_INFINITY);
    s21_decimal sum = s21_add(dec1, dec2);
    ck_assert_int_eq(sum.value_type, s21_INFINITY);
    ck_assert(is_zeroed(sum));
    dec1 = init_decimal(s21_NEGATIVE_INFINITY);
    dec2 = init_decimal(s21_NEGATIVE_INFINITY);
    sum = s21_add(dec1, dec2);
    ck_assert_int_eq(sum.value_type, s21_NEGATIVE_INFINITY);
    ck_assert(is_zeroed(sum));
}
END_TEST

START_TEST(s21_add_infinity_negative_infinity) {
    s21_decimal dec1 = init_decimal(s21_INFINITY);
    s21_decimal dec2 = init_decimal(s21_NEGATIVE_INFINITY);
    s21_decimal sum = s21_add(dec1, dec2);
    ck_assert_int_eq(sum.value_type, s21_NAN);
    ck_assert(is_zeroed(sum));
    dec1 = init_decimal(s21_NEGATIVE_INFINITY);
    dec2 = init_decimal(s21_INFINITY);
    sum = s21_add(dec1, dec2);
    ck_assert_int_eq(sum.value_type, s21_NAN);
    ck_assert(is_zeroed(sum));
}
END_TEST

START_TEST(s21_add_nan_normal) {
    s21_decimal dec1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal dec2 = init_decimal(s21_NAN);
    s21_from_int_to_decimal(10, &dec1);
    s21_decimal sum = s21_add(dec1, dec2);
    ck_assert_int_eq(sum.value_type, s21_NAN);
    ck_assert(is_zeroed(sum));
}
END_TEST

START_TEST(s21_add_nan_nan) {
    s21_decimal dec1 = init_decimal(s21_NAN);
    s21_decimal dec2 = init_decimal(s21_NAN);
    s21_decimal sum = s21_add(dec1, dec2);
    ck_assert_int_eq(sum.value_type, s21_NAN);
    ck_assert(is_zeroed(sum));
}
END_TEST

START_TEST(s21_add_nan_infinity) {
    s21_decimal dec1 = init_decimal(s21_INFINITY);
    s21_decimal dec2 = init_decimal(s21_NAN);
    s21_decimal sum = s21_add(dec1, dec2);
    ck_assert_int_eq(sum.value_type, s21_NAN);
    ck_assert(is_zeroed(sum));
}
END_TEST

START_TEST(s21_add_nan_negative_infinity) {
    s21_decimal dec1 = init_decimal(s21_NEGATIVE_INFINITY);
    s21_decimal dec2 = init_decimal(s21_NAN);
    s21_decimal sum = s21_add(dec1, dec2);
    ck_assert_int_eq(sum.value_type, s21_NAN);
    ck_assert(is_zeroed(sum));
}
END_TEST

START_TEST(s21_sub_normal) {
    s21_decimal dec1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    int i1 = -5000, i2 = 5000;
    unsigned int seed = time(NULL);
    for (size_t i = 0; i < 100; i++) {
        i1 += rand_r(&seed) % 1000;
        i2 -= rand_r(&seed) % 1000;
        s21_from_int_to_decimal(i1, &dec1);
        s21_from_int_to_decimal(i2, &dec2);
        s21_decimal diff = s21_sub(dec1, dec2);
        int res;
        ck_assert_int_eq(s21_from_decimal_to_int(diff, &res), OPR_SUCCESS);
        ck_assert_int_eq(res, i1 - i2);
    }
}
END_TEST

START_TEST(s21_sub_normal_float) {
    s21_decimal dec1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    float i1 = -123.5, i2 = -241.7;
    s21_decimal diff;
    s21_from_float_to_decimal(i1, &dec1);
    s21_from_float_to_decimal(i2, &dec2);
    diff = s21_sub(dec1, dec2);
    ck_assert(cmp_decimal_str(diff, "118.1999969"));
    i1 = -1.552354;
    i2 = -1.125123;
    s21_from_float_to_decimal(i1, &dec1);
    s21_from_float_to_decimal(i2, &dec2);
    diff = s21_sub(dec1, dec2);
    ck_assert(cmp_decimal_str(diff, "-0.427231"));
    i1 = 0;
    i2 = -1.125123;
    s21_from_float_to_decimal(i1, &dec1);
    s21_from_float_to_decimal(i2, &dec2);
    diff = s21_sub(dec1, dec2);
    ck_assert(cmp_decimal_str(diff, "1.125123"));
    i1 = 1.125123;
    i2 = 0;
    s21_from_float_to_decimal(i1, &dec1);
    s21_from_float_to_decimal(i2, &dec2);
    diff = s21_sub(dec1, dec2);
    ck_assert(cmp_decimal_str(diff, "1.125123"));
    i1 = 1.125123;
    i2 = 1.121253;
    s21_from_float_to_decimal(i1, &dec1);
    s21_from_float_to_decimal(i2, &dec2);
    diff = s21_sub(dec1, dec2);
    ck_assert(cmp_decimal_str(diff, "0.003870"));
    i1 = 4231.12;
    i2 = 541.121;
    s21_from_float_to_decimal(i1, &dec1);
    s21_from_float_to_decimal(i2, &dec2);
    diff = s21_sub(dec1, dec2);
    ck_assert(cmp_decimal_str(diff, "3689.9991455"));
}
END_TEST

START_TEST(s21_sub_zero) {
    s21_decimal dec1 = init_decimal(s21_NORMAL_VALUE);
    int i1 = -100;
    s21_from_int_to_decimal(i1, &dec1);
    s21_decimal diff = s21_sub(dec1, dec1);
    int res;
    ck_assert_int_eq(s21_from_decimal_to_int(diff, &res), OPR_SUCCESS);
    ck_assert_int_eq(res, i1 - i1);
    s21_decimal dec0 = init_decimal(s21_NORMAL_VALUE);
    int i0 = 0;
    s21_from_int_to_decimal(i0, &dec0);
    diff = s21_sub(dec1, dec0);
    ck_assert_int_eq(s21_from_decimal_to_int(diff, &res), OPR_SUCCESS);
    ck_assert_int_eq(res, i1 - i0);
}
END_TEST

START_TEST(s21_sub_limits) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = get_max_decimal();
    s21_decimal diff = s21_sub(dec1, dec2);
    int res;
    ck_assert_int_eq(s21_from_decimal_to_int(diff, &res), OPR_SUCCESS);
    ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_sub_limits_round1) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    dec2.bits[DEC_LOW] = 6;
    set_exp(&dec2, 1);
    s21_decimal diff = s21_sub(dec1, dec2);
    ck_assert(cmp_decimal_str(diff, "79228162514264337593543950334"));
}
END_TEST

START_TEST(s21_sub_limits_round2) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    dec2.bits[DEC_LOW] = 5;
    set_exp(&dec2, 1);
    s21_decimal diff = s21_sub(dec1, dec2);
    ck_assert(cmp_decimal_str(diff, "79228162514264337593543950335"));
}
END_TEST

START_TEST(s21_sub_limits_round3) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    dec2.bits[DEC_LOW] = 4;
    set_exp(&dec2, 1);
    s21_decimal diff = s21_sub(dec1, dec2);
    ck_assert(cmp_decimal_str(diff, "79228162514264337593543950335"));
}
END_TEST

START_TEST(s21_sub_limits_round4) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    dec2.bits[DEC_LOW] = 49;
    set_exp(&dec2, 2);
    s21_decimal diff = s21_sub(dec1, dec2);
    ck_assert(cmp_decimal_str(diff, "79228162514264337593543950335"));
}
END_TEST

START_TEST(s21_sub_limits_round5) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    dec2.bits[DEC_LOW] = 5;
    set_exp(&dec2, 1);
    set_bit(&dec2, 1, SIGN_BIT);
    s21_decimal diff = s21_sub(dec1, dec2);
    ck_assert_int_eq(diff.value_type, s21_INFINITY);
    ck_assert(is_zeroed(diff));
}
END_TEST

START_TEST(s21_sub_limits_round6) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    dec2.bits[DEC_LOW] = 4;
    set_exp(&dec2, 1);
    set_bit(&dec2, 1, SIGN_BIT);
    s21_decimal diff = s21_sub(dec1, dec2);
    ck_assert(cmp_decimal_str(diff, "79228162514264337593543950335"));
}
END_TEST

START_TEST(s21_sub_infinity_normal) {
    s21_decimal dec1 = init_decimal(s21_INFINITY);
    s21_decimal dec2;
    s21_from_int_to_decimal(10, &dec2);
    s21_decimal diff = s21_sub(dec1, dec2);
    ck_assert_int_eq(diff.value_type, s21_INFINITY);
    ck_assert(is_zeroed(diff));
    dec1 = init_decimal(s21_NEGATIVE_INFINITY);
    diff = s21_sub(dec1, dec2);
    ck_assert_int_eq(diff.value_type, s21_NEGATIVE_INFINITY);
    ck_assert(is_zeroed(diff));
}
END_TEST

START_TEST(s21_sub_infinity_normal_reverse) {
    s21_decimal dec1;
    s21_decimal dec2 = init_decimal(s21_INFINITY);
    s21_from_int_to_decimal(10, &dec1);
    s21_decimal diff = s21_sub(dec1, dec2);
    ck_assert_int_eq(diff.value_type, s21_NEGATIVE_INFINITY);
    ck_assert(is_zeroed(diff));
    dec2 = init_decimal(s21_NEGATIVE_INFINITY);
    diff = s21_sub(dec1, dec2);
    ck_assert_int_eq(diff.value_type, s21_INFINITY);
    ck_assert(is_zeroed(diff));
}
END_TEST

START_TEST(s21_sub_infinity_negative_normal) {
    s21_decimal dec1 = init_decimal(s21_INFINITY);
    s21_decimal dec2;
    s21_from_int_to_decimal(-10, &dec2);
    s21_decimal diff = s21_sub(dec1, dec2);
    ck_assert_int_eq(diff.value_type, s21_INFINITY);
    ck_assert(is_zeroed(diff));
    dec1 = init_decimal(s21_NEGATIVE_INFINITY);
    diff = s21_sub(dec1, dec2);
    ck_assert_int_eq(diff.value_type, s21_NEGATIVE_INFINITY);
    ck_assert(is_zeroed(diff));
}
END_TEST

START_TEST(s21_sub_infinity_negative_normal_reverse) {
    s21_decimal dec1;
    s21_decimal dec2 = init_decimal(s21_INFINITY);
    s21_from_int_to_decimal(-10, &dec1);
    s21_decimal diff = s21_sub(dec1, dec2);
    ck_assert_int_eq(diff.value_type, s21_NEGATIVE_INFINITY);
    ck_assert(is_zeroed(diff));
    dec2 = init_decimal(s21_NEGATIVE_INFINITY);
    diff = s21_sub(dec1, dec2);
    ck_assert_int_eq(diff.value_type, s21_INFINITY);
    ck_assert(is_zeroed(diff));
}
END_TEST

START_TEST(s21_sub_infinity_infinity) {
    s21_decimal dec1 = init_decimal(s21_INFINITY);
    s21_decimal dec2 = init_decimal(s21_INFINITY);
    s21_decimal diff = s21_sub(dec1, dec2);
    ck_assert_int_eq(diff.value_type, s21_NAN);
    ck_assert(is_zeroed(diff));
    dec1 = init_decimal(s21_NEGATIVE_INFINITY);
    dec2 = init_decimal(s21_NEGATIVE_INFINITY);
    diff = s21_sub(dec1, dec2);
    ck_assert_int_eq(diff.value_type, s21_NAN);
    ck_assert(is_zeroed(diff));
}
END_TEST

START_TEST(s21_sub_infinity_negative_infinity) {
    s21_decimal dec1 = init_decimal(s21_INFINITY);
    s21_decimal dec2 = init_decimal(s21_NEGATIVE_INFINITY);
    s21_decimal diff = s21_sub(dec1, dec2);
    ck_assert_int_eq(diff.value_type, s21_INFINITY);
    ck_assert(is_zeroed(diff));
    dec1 = init_decimal(s21_NEGATIVE_INFINITY);
    dec2 = init_decimal(s21_INFINITY);
    diff = s21_sub(dec1, dec2);
    ck_assert_int_eq(diff.value_type, s21_NEGATIVE_INFINITY);
    ck_assert(is_zeroed(diff));
}
END_TEST

START_TEST(s21_sub_overflow) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2;
    dec2 = s21_negate(dec1);
    s21_decimal diff = s21_sub(dec1, dec2);
    ck_assert_int_eq(diff.value_type, s21_INFINITY);
    ck_assert(is_zeroed(diff));
}
END_TEST

START_TEST(s21_sub_overflow1) {
    s21_decimal dec1;
    s21_decimal dec2 = get_max_decimal();
    dec1 = s21_negate(dec2);
    s21_decimal diff = s21_sub(dec1, dec2);
    ck_assert_int_eq(diff.value_type, s21_NEGATIVE_INFINITY);
    ck_assert(is_zeroed(diff));
}
END_TEST

START_TEST(s21_sub_nan) {
    s21_decimal dec1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal dec2 = init_decimal(s21_NAN);
    s21_decimal diff = s21_sub(dec1, dec2);
    ck_assert_int_eq(diff.value_type, s21_NAN);
    ck_assert(is_zeroed(diff));
}
END_TEST

START_TEST(s21_mul_normal_zero) {
    s21_decimal dec1;
    s21_decimal dec2;
    s21_from_int_to_decimal(0, &dec1);
    s21_from_int_to_decimal(0, &dec2);
    s21_decimal product = s21_mul(dec1, dec2);
    ck_assert(cmp_decimal_str(product, "0"));
    s21_from_int_to_decimal(1234, &dec1);
    product = s21_mul(dec1, dec2);
    ck_assert(cmp_decimal_str(product, "0"));
    s21_from_int_to_decimal(-1234, &dec1);
    product = s21_mul(dec1, dec2);
    ck_assert(cmp_decimal_str(product, "0"));
}
END_TEST

START_TEST(s21_mul_normal) {
    s21_decimal dec1;
    s21_decimal dec2;
    int i1 = -2000, i2 = 2000;
    unsigned int seed = time(NULL);
    for (size_t i = 0; i < 100; i++) {
        i1 -= rand_r(&seed) % 100;
        i2 -= rand_r(&seed) % 100;
        s21_from_int_to_decimal(i1, &dec1);
        s21_from_int_to_decimal(i2, &dec2);
        s21_decimal product = s21_mul(dec1, dec2);
        int res;
        ck_assert_int_eq(s21_from_decimal_to_int(product, &res), OPR_SUCCESS);
        ck_assert_int_eq(res, i1 * i2);
    }
}
END_TEST

START_TEST(s21_mul_normal_float) {
    s21_decimal dec1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    float i1 = -123.5, i2 = -241.7;
    s21_decimal res;
    s21_from_float_to_decimal(i1, &dec1);
    s21_from_float_to_decimal(i2, &dec2);
    res = s21_mul(dec1, dec2);
    ck_assert(cmp_decimal_str(res, "29849.94961715"));
    i1 = -1.552354;
    i2 = -1.125123;
    s21_from_float_to_decimal(i1, &dec1);
    s21_from_float_to_decimal(i2, &dec2);
    res = s21_mul(dec1, dec2);
    ck_assert(cmp_decimal_str(res, "1.746589189542"));
    i1 = 0;
    i2 = -1.125123;
    s21_from_float_to_decimal(i1, &dec1);
    s21_from_float_to_decimal(i2, &dec2);
    res = s21_mul(dec1, dec2);
    ck_assert(cmp_decimal_str(res, "0"));
    i1 = 1.125123;
    i2 = 0;
    s21_from_float_to_decimal(i1, &dec1);
    s21_from_float_to_decimal(i2, &dec2);
    res = s21_mul(dec1, dec2);
    ck_assert(cmp_decimal_str(res, "0"));
    i1 = 1.125123;
    i2 = 1.121253;
    s21_from_float_to_decimal(i1, &dec1);
    s21_from_float_to_decimal(i2, &dec2);
    res = s21_mul(dec1, dec2);
    ck_assert(cmp_decimal_str(res, "1.261547539119"));
    i1 = 4231.12;
    i2 = 541.121;
    s21_from_float_to_decimal(i1, &dec1);
    s21_from_float_to_decimal(i2, &dec2);
    res = s21_mul(dec1, dec2);
    ck_assert(cmp_decimal_str(res, "2289547.82919868188324"));
}
END_TEST

START_TEST(s21_mul_float_positive) {
    s21_decimal dec1;
    s21_decimal dec2;
    float f1 = 1234.2;
    float f2 = 34.1;
    s21_from_float_to_decimal(f1, &dec1);
    s21_from_float_to_decimal(f2, &dec2);
    s21_decimal product = s21_mul(dec1, dec2);
    float res;
    s21_from_decimal_to_float(product, &res);
    ck_assert_float_eq(res, f1 * f2);
}
END_TEST

START_TEST(s21_mul_float_negative) {
    s21_decimal dec1;
    s21_decimal dec2;
    float f1 = 1234.2;
    float f2 = -34.1;
    s21_from_float_to_decimal(f1, &dec1);
    s21_from_float_to_decimal(f2, &dec2);
    s21_decimal product = s21_mul(dec1, dec2);
    float res;
    s21_from_decimal_to_float(product, &res);
    ck_assert_float_eq(res, f1 * f2);
}
END_TEST

START_TEST(s21_mul_limits) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2;
    s21_from_int_to_decimal(1, &dec2);
    set_exp(&dec1, 12);
    set_exp(&dec2, 12);
    s21_decimal product = s21_mul(dec1, dec2);
    ck_assert(cmp_decimal_str(product, "79228.162514264337593543950335"));
}
END_TEST

START_TEST(s21_mul_limits_inf) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = get_max_decimal();
    s21_decimal product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_INFINITY);
    ck_assert(is_zeroed(product));
}
END_TEST

START_TEST(s21_mul_limits_neg_inf) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = get_max_decimal();
    set_bit(&dec1, 1, SIGN_BIT);
    s21_decimal product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_NEGATIVE_INFINITY);
    ck_assert(is_zeroed(product));
}
END_TEST

START_TEST(s21_mul_limits_max_exp) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = get_max_decimal();
    set_exp(&dec1, 28);
    set_exp(&dec2, 28);
    s21_decimal product = s21_mul(dec1, dec2);
    ck_assert(cmp_decimal_str(product, "62.771017353866807638357894230"));
}
END_TEST

START_TEST(s21_mul_limits_max_exp_sign) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2 = get_max_decimal();
    set_bit(&dec1, 1, SIGN_BIT);
    set_exp(&dec1, 28);
    set_exp(&dec2, 28);
    s21_decimal product = s21_mul(dec1, dec2);
    ck_assert(cmp_decimal_str(product, "-62.771017353866807638357894230"));
}
END_TEST

START_TEST(s21_mul_limits_zero_max_exp) {
    s21_decimal dec1;
    s21_decimal dec2;
    s21_from_int_to_decimal(0, &dec1);
    s21_from_int_to_decimal(0, &dec2);
    set_exp(&dec1, 28);
    set_exp(&dec2, 28);
    s21_decimal product = s21_mul(dec1, dec2);
    ck_assert(cmp_decimal_str(product, "0"));
}
END_TEST

START_TEST(s21_mul_limits_zero_max_exp_sign) {
    s21_decimal dec1;
    s21_decimal dec2;
    s21_from_int_to_decimal(0, &dec1);
    s21_from_int_to_decimal(0, &dec2);
    set_bit(&dec1, 1, SIGN_BIT);
    set_exp(&dec1, 28);
    set_exp(&dec2, 28);
    s21_decimal product = s21_mul(dec1, dec2);
    ck_assert(cmp_decimal_str(product, "0"));
}
END_TEST

START_TEST(s21_mul_round) {
    s21_decimal dec1 = get_max_decimal();
    s21_decimal dec2;
    s21_from_int_to_decimal(988888, &dec2);
    set_exp(&dec1, 7);
    s21_decimal product = s21_mul(dec1, dec2);
    // 7834777917240583227420448995.887748 rounds to 7834777917240583227420448995.9
    ck_assert(cmp_decimal_str(product, "7834777917240583227420448995.9"));
}
END_TEST

START_TEST(s21_mul_inf) {
    // inf * inf
    s21_decimal dec1 = init_decimal(s21_INFINITY);
    s21_decimal dec2 = init_decimal(s21_INFINITY);
    s21_decimal product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_INFINITY);
    ck_assert(is_zeroed(product));
    // -inf * -inf
    dec1 = init_decimal(s21_NEGATIVE_INFINITY);
    dec2 = init_decimal(s21_NEGATIVE_INFINITY);
    product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_INFINITY);
    ck_assert(is_zeroed(product));
}
END_TEST

START_TEST(s21_mul_inf_normal) {
    // inf * +normal
    s21_decimal dec1 = init_decimal(s21_INFINITY);
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    s21_from_int_to_decimal(1234, &dec2);
    s21_decimal product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_INFINITY);
    ck_assert(is_zeroed(product));
    // +normal * inf
    dec1 = init_decimal(s21_NORMAL_VALUE);
    dec2 = init_decimal(s21_INFINITY);
    s21_from_int_to_decimal(1234, &dec1);
    product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_INFINITY);
    ck_assert(is_zeroed(product));
    // -inf * -normal
    dec1 = init_decimal(s21_NEGATIVE_INFINITY);
    dec2 = init_decimal(s21_NORMAL_VALUE);
    s21_from_int_to_decimal(-1234, &dec2);
    product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_INFINITY);
    ck_assert(is_zeroed(product));
    // -normal * -inf
    dec1 = init_decimal(s21_NORMAL_VALUE);
    dec2 = init_decimal(s21_NEGATIVE_INFINITY);
    s21_from_int_to_decimal(-1234, &dec1);
    product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_INFINITY);
    ck_assert(is_zeroed(product));
}
END_TEST

START_TEST(s21_mul_neg_inf) {
    // inf * -inf
    s21_decimal dec1 = init_decimal(s21_INFINITY);
    s21_decimal dec2 = init_decimal(s21_NEGATIVE_INFINITY);
    s21_decimal product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_NEGATIVE_INFINITY);
    ck_assert(is_zeroed(product));
    // -inf * inf
    dec1 = init_decimal(s21_NEGATIVE_INFINITY);
    dec2 = init_decimal(s21_INFINITY);
    product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_NEGATIVE_INFINITY);
    ck_assert(is_zeroed(product));
}
END_TEST

START_TEST(s21_mul_neg_inf_normal) {
    // -inf * +normal
    s21_decimal dec1 = init_decimal(s21_NEGATIVE_INFINITY);
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    s21_from_int_to_decimal(1234, &dec2);
    s21_decimal product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_NEGATIVE_INFINITY);
    ck_assert(is_zeroed(product));
    // +normal * -inf
    dec1 = init_decimal(s21_NORMAL_VALUE);
    dec2 = init_decimal(s21_NEGATIVE_INFINITY);
    s21_from_int_to_decimal(1234, &dec1);
    product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_NEGATIVE_INFINITY);
    ck_assert(is_zeroed(product));
    // inf * -normal
    dec1 = init_decimal(s21_INFINITY);
    dec2 = init_decimal(s21_NORMAL_VALUE);
    s21_from_int_to_decimal(-1234, &dec2);
    product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_NEGATIVE_INFINITY);
    ck_assert(is_zeroed(product));
    // -normal * inf
    dec1 = init_decimal(s21_NORMAL_VALUE);
    dec2 = init_decimal(s21_INFINITY);
    s21_from_int_to_decimal(-1234, &dec1);
    product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_NEGATIVE_INFINITY);
    ck_assert(is_zeroed(product));
}
END_TEST

START_TEST(s21_mul_nan_nan) {
    s21_decimal dec1 = init_decimal(s21_NAN);
    s21_decimal dec2 = init_decimal(s21_NAN);
    s21_decimal product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_NAN);
    ck_assert(is_zeroed(product));
}
END_TEST

START_TEST(s21_mul_nan_number) {
    s21_decimal dec1 = init_decimal(s21_NAN);
    s21_decimal dec2 = init_decimal(s21_NORMAL_VALUE);
    s21_from_int_to_decimal(1234, &dec2);
    s21_decimal product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_NAN);
    ck_assert(is_zeroed(product));
}
END_TEST

START_TEST(s21_mul_nan_inf) {
    s21_decimal dec1 = init_decimal(s21_INFINITY);
    s21_decimal dec2 = init_decimal(s21_NAN);
    s21_decimal product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_NAN);
    ck_assert(is_zeroed(product));
    dec1.value_type = s21_NEGATIVE_INFINITY;
    product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_NAN);
    ck_assert(is_zeroed(product));
    dec1 = init_decimal(s21_NAN);
    dec2.value_type = s21_INFINITY;
    product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_NAN);
    ck_assert(is_zeroed(product));
    dec2.value_type = s21_NEGATIVE_INFINITY;
    product = s21_mul(dec1, dec2);
    ck_assert_int_eq(product.value_type, s21_NAN);
    ck_assert(is_zeroed(product));
}
END_TEST

START_TEST(s21_div_zero_divisor) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 1;
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "inf"));
}
END_TEST

START_TEST(s21_div_test1) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 5;
    d2.bits[DEC_LOW] = 11;
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "0.4545454545454545454545454545"));
}
END_TEST

START_TEST(s21_div_test2) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 10;
    d2.bits[DEC_LOW] = 3;
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "3.3333333333333333333333333333"));
}
END_TEST

START_TEST(s21_div_test3) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 10;
    d2.bits[DEC_LOW] = 6;
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "1.6666666666666666666666666667"));
}
END_TEST

START_TEST(s21_div_test4) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = get_max_decimal();
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 1;
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "0.0000000000000000000000000000"));
}
END_TEST

START_TEST(s21_div_test5) {
    s21_decimal d1 = get_max_decimal();
    s21_decimal d2 = get_max_decimal();
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] -= 5;
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "0.9999999999999999999999999999"));
}
END_TEST

START_TEST(s21_div_test6) {
    s21_decimal d1 = get_max_decimal();
    s21_decimal d2 = get_max_decimal();
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] -= 1;
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "1.0000000000000000000000000000"));
}
END_TEST

START_TEST(s21_div_test7) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 1;
    d2.bits[DEC_LOW] = 1;
    set_exp(&d1, 28);
    set_exp(&d2, 14);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "0.0000000000000100000000000000"));
}
END_TEST

START_TEST(s21_div_test8) {
    s21_decimal d1 = get_max_decimal();
    s21_decimal d2 = get_max_decimal();
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] -= 5;
    d2 = s21_negate(d2);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "-0.9999999999999999999999999999"));
}
END_TEST

START_TEST(s21_div_test9) {
    s21_decimal d1 = get_max_decimal();
    s21_decimal d2 = get_max_decimal();
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] -= 5;
    d1 = s21_negate(d1);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "-0.9999999999999999999999999999"));
}
END_TEST

START_TEST(s21_div_test10) {
    s21_decimal d1 = get_max_decimal();
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d2.bits[DEC_LOW] = 5;
    set_exp(&d2, 1);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "inf"));
}
END_TEST

START_TEST(s21_div_test11) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 1;
    d2.bits[DEC_LOW] = 1;
    set_exp(&d1, 1);
    set_exp(&d2, 28);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "1000000000000000000000000000.0"));
}
END_TEST

START_TEST(s21_div_test12) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = get_max_decimal();
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 12345;
    set_exp(&d2, 28);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "1558.1580599925425180492462622"));
}
END_TEST

START_TEST(s21_div_test13) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = get_max_decimal();
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 1;
    set_exp(&d2, 28);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "0.1262177448353618888658765704"));
}
END_TEST

START_TEST(s21_div_test14) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = get_max_decimal();
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 1;
    set_exp(&d1, 2);
    set_exp(&d2, 28);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "0.0012621774483536188886587657"));
}
END_TEST

START_TEST(s21_div_negzero_divisor) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 1;
    d2 = s21_negate(d2);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "-inf"));
}
END_TEST

START_TEST(s21_div_zero_zero) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "nan"));
}
END_TEST

START_TEST(s21_div_max) {
    s21_decimal d1 = get_max_decimal();
    s21_decimal d2 = get_max_decimal();
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d2.bits[DEC_LOW]-= 1000;
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "1.0000000000000000000000000126"));
}
END_TEST

START_TEST(s21_div_max_min) {
    s21_decimal d1 = get_max_decimal();
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d2.bits[DEC_LOW] = 1;
    set_exp(&d1, 28);
    set_exp(&d2, 28);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "79228162514264337593543950335"));
}
END_TEST

START_TEST(s21_div_cicle) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 100;
    FILE *div = fopen("tests/div_C#.txt", "r");
    for (int i = 1; i <= 101; i++) {
        char buf[100];
        d2.bits[DEC_LOW] = i;
        result = s21_div(d1, d2);
        fgets(buf, 100, div);
        buf[strlen(buf) - 1] = 0;
        ck_assert(cmp_decimal_str(result, buf));
    }
    fclose(div);
}
END_TEST

START_TEST(s21_div_cicle2) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 33;
    d2.bits[DEC_LOW] = 7;
    FILE *div = fopen("tests/divexpdivisor_c#.txt", "r");
    for (int i = 0; i <= 27; i++) {
        char buf[100];
        set_exp(&d2, i);
        result = s21_div(d1, d2);
        fgets(buf, 100, div);
        buf[strlen(buf) - 1] = 0;
        ck_assert(cmp_decimal_str(result, buf));
    }
    fclose(div);
}
END_TEST

START_TEST(s21_div_cicle3) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 33;
    d2.bits[DEC_LOW] = 7;
    FILE *div = fopen("tests/divexpdividend_c#.txt", "r");
    for (int i = 0; i <= 27; i++) {
        char buf[100];
        set_exp(&d1, i);
        result = s21_div(d1, d2);
        fgets(buf, 100, div);
        buf[strlen(buf) - 1] = 0;
        ck_assert(cmp_decimal_str(result, buf));
    }
    fclose(div);
}
END_TEST

START_TEST(s21_div_inf_divisor) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = init_decimal(s21_INFINITY);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 1;
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "0"));

    d1 = init_decimal(s21_NORMAL_VALUE);
    d2 = init_decimal(s21_NEGATIVE_INFINITY);
    result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 1;
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "-0"));

    d1 = init_decimal(s21_INFINITY);
    d2 = init_decimal(s21_NORMAL_VALUE);
    result = init_decimal(s21_NORMAL_VALUE);
    d2.bits[DEC_LOW] = 1;
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "inf"));

    d1 = init_decimal(s21_NEGATIVE_INFINITY);
    d2 = init_decimal(s21_NORMAL_VALUE);
    result = init_decimal(s21_NORMAL_VALUE);
    d2.bits[DEC_LOW] = 1;
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "-inf"));

    d1 = init_decimal(s21_INFINITY);
    d2 = init_decimal(s21_NORMAL_VALUE);
    result = init_decimal(s21_NORMAL_VALUE);
    d2.bits[DEC_LOW] = 1;
    d2 = s21_negate(d2);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "-inf"));

    d1 = init_decimal(s21_NEGATIVE_INFINITY);
    d2 = init_decimal(s21_NORMAL_VALUE);
    result = init_decimal(s21_NORMAL_VALUE);
    d2.bits[DEC_LOW] = 1;
    d2 = s21_negate(d2);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "inf"));
}
END_TEST

START_TEST(s21_div_nan) {
    s21_decimal d1 = init_decimal(s21_NAN);
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d2.bits[DEC_LOW] = 1;
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "nan"));

    d1 = init_decimal(s21_NORMAL_VALUE);
    d2 = init_decimal(s21_NAN);
    result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 1;
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "nan"));

    d1 = init_decimal(s21_NAN);
    d2 = init_decimal(s21_NAN);
    result = init_decimal(s21_NORMAL_VALUE);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "nan"));

    d1 = init_decimal(s21_INFINITY);
    d2 = init_decimal(s21_INFINITY);
    result = init_decimal(s21_NORMAL_VALUE);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "nan"));

    d1 = init_decimal(s21_NEGATIVE_INFINITY);
    d2 = init_decimal(s21_INFINITY);
    result = init_decimal(s21_NORMAL_VALUE);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "nan"));

    d1 = init_decimal(s21_INFINITY);
    d2 = init_decimal(s21_NEGATIVE_INFINITY);
    result = init_decimal(s21_NORMAL_VALUE);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "nan"));

    d1 = init_decimal(s21_NEGATIVE_INFINITY);
    d2 = init_decimal(s21_NEGATIVE_INFINITY);
    result = init_decimal(s21_NORMAL_VALUE);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "nan"));
}
END_TEST

START_TEST(s21_div_overflow) {
    s21_decimal d1 = get_max_decimal();
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d2.bits[DEC_LOW]= 1;
    set_exp(&d2, 1);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "inf"));
}
END_TEST

START_TEST(s21_div_zero_dividend) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d2.bits[DEC_LOW]= 5;
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "0"));
    d2 = init_decimal(s21_INFINITY);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "0"));
    d2 = init_decimal(s21_NEGATIVE_INFINITY);
    result = s21_div(d1, d2);
    ck_assert(cmp_decimal_str(result, "-0"));
}
END_TEST

START_TEST(s21_mod_def) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal add_num = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    add_num.bits[DEC_LOW] = 13;
    d1.bits[DEC_LOW] = 1;
    d1.bits[DEC_MID] = 1;
    d1.bits[DEC_HIGH] = 1;
    d2.bits[DEC_MID] = 7;
    FILE *div = fopen("tests/mod_c#_def.txt", "r");
    for (int i = 0; i <= 20; i++) {
        char buf[100];
        result = s21_mod(d1, d2);
        d2 = s21_add(d2, add_num);
        fgets(buf, 100, div);
        buf[strlen(buf) - 1] = 0;
        ck_assert(cmp_decimal_str(result, buf));
    }
    fclose(div);
}
END_TEST

START_TEST(s21_mod_max) {
    s21_decimal d1 = get_max_decimal();
    s21_decimal d2 = get_max_decimal();
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d2.bits[DEC_LOW] -= 1000;
    set_exp(&d2, 1);
    result = s21_mod(d1, d2);
    ck_assert(cmp_decimal_str(result, "1000.0"));
}
END_TEST

START_TEST(s21_mod_sign) {
    s21_decimal d1 = get_max_decimal();
    s21_decimal d2 = get_max_decimal();
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d2.bits[DEC_LOW] -= 1000;
    set_exp(&d2, 1);
    for (int i = 0; i < 4; i++) {
        if (i % 2) {
            d1 = s21_negate(d1);
        } else {
            d2 = s21_negate(d2);
        }
        result = s21_mod(d1, d2);
        if (i == 1 || i == 2) {
            ck_assert(cmp_decimal_str(result, "-1000.0"));
        } else {
            ck_assert(cmp_decimal_str(result, "1000.0"));
        }
    }
}
END_TEST

START_TEST(s21_mod_zero_divisor) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 1;
    result = s21_mod(d1, d2);
    ck_assert(cmp_decimal_str(result, "nan"));
}
END_TEST

START_TEST(s21_mod_nan) {
    s21_decimal d1 = init_decimal(s21_NAN);
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    result = s21_mod(d1, d2);
    ck_assert(cmp_decimal_str(result, "nan"));

    d1 = init_decimal(s21_NORMAL_VALUE);
    d2 = init_decimal(s21_NAN);
    result = init_decimal(s21_NORMAL_VALUE);
    result = s21_mod(d1, d2);
    ck_assert(cmp_decimal_str(result, "nan"));

    d1 = init_decimal(s21_NAN);
    d2 = init_decimal(s21_NAN);
    result = init_decimal(s21_NORMAL_VALUE);
    result = s21_mod(d1, d2);
    ck_assert(cmp_decimal_str(result, "nan"));

    d1 = init_decimal(s21_INFINITY);
    d2 = init_decimal(s21_NORMAL_VALUE);
    result = init_decimal(s21_NORMAL_VALUE);
    result = s21_mod(d1, d2);
    ck_assert(cmp_decimal_str(result, "nan"));

    d1 = init_decimal(s21_NEGATIVE_INFINITY);
    d2 = init_decimal(s21_NORMAL_VALUE);
    result = init_decimal(s21_NORMAL_VALUE);
    result = s21_mod(d1, d2);
    ck_assert(cmp_decimal_str(result, "nan"));
}
END_TEST

START_TEST(s21_mod_inf_divisor) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = init_decimal(s21_INFINITY);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 1;
    result = s21_mod(d1, d2);
    ck_assert(cmp_decimal_str(result, "1"));

    d1 = init_decimal(s21_NORMAL_VALUE);
    d2 = init_decimal(s21_NEGATIVE_INFINITY);
    result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 1;
    result = s21_mod(d1, d2);
    ck_assert(cmp_decimal_str(result, "1"));
}
END_TEST

START_TEST(s21_mod_scale) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d1.bits[DEC_LOW] = 10;
    d2.bits[DEC_LOW] = 3;
    FILE *div = fopen("tests/mod_c#_scale.txt", "r");
    for (int i = 0; i <= 28; i++) {
        set_exp(&d1, i);
        char buf[100];
        result = s21_mod(d1, d2);
        fgets(buf, 100, div);
        buf[strlen(buf) - 1] = 0;
        ck_assert(cmp_decimal_str(result, buf));
    }
    fclose(div);
}
END_TEST

START_TEST(s21_mod_overflow) {
    s21_decimal d1 = get_max_decimal();
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d2.bits[DEC_LOW] = 777;
    set_exp(&d2, 4);
    result = s21_mod(d1, d2);
    ck_assert(cmp_decimal_str(result, "0.0756"));
}
END_TEST

START_TEST(s21_mod_zero_dividend) {
    s21_decimal d1 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal d2 = init_decimal(s21_NORMAL_VALUE);
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    d2.bits[DEC_LOW] = 777;
    result = s21_mod(d1, d2);
    ck_assert(cmp_decimal_str(result, "0"));
}
END_TEST

// Функция создания набора тестов.
Suite *arithmetic_suite(void) {
    Suite *suite = suite_create("Arithmetics");
    // Набор разбивается на группы тестов, разделённых по каким-либо критериям.
    TCase *ADD = tcase_create("s21_addition");
    tcase_add_test(ADD, s21_add_normal);
    tcase_add_test(ADD, s21_add_normal_float);
    tcase_add_test(ADD, s21_add_zero);
    tcase_add_test(ADD, s21_add_zero1);
    tcase_add_test(ADD, s21_add_limits);
    tcase_add_test(ADD, s21_add_limits_round1);
    tcase_add_test(ADD, s21_add_limits_round2);
    tcase_add_test(ADD, s21_add_limits_round3);
    tcase_add_test(ADD, s21_add_limits_round4);
    tcase_add_test(ADD, s21_add_limits_round5);
    tcase_add_test(ADD, s21_add_limits_round6);
    tcase_add_test(ADD, s21_add_limits_float);
    tcase_add_test(ADD, s21_add_limits_corners);
    tcase_add_test(ADD, s21_add_overflow);
    tcase_add_test(ADD, s21_add_infinity_normal);
    tcase_add_test(ADD, s21_add_infinity_reverse);
    tcase_add_test(ADD, s21_add_infinity_negative_normal);
    tcase_add_test(ADD, s21_add_infinity_infinity);
    tcase_add_test(ADD, s21_add_infinity_negative_infinity);
    tcase_add_test(ADD, s21_add_nan_normal);
    tcase_add_test(ADD, s21_add_nan_nan);
    tcase_add_test(ADD, s21_add_nan_infinity);
    tcase_add_test(ADD, s21_add_nan_negative_infinity);
    TCase *SUB = tcase_create("s21_subtraction");
    tcase_add_test(SUB, s21_sub_normal);
    tcase_add_test(SUB, s21_sub_normal_float);
    tcase_add_test(SUB, s21_sub_zero);
    tcase_add_test(SUB, s21_sub_limits);
    tcase_add_test(ADD, s21_sub_limits_round1);
    tcase_add_test(ADD, s21_sub_limits_round2);
    tcase_add_test(ADD, s21_sub_limits_round3);
    tcase_add_test(ADD, s21_sub_limits_round4);
    tcase_add_test(ADD, s21_sub_limits_round5);
    tcase_add_test(ADD, s21_sub_limits_round6);
    tcase_add_test(SUB, s21_sub_overflow);
    tcase_add_test(SUB, s21_sub_overflow1);
    tcase_add_test(SUB, s21_sub_infinity_normal);
    tcase_add_test(SUB, s21_sub_infinity_normal_reverse);
    tcase_add_test(SUB, s21_sub_infinity_negative_normal);
    tcase_add_test(SUB, s21_sub_infinity_negative_normal_reverse);
    tcase_add_test(SUB, s21_sub_infinity_infinity);
    tcase_add_test(SUB, s21_sub_infinity_negative_infinity);
    tcase_add_test(SUB, s21_sub_nan);
    TCase *MUL = tcase_create("s21_multiplication");
    tcase_add_test(MUL, s21_mul_normal_zero);
    tcase_add_test(MUL, s21_mul_normal);
    tcase_add_test(MUL, s21_mul_normal_float);
    tcase_add_test(MUL, s21_mul_float_positive);
    tcase_add_test(MUL, s21_mul_float_negative);
    tcase_add_test(MUL, s21_mul_limits);
    tcase_add_test(MUL, s21_mul_limits_inf);
    tcase_add_test(MUL, s21_mul_limits_neg_inf);
    tcase_add_test(MUL, s21_mul_limits_max_exp);
    tcase_add_test(MUL, s21_mul_limits_max_exp_sign);
    tcase_add_test(MUL, s21_mul_limits_zero_max_exp);
    tcase_add_test(MUL, s21_mul_limits_zero_max_exp_sign);
    tcase_add_test(MUL, s21_mul_round);
    tcase_add_test(MUL, s21_mul_inf);
    tcase_add_test(MUL, s21_mul_inf_normal);
    tcase_add_test(MUL, s21_mul_neg_inf);
    tcase_add_test(MUL, s21_mul_neg_inf_normal);
    tcase_add_test(MUL, s21_mul_nan_nan);
    tcase_add_test(MUL, s21_mul_nan_number);
    tcase_add_test(MUL, s21_mul_nan_inf);
    TCase *DIV = tcase_create("Division");
    tcase_add_test(DIV, s21_div_test1);
    tcase_add_test(DIV, s21_div_test2);
    tcase_add_test(DIV, s21_div_test3);
    tcase_add_test(DIV, s21_div_test4);
    tcase_add_test(DIV, s21_div_test5);
    tcase_add_test(DIV, s21_div_test6);
    tcase_add_test(DIV, s21_div_test7);
    tcase_add_test(DIV, s21_div_test8);
    tcase_add_test(DIV, s21_div_test9);
    tcase_add_test(DIV, s21_div_test10);
    tcase_add_test(DIV, s21_div_test11);
    tcase_add_test(DIV, s21_div_test12);
    tcase_add_test(DIV, s21_div_test13);
    tcase_add_test(DIV, s21_div_test14);
    tcase_add_test(DIV, s21_div_zero_divisor);
    tcase_add_test(DIV, s21_div_negzero_divisor);
    tcase_add_test(DIV, s21_div_zero_zero);
    tcase_add_test(DIV, s21_div_max);
    tcase_add_test(DIV, s21_div_max_min);
    tcase_add_test(DIV, s21_div_cicle);
    tcase_add_test(DIV, s21_div_cicle2);
    tcase_add_test(DIV, s21_div_cicle3);
    tcase_add_test(DIV, s21_div_inf_divisor);
    tcase_add_test(DIV, s21_div_nan);
    tcase_add_test(DIV, s21_div_overflow);
    tcase_add_test(DIV, s21_div_zero_dividend);
    TCase *MOD = tcase_create("Mod");
    tcase_add_test(MOD, s21_mod_def);
    tcase_add_test(MOD, s21_mod_max);
    tcase_add_test(MOD, s21_mod_sign);
    tcase_add_test(MOD, s21_mod_zero_divisor);
    tcase_add_test(MOD, s21_mod_nan);
    tcase_add_test(MOD, s21_mod_inf_divisor);
    tcase_add_test(MOD, s21_mod_scale);
    tcase_add_test(MOD, s21_mod_overflow);
    tcase_add_test(MOD, s21_mod_zero_dividend);
    // Добавление теста в тестовый набор.
    suite_add_tcase(suite, DIV);
    suite_add_tcase(suite, ADD);
    suite_add_tcase(suite, SUB);
    suite_add_tcase(suite, MUL);
    suite_add_tcase(suite, MOD);
    return suite;
}
