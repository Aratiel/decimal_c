// Copyright 2022 <lwolmer, tfelton, stogg, jgerrick>
#include <time.h>
#include "test.h"

START_TEST(s21_from_int_to_decimal_and_backward_random) {
    s21_decimal dec;
    int to_dec = 200000;
    unsigned int seed = time(NULL);
    for (size_t i = 0; i < 100; i++) {
        to_dec -= rand_r(&seed) % 10000;
        int conv_stat1 = s21_from_int_to_decimal(to_dec, &dec);
        ck_assert_int_eq(conv_stat1, OPR_SUCCESS);
        int from_dec = 0;
        int conv_stat2 = s21_from_decimal_to_int(dec, &from_dec);
        ck_assert_int_eq(conv_stat2, OPR_SUCCESS);
        ck_assert_int_eq(to_dec, from_dec);
    }
}
END_TEST

START_TEST(s21_from_int_to_decimal_zero) {
    s21_decimal dec;
    int integer = 0;
    int success = s21_from_int_to_decimal(integer, &dec);
    ck_assert_int_eq(success, OPR_SUCCESS);
    int cmp_integer = 0;
    s21_from_decimal_to_int(dec, &cmp_integer);
    ck_assert_int_eq(integer, cmp_integer);
}
END_TEST

START_TEST(s21_from_int_to_decimal_limits) {
    s21_decimal dec;
    int integer = INT_MAX;
    int success = s21_from_int_to_decimal(integer, &dec);
    ck_assert_int_eq(success, OPR_SUCCESS);
    int cmp_integer = 0;
    s21_from_decimal_to_int(dec, &cmp_integer);
    ck_assert_int_eq(integer, cmp_integer);
    integer = INT_MIN;
    success = s21_from_int_to_decimal(integer, &dec);
    ck_assert_int_eq(success, OPR_SUCCESS);
    cmp_integer = 0;
    s21_from_decimal_to_int(dec, &cmp_integer);
    ck_assert_int_eq(integer, cmp_integer);
}
END_TEST

START_TEST(s21_from_int_to_decimal_failed) {
    s21_decimal *dec = NULL;
    int integer = 0;
    int success = s21_from_int_to_decimal(integer, dec);
    ck_assert_int_eq(success, OPR_ERROR);
}
END_TEST

START_TEST(s21_from_float_to_decimal_default) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    float f = -1.23;
    int success = s21_from_float_to_decimal(f, &dec);
    ck_assert_int_eq(success, OPR_SUCCESS);
    float res;
    s21_from_decimal_to_float(dec, &res);
    f = -1.1234567;
    success = s21_from_float_to_decimal(f, &dec);
    ck_assert_int_eq(success, OPR_SUCCESS);
    s21_from_decimal_to_float(dec, &res);
    ck_assert_float_eq(f, res);
    f = 0;
    success = s21_from_float_to_decimal(f, &dec);
    ck_assert_int_eq(success, OPR_SUCCESS);
    s21_from_decimal_to_float(dec, &res);
    ck_assert_float_eq(f, res);
    f = 15123.45;
    success = s21_from_float_to_decimal(f, &dec);
    ck_assert_int_eq(success, OPR_SUCCESS);
    s21_from_decimal_to_float(dec, &res);
    ck_assert_float_eq(f, res);
}
END_TEST

START_TEST(s21_from_float_to_decimal_inf) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    float f = INFINITY;
    int success = s21_from_float_to_decimal(f, &dec);
    ck_assert_int_eq(success, OPR_SUCCESS);
    ck_assert(is_zeroed(dec));
    ck_assert_int_eq(dec.value_type, s21_INFINITY);
    f = -INFINITY;
    success = s21_from_float_to_decimal(f, &dec);
    ck_assert_int_eq(success, OPR_SUCCESS);
    ck_assert(is_zeroed(dec));
    ck_assert_int_eq(dec.value_type, s21_NEGATIVE_INFINITY);
    f = NAN;
    success = s21_from_float_to_decimal(f, &dec);
    ck_assert_int_eq(success, OPR_SUCCESS);
    ck_assert(is_zeroed(dec));
    ck_assert_int_eq(dec.value_type, s21_NAN);
}

START_TEST(s21_from_decimal_to_float_default) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    float f1 = -12.3;
    s21_from_float_to_decimal(f1, &dec);
    float f2;
    int success = s21_from_decimal_to_float(dec, &f2);
    ck_assert_int_eq(success, OPR_SUCCESS);
    ck_assert_float_eq(f1, f2);
}
END_TEST

START_TEST(s21_from_decimal_to_float_limits) {
    float f;
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    ck_assert_int_eq(s21_from_decimal_to_float(dec, &f), OPR_SUCCESS);
    ck_assert_float_eq(f, 0.0f);
    dec = get_max_decimal();
    ck_assert_int_eq(s21_from_decimal_to_float(dec, &f), OPR_SUCCESS);
    ck_assert_float_eq(f, 79228162514264337593543950335.0f);
    set_bit(&dec, 1, SIGN_BIT);
    set_exp(&dec, 28);
    ck_assert_int_eq(s21_from_decimal_to_float(dec, &f), OPR_SUCCESS);
    ck_assert_float_eq(f, -7.9228162514264337593543950335f);
}
END_TEST

START_TEST(s21_from_decimal_to_float_nan) {
    s21_decimal dec = init_decimal(s21_NAN);
    float f;
    int success = s21_from_decimal_to_float(dec, &f);
    ck_assert_int_eq(success, OPR_SUCCESS);
    ck_assert(isnan(f));
    dec = init_decimal(s21_NEGATIVE_INFINITY);
    ck_assert_int_eq(s21_from_decimal_to_float(dec, &f), OPR_SUCCESS);
    ck_assert(isinf(f));
    dec = init_decimal(s21_INFINITY);
    ck_assert_int_eq(s21_from_decimal_to_float(dec, &f), OPR_SUCCESS);
    ck_assert(isinf(f));
}
END_TEST

START_TEST(s21_from_decimal_to_int_normal_zero) {
    s21_decimal dec;
    s21_from_int_to_decimal(0, &dec);
    int integer = 0;
    int success = s21_from_decimal_to_int(dec, &integer);
    ck_assert_int_eq(success, OPR_SUCCESS);
    ck_assert_int_eq(integer, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_int_float) {
    s21_decimal dec = get_max_decimal();
    set_exp(&dec, 28);
    set_bit(&dec, 1, SIGN_BIT);
    int integer = 0;
    int success = s21_from_decimal_to_int(dec, &integer);
    ck_assert_int_eq(success, OPR_SUCCESS);
    ck_assert_int_eq(integer, -7);
}
END_TEST

START_TEST(s21_from_decimal_to_int_limits) {
    s21_decimal dec;
    s21_from_int_to_decimal(INT_MAX, &dec);
    int integer = 0;
    int success = s21_from_decimal_to_int(dec, &integer);
    ck_assert_int_eq(success, OPR_SUCCESS);
    ck_assert_int_eq(integer, INT_MAX);
}
END_TEST

START_TEST(s21_from_decimal_to_int_limits_negative) {
    s21_decimal dec;
    s21_from_int_to_decimal(INT_MIN, &dec);
    int integer = 0;
    int success = s21_from_decimal_to_int(dec, &integer);
    ck_assert_int_eq(success, OPR_SUCCESS);
    ck_assert_int_eq(integer, INT_MIN);
}
END_TEST

START_TEST(s21_from_decimal_to_int_failed) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    dec.bits[DEC_LOW] = UINT_MAX;
    int integer = 0;
    int success = s21_from_decimal_to_int(dec, &integer);
    ck_assert_int_eq(success, OPR_ERROR);
}
END_TEST

START_TEST(s21_from_decimal_to_int_failed1) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    dec.bits[DEC_LOW] = INT_MAX + 1U;
    int integer = 0;
    int success = s21_from_decimal_to_int(dec, &integer);
    ck_assert_int_eq(success, OPR_ERROR);
}
END_TEST

START_TEST(s21_from_decimal_to_int_failed_NAN) {
    s21_decimal dec = init_decimal(s21_NAN);
    int integer = 0;
    int success = s21_from_decimal_to_int(dec, &integer);
    ck_assert_int_eq(success, OPR_ERROR);
}
END_TEST

START_TEST(s21_from_decimal_to_int_failed_INF) {
    s21_decimal dec = init_decimal(s21_INFINITY);
    int integer = 0;
    int success = s21_from_decimal_to_int(dec, &integer);
    ck_assert_int_eq(success, OPR_ERROR);
}
END_TEST

START_TEST(s21_from_decimal_to_int_failed_NINF) {
    s21_decimal dec = init_decimal(s21_NEGATIVE_INFINITY);
    int integer = 0;
    int success = s21_from_decimal_to_int(dec, &integer);
    ck_assert_int_eq(success, OPR_ERROR);
}
END_TEST

START_TEST(s21_from_decimal_to_int_failed_NULL) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    int success = s21_from_decimal_to_int(dec, NULL);
    ck_assert_int_eq(success, OPR_ERROR);
}
END_TEST

// Функция создания набора тестов.
Suite *convertion_suite(void) {
    Suite *suite = suite_create("\n Convertions");
    // Набор разбивается на группы тестов, разделённых по каким-либо критериям.
    TCase *ITD = tcase_create("Int to decimal");
    tcase_add_test(ITD, s21_from_int_to_decimal_and_backward_random);
    tcase_add_test(ITD, s21_from_int_to_decimal_zero);
    tcase_add_test(ITD, s21_from_int_to_decimal_limits);
    tcase_add_test(ITD, s21_from_int_to_decimal_failed);

    TCase *FTD = tcase_create("Float to decimal");
    tcase_add_test(FTD, s21_from_float_to_decimal_default);
    tcase_add_test(FTD, s21_from_float_to_decimal_inf);

    TCase *DTF = tcase_create("Decimal to float");
    tcase_add_test(DTF, s21_from_decimal_to_float_default);
    tcase_add_test(DTF, s21_from_decimal_to_float_limits);
    tcase_add_test(DTF, s21_from_decimal_to_float_nan);

    TCase *DTI = tcase_create("Decimal to int");
    tcase_add_test(ITD, s21_from_decimal_to_int_normal_zero);
    tcase_add_test(ITD, s21_from_decimal_to_int_float);
    tcase_add_test(ITD, s21_from_decimal_to_int_failed);
    tcase_add_test(ITD, s21_from_decimal_to_int_failed1);
    tcase_add_test(ITD, s21_from_decimal_to_int_failed_NAN);
    tcase_add_test(ITD, s21_from_decimal_to_int_failed_INF);
    tcase_add_test(ITD, s21_from_decimal_to_int_failed_NINF);
    tcase_add_test(ITD, s21_from_decimal_to_int_failed_NULL);
    tcase_add_test(ITD, s21_from_decimal_to_int_limits);
    tcase_add_test(ITD, s21_from_decimal_to_int_limits_negative);

    // Добавление теста в тестовый набор.
    suite_add_tcase(suite, ITD);
    suite_add_tcase(suite, FTD);
    suite_add_tcase(suite, DTF);
    suite_add_tcase(suite, DTI);
    return suite;
}
