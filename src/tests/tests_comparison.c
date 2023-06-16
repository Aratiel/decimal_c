// Copyright 2022 <lwolmer, tfelton, stogg, jgerrick>
#include <time.h>
#include "test.h"

START_TEST(s21_is_equal_normal) {
    s21_decimal a, b;
    s21_from_int_to_decimal(21, &a);
    s21_from_int_to_decimal(21, &b);
    ck_assert(s21_is_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_equal_zero) {
    s21_decimal a = init_decimal(s21_NORMAL_VALUE);
    s21_decimal b = init_decimal(s21_NORMAL_VALUE);
    set_bit(&b, 1, SIGN_BIT);
    ck_assert(s21_is_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_equal_normal_not_equal) {
    s21_decimal a, b;
    s21_from_int_to_decimal(21, &a);
    s21_from_int_to_decimal(24, &b);
    ck_assert(s21_is_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_equal_normal_float) {
    s21_decimal a, b;
    s21_from_float_to_decimal(21.1234, &a);
    s21_from_float_to_decimal(21.1234, &b);
    ck_assert(s21_is_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_equal_limits_infinitesimal) {
    s21_decimal a = init_decimal(s21_NORMAL_VALUE);
    s21_decimal b = init_decimal(s21_NORMAL_VALUE);
    set_exp(&b, 28);
    ck_assert(s21_is_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_equal_float_not_equal) {
    s21_decimal a, b;
    s21_from_float_to_decimal(21.1234, &a);
    s21_from_float_to_decimal(24.1234, &b);
    ck_assert(s21_is_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_equal_float_not_equal_1) {
    s21_decimal a, b;
    s21_from_float_to_decimal(21.1234, &a);
    s21_from_float_to_decimal(21.12345, &b);
    ck_assert(s21_is_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_equal_inf) {
    s21_decimal a, b;
    a = init_decimal(s21_INFINITY);
    b = init_decimal(s21_INFINITY);
    ck_assert(s21_is_equal(a, b) == OPR_SUCCESS);
    a = init_decimal(s21_NEGATIVE_INFINITY);
    b = init_decimal(s21_NEGATIVE_INFINITY);
    ck_assert(s21_is_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_equal_inf_not_equal) {
    s21_decimal a, b;
    a = init_decimal(s21_NEGATIVE_INFINITY);
    b = init_decimal(s21_INFINITY);
    ck_assert(s21_is_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_equal_error) {
    s21_decimal a, b;
    a = init_decimal(s21_NAN);
    s21_from_int_to_decimal(21, &b);
    ck_assert(s21_is_equal(a, b) == OPR_ERROR);
    b = init_decimal(s21_INFINITY);
    ck_assert(s21_is_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_equal_limits_zero) {
    s21_decimal a = init_decimal(s21_NORMAL_VALUE);
    s21_decimal b = init_decimal(s21_NORMAL_VALUE);
    ck_assert(s21_is_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_equal_limits_zero_float) {
    s21_decimal a = init_decimal(s21_NORMAL_VALUE);
    s21_decimal b = init_decimal(s21_NORMAL_VALUE);
    set_exp(&a, 28);
    set_exp(&b, 28);
    ck_assert(s21_is_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_equal_limits_max) {
    s21_decimal a = get_max_decimal();
    s21_decimal b = get_max_decimal();
    ck_assert(s21_is_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_equal_limits_max_float) {
    s21_decimal a = get_max_decimal();
    s21_decimal b = get_max_decimal();
    set_exp(&a, 28);
    set_exp(&b, 28);
    ck_assert(s21_is_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_not_equal_normal) {
    s21_decimal a, b;
    s21_from_int_to_decimal(21, &a);
    s21_from_int_to_decimal(24, &b);
    ck_assert(s21_is_not_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_not_equal_normal_opposite) {
    s21_decimal a, b;
    s21_from_int_to_decimal(-21, &a);
    s21_from_int_to_decimal(21, &b);
    ck_assert(s21_is_not_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_not_equal_normal_equal) {
    s21_decimal a, b;
    s21_from_int_to_decimal(21, &a);
    s21_from_int_to_decimal(21, &b);
    ck_assert(s21_is_not_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_not_equal_normal_float) {
    s21_decimal a, b;
    s21_from_float_to_decimal(21.1234, &a);
    s21_from_float_to_decimal(21.12324, &b);
    ck_assert(s21_is_not_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_not_equal_float_equal) {
    s21_decimal a, b;
    s21_from_float_to_decimal(21.1234, &a);
    s21_from_float_to_decimal(21.1234, &b);
    ck_assert(s21_is_not_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_not_equal_inf) {
    s21_decimal a, b;
    a = init_decimal(s21_INFINITY);
    b = init_decimal(s21_NEGATIVE_INFINITY);
    ck_assert(s21_is_not_equal(a, b) == OPR_SUCCESS);
    a = init_decimal(s21_NEGATIVE_INFINITY);
    s21_from_int_to_decimal(54, &b);
    ck_assert(s21_is_not_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_not_equal_inf_equal) {
    s21_decimal a, b;
    a = init_decimal(s21_INFINITY);
    b = init_decimal(s21_INFINITY);
    ck_assert(s21_is_not_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_not_equal_error) {
    s21_decimal a, b;
    a = init_decimal(s21_NAN);
    s21_from_int_to_decimal(21, &b);
    ck_assert(s21_is_not_equal(a, b) == OPR_ERROR);
    b = init_decimal(s21_INFINITY);
    ck_assert(s21_is_not_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_not_equal_limits_max) {
    s21_decimal a = init_decimal(s21_NORMAL_VALUE);
    s21_decimal b = get_max_decimal();
    ck_assert(s21_is_not_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_not_equal_limits_corners) {
    s21_decimal a = get_max_decimal();
    s21_decimal b = get_max_decimal();
    set_bit(&a, 1, SIGN_BIT);
    ck_assert(s21_is_not_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_not_equal_limits_max_float) {
    s21_decimal a = init_decimal(s21_NORMAL_VALUE);
    s21_decimal b = get_max_decimal();
    set_exp(&b, 28);
    ck_assert(s21_is_not_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_normal_random) {
    s21_decimal a, b;
    int i1 = -5000, i2 = 5000;
    unsigned int seed = time(NULL);
    for (size_t i = 0; i < 20; i++) {
        i1 += rand_r(&seed) % 1000;
        i2 -= rand_r(&seed) % 1000;
        s21_from_int_to_decimal(i1, &a);
        s21_from_int_to_decimal(i2, &b);
        if (i1 < i2)
            ck_assert(s21_is_less(a, b) == OPR_SUCCESS);
        else
            ck_assert(s21_is_less(a, b) == OPR_ERROR);
    }
}
END_TEST

START_TEST(s21_is_less_normal_float_random) {
    s21_decimal a, b;
    float i1 = -200, i2 = 200;
    unsigned int seed = time(NULL);
    for (size_t i = 0; i < 20; i++) {
        i1 += (float)((rand_r(&seed) % 100) + 1) / (float)((rand_r(&seed) % 10) + 1);
        i2 -= (float)((rand_r(&seed) % 100) + 1) / (float)((rand_r(&seed) % 10) + 1);
        s21_from_float_to_decimal(i1, &a);
        s21_from_float_to_decimal(i2, &b);
        if (i1 < i2)
            ck_assert(s21_is_less(a, b) == OPR_SUCCESS);
        else
            ck_assert(s21_is_less(a, b) == OPR_ERROR);
    }
}
END_TEST

START_TEST(s21_is_less_negative_positive) {
    s21_decimal a, b;
    s21_from_int_to_decimal(-24, &a);
    s21_from_int_to_decimal(21, &b);
    ck_assert(s21_is_less(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_normal_not_less_positive) {
    s21_decimal a, b;
    s21_from_int_to_decimal(21, &a);
    s21_from_int_to_decimal(20, &b);
    ck_assert(s21_is_less(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_less_normal_not_less_negative) {
    s21_decimal a, b;
    s21_from_int_to_decimal(-20, &a);
    s21_from_int_to_decimal(-21, &b);
    ck_assert(s21_is_less(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_less_normal_equal) {
    s21_decimal a, b;
    s21_from_int_to_decimal(21, &a);
    s21_from_int_to_decimal(21, &b);
    ck_assert(s21_is_less(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_less_float_not_less_positive) {
    s21_decimal a, b;
    s21_from_float_to_decimal(24.123400001, &a);
    s21_from_float_to_decimal(24.1234, &b);
    ck_assert(s21_is_less(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_less_float_not_less_negative) {
    s21_decimal a, b;
    s21_from_float_to_decimal(-24.1234, &a);
    s21_from_float_to_decimal(-24.123400001, &b);
    ck_assert(s21_is_less(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_less_float_not_less_1) {
    s21_decimal a, b;
    s21_from_float_to_decimal(21.1234, &a);
    s21_from_float_to_decimal(20.1234, &b);
    ck_assert(s21_is_less(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_less_inf) {
    s21_decimal a, b;
    a = init_decimal(s21_NEGATIVE_INFINITY);
    b = init_decimal(s21_INFINITY);
    ck_assert(s21_is_less(a, b) == OPR_SUCCESS);
    a = init_decimal(s21_NEGATIVE_INFINITY);
    s21_from_int_to_decimal(0, &b);
    ck_assert(s21_is_less(a, b) == OPR_SUCCESS);
    s21_from_int_to_decimal(0, &a);
    b = init_decimal(s21_INFINITY);
    ck_assert(s21_is_less(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_inf_not_less) {
    s21_decimal a, b;
    a = init_decimal(s21_INFINITY);
    b = init_decimal(s21_NEGATIVE_INFINITY);
    ck_assert(s21_is_less(a, b) == OPR_ERROR);
    s21_from_int_to_decimal(0, &b);
    ck_assert(s21_is_less(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_less_error) {
    s21_decimal a, b;
    a = init_decimal(s21_NAN);
    s21_from_int_to_decimal(21, &b);
    ck_assert(s21_is_less(a, b) == OPR_ERROR);
    b = init_decimal(s21_INFINITY);
    ck_assert(s21_is_less(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_less_limits) {
    s21_decimal a;
    s21_decimal b = get_max_decimal();
    s21_from_int_to_decimal(0, &a);
    ck_assert(s21_is_less(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_limits_exp_positive) {
    s21_decimal a = get_max_decimal();
    s21_decimal b = get_max_decimal();
    set_exp(&a, 1);
    b.bits[DEC_LOW] -= 5;
    ck_assert(s21_is_less(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_limits_exp_negative) {
    s21_decimal a = get_max_decimal();
    s21_decimal b = get_max_decimal();
    a.bits[DEC_LOW] -= 5;
    set_exp(&b, 1);
    set_bit(&a, 1, SIGN_BIT);
    set_bit(&b, 1, SIGN_BIT);
    ck_assert(s21_is_less(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_limits_corner) {
    s21_decimal a = get_max_decimal();
    s21_decimal b = get_max_decimal();
    set_bit(&a, 1, SIGN_BIT);
    ck_assert(s21_is_less(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_limits_zero_float) {
    s21_decimal a = init_decimal(s21_NORMAL_VALUE);
    s21_decimal b;
    set_exp(&a, 28);
    s21_from_int_to_decimal(1, &b);
    ck_assert(s21_is_less(a, b) == OPR_SUCCESS);
    s21_from_int_to_decimal(-1, &b);
    ck_assert(s21_is_less(a, b) == OPR_ERROR);
    s21_from_int_to_decimal(0, &b);
    ck_assert(s21_is_less(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_less_limits_max_float_positive) {
    s21_decimal a;
    s21_decimal b = get_max_decimal();
    s21_from_int_to_decimal(0, &a);
    set_exp(&b, 28);
    ck_assert(s21_is_less(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_limits_max_float_negative) {
    s21_decimal a = get_max_decimal();
    s21_decimal b;
    s21_from_int_to_decimal(0, &b);
    set_exp(&a, 28);
    set_bit(&a, 1, SIGN_BIT);
    ck_assert(s21_is_less(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_normal) {
    s21_decimal a, b;
    int i1 = -5000, i2 = 5000;
    unsigned int seed = time(NULL);
    for (size_t i = 0; i < 20; i++) {
        i1 += rand_r(&seed) % 1000;
        i2 -= rand_r(&seed) % 1000;
        s21_from_int_to_decimal(i1, &a);
        s21_from_int_to_decimal(i2, &b);
        if (i1 > i2)
            ck_assert(s21_is_greater(a, b) == OPR_SUCCESS);
        else
            ck_assert(s21_is_greater(a, b) == OPR_ERROR);         
    }
}
END_TEST

START_TEST(s21_is_greater_normal_float) {
    s21_decimal a, b;
    float i1 = -200, i2 = 200;
    unsigned int seed = time(NULL);
    for (size_t i = 0; i < 20; i++) {
        i1 += (float)((rand_r(&seed) % 100) + 1) / (float)((rand_r(&seed) % 10) + 1);
        i2 -= (float)((rand_r(&seed) % 100) + 1) / (float)((rand_r(&seed) % 10) + 1);
        s21_from_float_to_decimal(i1, &a);
        s21_from_float_to_decimal(i2, &b);
        if (i1 > i2)
            ck_assert(s21_is_greater(a, b) == OPR_SUCCESS);
        else
            ck_assert(s21_is_greater(a, b) == OPR_ERROR);         
    }
}
END_TEST

START_TEST(s21_is_greater_positive_negative) {
    s21_decimal a, b;
    s21_from_int_to_decimal(24, &a);
    s21_from_int_to_decimal(-25, &b);
    ck_assert(s21_is_greater(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_normal_not_greater_positive) {
    s21_decimal a, b;
    s21_from_int_to_decimal(20, &a);
    s21_from_int_to_decimal(21, &b);
    ck_assert(s21_is_greater(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_greater_normal_not_greater_negative) {
    s21_decimal a, b;
    s21_from_int_to_decimal(-21, &a);
    s21_from_int_to_decimal(-20, &b);
    ck_assert(s21_is_greater(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_greater_normal_equal_positive) {
    s21_decimal a, b;
    s21_from_int_to_decimal(21, &a);
    s21_from_int_to_decimal(21, &b);
    ck_assert(s21_is_greater(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_greater_normal_equal_negative) {
    s21_decimal a, b;
    s21_from_int_to_decimal(-21, &a);
    s21_from_int_to_decimal(-21, &b);
    ck_assert(s21_is_greater(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_greater_float_not_greater_positive) {
    s21_decimal a, b;
    s21_from_float_to_decimal(24.1234, &a);
    s21_from_float_to_decimal(24.123400001, &b);
    ck_assert(s21_is_greater(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_greater_float_not_greater_negative) {
    s21_decimal a, b;
    s21_from_float_to_decimal(24.1234, &a);
    s21_from_float_to_decimal(24.123400001, &b);
    ck_assert(s21_is_greater(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_greater_float_not_greater_1) {
    s21_decimal a, b;
    s21_from_float_to_decimal(20.1234, &a);
    s21_from_float_to_decimal(21.1234, &b);
    ck_assert(s21_is_greater(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_greater_inf) {
    s21_decimal a, b;
    a = init_decimal(s21_INFINITY);
    b = init_decimal(s21_NEGATIVE_INFINITY);
    ck_assert(s21_is_greater(a, b) == OPR_SUCCESS);
    a = init_decimal(s21_INFINITY);
    s21_from_int_to_decimal(0, &b);
    ck_assert(s21_is_greater(a, b) == OPR_SUCCESS);
    s21_from_int_to_decimal(0, &a);
    b = init_decimal(s21_NEGATIVE_INFINITY);
    ck_assert(s21_is_greater(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_inf_not_greater) {
    s21_decimal a, b;
    a = init_decimal(s21_NEGATIVE_INFINITY);
    b = init_decimal(s21_INFINITY);
    ck_assert(s21_is_greater(a, b) == OPR_ERROR);
    s21_from_int_to_decimal(0, &b);
    ck_assert(s21_is_greater(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_greater_error) {
    s21_decimal a, b;
    a = init_decimal(s21_NAN);
    s21_from_int_to_decimal(21, &b);
    ck_assert(s21_is_greater(a, b) == OPR_ERROR);
    b = init_decimal(s21_INFINITY);
    ck_assert(s21_is_greater(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_greater_limits_positive) {
    s21_decimal a = get_max_decimal();
    s21_decimal b;
    s21_from_int_to_decimal(0, &b);
    ck_assert(s21_is_greater(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_limits_negative) {
    s21_decimal a;
    s21_decimal b = get_max_decimal();
    s21_from_int_to_decimal(0, &a);
    set_bit(&b, 1, SIGN_BIT);
    ck_assert(s21_is_greater(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_limits_corner) {
    s21_decimal a = get_max_decimal();
    s21_decimal b = get_max_decimal();
    set_bit(&b, 1, SIGN_BIT);
    ck_assert(s21_is_greater(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_limits_zero_float) {
    s21_decimal a;
    s21_decimal b = init_decimal(s21_NORMAL_VALUE);
    s21_from_int_to_decimal(1, &a);
    set_exp(&b, 28);
    ck_assert(s21_is_greater(a, b) == OPR_SUCCESS);
    s21_from_int_to_decimal(0, &a);
    ck_assert(s21_is_greater(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_greater_limits_max_float_positive) {
    s21_decimal a = get_max_decimal();
    s21_decimal b;
    set_exp(&a, 28);
    s21_from_int_to_decimal(0, &b);
    ck_assert(s21_is_greater(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_limits_max_float_negative) {
    s21_decimal a;
    s21_decimal b = get_max_decimal();
    set_exp(&b, 28);
    s21_from_int_to_decimal(0, &a);
    set_bit(&b, 1, SIGN_BIT);
    ck_assert(s21_is_greater(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_or_equal_normal_less_positive) {
    s21_decimal a, b;
    s21_from_int_to_decimal(20, &a);
    s21_from_int_to_decimal(21, &b);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_or_equal_normal_less_negative) {
    s21_decimal a, b;
    s21_from_int_to_decimal(-22, &a);
    s21_from_int_to_decimal(-21, &b);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_or_equal_normal_less_negative_positive) {
    s21_decimal a, b;
    s21_from_int_to_decimal(-24, &a);
    s21_from_int_to_decimal(21, &b);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_or_equal_normal_not_less_positive) {
    s21_decimal a, b;
    s21_from_int_to_decimal(21, &a);
    s21_from_int_to_decimal(20, &b);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_less_or_equal_normal_not_less_negative) {
    s21_decimal a, b;
    s21_from_int_to_decimal(-20, &a);
    s21_from_int_to_decimal(-21, &b);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_less_or_equal_normal_equal_positive) {
    s21_decimal a, b;
    s21_from_int_to_decimal(21, &a);
    s21_from_int_to_decimal(21, &b);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_or_equal_normal_equal_negative) {
    s21_decimal a, b;
    s21_from_int_to_decimal(-21, &a);
    s21_from_int_to_decimal(-21, &b);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_or_equal_normal_float) {
    s21_decimal a, b;
    s21_from_float_to_decimal(21.1234, &a);
    s21_from_float_to_decimal(21.12345, &b);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_or_equal_float_not_less) {
    s21_decimal a, b;
    s21_from_float_to_decimal(24.12351, &a);
    s21_from_float_to_decimal(24.1235, &b);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_less_or_equal_float_not_less_1) {
    s21_decimal a, b;
    s21_from_float_to_decimal(21.1234, &a);
    s21_from_float_to_decimal(20.1234, &b);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_less_or_equal_float_equal) {
    s21_decimal a, b;
    s21_from_float_to_decimal(24.1234, &a);
    s21_from_float_to_decimal(24.1234, &b);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_or_equal_inf) {
    s21_decimal a, b;
    a = init_decimal(s21_NEGATIVE_INFINITY);
    b = init_decimal(s21_INFINITY);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
    a = init_decimal(s21_NEGATIVE_INFINITY);
    s21_from_int_to_decimal(0, &b);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
    s21_from_int_to_decimal(0, &a);
    b = init_decimal(s21_INFINITY);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
    a = init_decimal(s21_INFINITY);
    b = init_decimal(s21_INFINITY);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
    a = init_decimal(s21_NEGATIVE_INFINITY);
    b = init_decimal(s21_NEGATIVE_INFINITY);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_or_equal_inf_not_less) {
    s21_decimal a, b;
    a = init_decimal(s21_INFINITY);
    b = init_decimal(s21_NEGATIVE_INFINITY);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_ERROR);
    s21_from_int_to_decimal(0, &b);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_less_or_equal_error) {
    s21_decimal a, b;
    a = init_decimal(s21_NAN);
    s21_from_int_to_decimal(21, &b);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_ERROR);
    b = init_decimal(s21_INFINITY);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_less_or_equal_limits_positive) {
    s21_decimal a;
    s21_decimal b = get_max_decimal();
    s21_from_int_to_decimal(0, &a);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_or_equal_limits_negative) {
    s21_decimal a = get_max_decimal();
    s21_decimal b;
    s21_from_int_to_decimal(0, &b);
    set_bit(&a, 1, SIGN_BIT);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_or_equal_limits_corner) {
    s21_decimal a = get_max_decimal();
    s21_decimal b = get_max_decimal();
    set_bit(&a, 1, SIGN_BIT);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_or_equal_limits_equal_max) {
    s21_decimal a = get_max_decimal();
    s21_decimal b = get_max_decimal();
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_or_equal_limits_equal_min) {
    s21_decimal a = get_max_decimal();
    s21_decimal b = get_max_decimal();
    set_bit(&a, 1, SIGN_BIT);
    set_bit(&b, 1, SIGN_BIT);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_or_equal_limits_zero_float) {
    s21_decimal a = init_decimal(s21_NORMAL_VALUE);
    s21_decimal b;
    set_exp(&a, 28);
    s21_from_int_to_decimal(1, &b);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
    s21_from_int_to_decimal(0, &b);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_or_equal_limits_max_float) {
    s21_decimal a;
    s21_decimal b = get_max_decimal();
    s21_from_int_to_decimal(0, &a);
    set_exp(&b, 28);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_less_or_equal_limits_max_both_float) {
    s21_decimal a = get_max_decimal();
    s21_decimal b = get_max_decimal();
    set_exp(&a, 28);
    set_exp(&b, 28);
    ck_assert(s21_is_less_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_or_equal_normal_greater_positive) {
    s21_decimal a, b;
    s21_from_int_to_decimal(21, &a);
    s21_from_int_to_decimal(20, &b);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_or_equal_normal_greater_negative) {
    s21_decimal a, b;
    s21_from_int_to_decimal(-21, &a);
    s21_from_int_to_decimal(-22, &b);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_or_equal_normal_greater_positive_negative) {
    s21_decimal a, b;
    s21_from_int_to_decimal(21, &a);
    s21_from_int_to_decimal(-24, &b);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_or_equal_normal_not_greater_positive) {
    s21_decimal a, b;
    s21_from_int_to_decimal(20, &a);
    s21_from_int_to_decimal(21, &b);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_greater_or_equal_normal_not_greater_negative) {
    s21_decimal a, b;
    s21_from_int_to_decimal(-21, &a);
    s21_from_int_to_decimal(-20, &b);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_greater_or_equal_normal_equal) {
    s21_decimal a, b;
    s21_from_int_to_decimal(21, &a);
    s21_from_int_to_decimal(21, &b);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_or_equal_normal_equal_negative) {
    s21_decimal a, b;
    s21_from_int_to_decimal(-21, &a);
    s21_from_int_to_decimal(-21, &b);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_or_equal_normal_float) {
    s21_decimal a, b;
    s21_from_float_to_decimal(21.12345, &a);
    s21_from_float_to_decimal(21.1234, &b);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_or_equal_float_not_greater) {
    s21_decimal a, b;
    s21_from_float_to_decimal(24.1235, &a);
    s21_from_float_to_decimal(24.12351, &b);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_greater_or_equal_float_not_greater_1) {
    s21_decimal a, b;
    s21_from_float_to_decimal(20.1234, &a);
    s21_from_float_to_decimal(21.1234, &b);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_greater_or_equal_float_equal) {
    s21_decimal a, b;
    s21_from_float_to_decimal(24.1234, &a);
    s21_from_float_to_decimal(24.1234, &b);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_or_equal_inf) {
    s21_decimal a, b;
    a = init_decimal(s21_INFINITY);
    b = init_decimal(s21_NEGATIVE_INFINITY);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
    s21_from_int_to_decimal(0, &a);
    b = init_decimal(s21_NEGATIVE_INFINITY);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
    a = init_decimal(s21_INFINITY);
    s21_from_int_to_decimal(0, &b);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
    a = init_decimal(s21_INFINITY);
    b = init_decimal(s21_INFINITY);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
    a = init_decimal(s21_NEGATIVE_INFINITY);
    b = init_decimal(s21_NEGATIVE_INFINITY);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_or_equal_inf_not_greater) {
    s21_decimal a, b;
    a = init_decimal(s21_NEGATIVE_INFINITY);
    b = init_decimal(s21_INFINITY);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_ERROR);
    s21_from_int_to_decimal(0, &b);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_greater_or_equal_error) {
    s21_decimal a, b;
    s21_from_int_to_decimal(21, &a);
    b = init_decimal(s21_NAN);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_ERROR);
    a = init_decimal(s21_INFINITY);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_ERROR);
}
END_TEST

START_TEST(s21_is_greater_or_equal_limits_positive) {
    s21_decimal a = get_max_decimal();
    s21_decimal b;
    s21_from_int_to_decimal(0, &b);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_or_equal_limits_negative) {
    s21_decimal a;
    s21_decimal b = get_max_decimal();
    s21_from_int_to_decimal(0, &a);
    set_bit(&b, 1, SIGN_BIT);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_or_equal_limits_corner) {
    s21_decimal a = get_max_decimal();
    s21_decimal b = get_max_decimal();
    set_bit(&b, 1, SIGN_BIT);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_or_equal_limits_equal_max) {
    s21_decimal a = get_max_decimal();
    s21_decimal b = get_max_decimal();
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_or_equal_limits_equal_min) {
    s21_decimal a = get_max_decimal();
    s21_decimal b = get_max_decimal();
    set_bit(&a, 1, SIGN_BIT);
    set_bit(&b, 1, SIGN_BIT);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_or_equal_limits_zero_float) {
    s21_decimal a;
    s21_decimal b = init_decimal(s21_NORMAL_VALUE);
    s21_from_int_to_decimal(1, &a);
    set_exp(&b, 28);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
    s21_from_int_to_decimal(0, &a);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_or_equal_limits_max_float_positive) {
    s21_decimal a = get_max_decimal();
    s21_decimal b;
    set_exp(&a, 28);
    s21_from_int_to_decimal(0, &b);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_or_equal_limits_max_float_negative) {
    s21_decimal a;
    s21_decimal b = get_max_decimal();
    set_exp(&b, 28);
    s21_from_int_to_decimal(0, &a);
    set_bit(&b, 1, SIGN_BIT);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

START_TEST(s21_is_greater_or_equal_limits_max_both_float) {
    s21_decimal a = get_max_decimal();
    s21_decimal b = get_max_decimal();
    set_exp(&a, 28);
    set_exp(&b, 28);
    ck_assert(s21_is_greater_or_equal(a, b) == OPR_SUCCESS);
}
END_TEST

// Функция создания набора тестов.
Suite *comparison_suite(void) {
    Suite *suite = suite_create("Comparisons");
    // Набор разбивается на группы тестов, разделённых по каким-либо критериям.
    TCase *IE = tcase_create("s21_is_equal");
    tcase_add_test(IE, s21_is_equal_normal);
    tcase_add_test(IE, s21_is_equal_zero);
    tcase_add_test(IE, s21_is_equal_normal_not_equal);
    tcase_add_test(IE, s21_is_equal_normal_float);
    tcase_add_test(IE, s21_is_equal_limits_infinitesimal);
    tcase_add_test(IE, s21_is_equal_float_not_equal);
    tcase_add_test(IE, s21_is_equal_float_not_equal_1);
    tcase_add_test(IE, s21_is_equal_inf);
    tcase_add_test(IE, s21_is_equal_inf_not_equal);
    tcase_add_test(IE, s21_is_equal_error);
    tcase_add_test(IE, s21_is_equal_limits_zero);
    tcase_add_test(IE, s21_is_equal_limits_zero_float);
    tcase_add_test(IE, s21_is_equal_limits_max);
    tcase_add_test(IE, s21_is_equal_limits_max_float);
    TCase *INE = tcase_create("s21_is_not_equal");
    tcase_add_test(INE, s21_is_not_equal_normal);
    tcase_add_test(INE, s21_is_not_equal_normal_opposite);
    tcase_add_test(INE, s21_is_not_equal_normal_equal);
    tcase_add_test(INE, s21_is_not_equal_normal_float);
    tcase_add_test(INE, s21_is_not_equal_float_equal);
    tcase_add_test(INE, s21_is_not_equal_inf);
    tcase_add_test(INE, s21_is_not_equal_inf_equal);
    tcase_add_test(INE, s21_is_not_equal_error);
    tcase_add_test(INE, s21_is_not_equal_limits_max);
    tcase_add_test(INE, s21_is_not_equal_limits_corners);
    tcase_add_test(INE, s21_is_not_equal_limits_max_float);
    TCase *IL = tcase_create("s21_is_less");
    tcase_add_test(IL, s21_is_less_normal_random);
    tcase_add_test(IL, s21_is_less_normal_float_random);
    tcase_add_test(IL, s21_is_less_negative_positive);
    tcase_add_test(IL, s21_is_less_normal_equal);
    tcase_add_test(IL, s21_is_less_normal_not_less_positive);
    tcase_add_test(IL, s21_is_less_normal_not_less_negative);
    tcase_add_test(IL, s21_is_less_float_not_less_positive);
    tcase_add_test(IL, s21_is_less_float_not_less_negative);
    tcase_add_test(IL, s21_is_less_float_not_less_1);
    tcase_add_test(IL, s21_is_less_inf);
    tcase_add_test(IL, s21_is_less_inf_not_less);
    tcase_add_test(IL, s21_is_less_error);
    tcase_add_test(IL, s21_is_less_limits);
    tcase_add_test(IL, s21_is_less_limits_exp_positive);
    tcase_add_test(IL, s21_is_less_limits_exp_negative);
    tcase_add_test(IL, s21_is_less_limits_corner);
    tcase_add_test(IL, s21_is_less_limits_zero_float);
    tcase_add_test(IL, s21_is_less_limits_max_float_positive);
    tcase_add_test(IL, s21_is_less_limits_max_float_negative);
    TCase *IG = tcase_create("s21_is_greater");
    tcase_add_test(IG, s21_is_greater_normal);
    tcase_add_test(IG, s21_is_greater_normal_float);
    tcase_add_test(IG, s21_is_greater_positive_negative);
    tcase_add_test(IG, s21_is_greater_normal_not_greater_positive);
    tcase_add_test(IG, s21_is_greater_normal_not_greater_negative);
    tcase_add_test(IG, s21_is_greater_normal_equal_positive);
    tcase_add_test(IG, s21_is_greater_normal_equal_negative);
    tcase_add_test(IG, s21_is_greater_float_not_greater_positive);
    tcase_add_test(IG, s21_is_greater_float_not_greater_negative);
    tcase_add_test(IG, s21_is_greater_float_not_greater_1);
    tcase_add_test(IG, s21_is_greater_inf);
    tcase_add_test(IG, s21_is_greater_inf_not_greater);
    tcase_add_test(IG, s21_is_greater_error);
    tcase_add_test(IG, s21_is_greater_limits_positive);
    tcase_add_test(IG, s21_is_greater_limits_negative);
    tcase_add_test(IG, s21_is_greater_limits_corner);
    tcase_add_test(IG, s21_is_greater_limits_zero_float);
    tcase_add_test(IG, s21_is_greater_limits_max_float_positive);
    tcase_add_test(IG, s21_is_greater_limits_max_float_negative);
    TCase *ILE = tcase_create("s21_is_less_or_equal");
    tcase_add_test(ILE, s21_is_less_or_equal_normal_less_positive);
    tcase_add_test(ILE, s21_is_less_or_equal_normal_less_negative);
    tcase_add_test(ILE, s21_is_less_or_equal_normal_less_negative_positive);
    tcase_add_test(ILE, s21_is_less_or_equal_normal_equal_positive);
    tcase_add_test(ILE, s21_is_less_or_equal_normal_equal_negative);
    tcase_add_test(ILE, s21_is_less_or_equal_normal_not_less_positive);
    tcase_add_test(ILE, s21_is_less_or_equal_normal_not_less_negative);
    tcase_add_test(ILE, s21_is_less_or_equal_normal_float);
    tcase_add_test(ILE, s21_is_less_or_equal_float_not_less_1);
    tcase_add_test(ILE, s21_is_less_or_equal_float_not_less);
    tcase_add_test(ILE, s21_is_less_or_equal_float_equal);
    tcase_add_test(ILE, s21_is_less_or_equal_inf);
    tcase_add_test(ILE, s21_is_less_or_equal_inf_not_less);
    tcase_add_test(ILE, s21_is_less_or_equal_error);
    tcase_add_test(ILE, s21_is_less_or_equal_limits_positive);
    tcase_add_test(ILE, s21_is_less_or_equal_limits_negative);
    tcase_add_test(ILE, s21_is_less_or_equal_limits_equal_min);
    tcase_add_test(ILE, s21_is_less_or_equal_limits_equal_max);
    tcase_add_test(ILE, s21_is_less_or_equal_limits_corner);
    tcase_add_test(ILE, s21_is_less_or_equal_limits_zero_float);
    tcase_add_test(ILE, s21_is_less_or_equal_limits_max_float);
    tcase_add_test(ILE, s21_is_less_or_equal_limits_max_both_float);
    TCase *IGE = tcase_create("s21_is_greater_or_equal");
    tcase_add_test(IGE, s21_is_greater_or_equal_normal_greater_positive);
    tcase_add_test(IGE, s21_is_greater_or_equal_normal_greater_negative);
    tcase_add_test(IGE, s21_is_greater_or_equal_normal_greater_positive_negative);
    tcase_add_test(IGE, s21_is_greater_or_equal_normal_equal);
    tcase_add_test(IGE, s21_is_greater_or_equal_normal_equal_negative);
    tcase_add_test(IGE, s21_is_greater_or_equal_normal_not_greater_positive);
    tcase_add_test(IGE, s21_is_greater_or_equal_normal_not_greater_negative);
    tcase_add_test(IGE, s21_is_greater_or_equal_normal_float);
    tcase_add_test(IGE, s21_is_greater_or_equal_float_not_greater_1);
    tcase_add_test(IGE, s21_is_greater_or_equal_float_not_greater);
    tcase_add_test(IGE, s21_is_greater_or_equal_float_equal);
    tcase_add_test(IGE, s21_is_greater_or_equal_inf);
    tcase_add_test(IGE, s21_is_greater_or_equal_inf_not_greater);
    tcase_add_test(IGE, s21_is_greater_or_equal_error);
    tcase_add_test(IGE, s21_is_greater_or_equal_limits_positive);
    tcase_add_test(IGE, s21_is_greater_or_equal_limits_negative);
    tcase_add_test(IGE, s21_is_greater_or_equal_limits_equal_min);
    tcase_add_test(IGE, s21_is_greater_or_equal_limits_equal_max);
    tcase_add_test(IGE, s21_is_greater_or_equal_limits_corner);
    tcase_add_test(IGE, s21_is_greater_or_equal_limits_zero_float);
    tcase_add_test(IGE, s21_is_greater_or_equal_limits_max_float_positive);
    tcase_add_test(IGE, s21_is_greater_or_equal_limits_max_float_negative);
    tcase_add_test(IGE, s21_is_greater_or_equal_limits_max_both_float);

    // Добавление теста в тестовый набор.
    suite_add_tcase(suite, IE);
    suite_add_tcase(suite, INE);
    suite_add_tcase(suite, IL);
    suite_add_tcase(suite, IG);
    suite_add_tcase(suite, ILE);
    suite_add_tcase(suite, IGE);
    return suite;
}
