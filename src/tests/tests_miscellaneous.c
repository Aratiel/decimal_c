// Copyright 2022 <lwolmer, tfelton, stogg, jgerrick>
#include <limits.h>
#include <time.h>
#include "test.h"
#include "utilities.h"

START_TEST(s21_truncate_normal) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    dec.bits[DEC_LOW] = 123456;
    set_exp(&dec, 3);
    dec = s21_truncate(dec);
    ck_assert(cmp_decimal_str(dec, "123"));
}
END_TEST

START_TEST(s21_truncate_max) {
    s21_decimal dec = get_max_decimal();
    set_exp(&dec, 28);
    dec = s21_truncate(dec);
    ck_assert(cmp_decimal_str(dec, "7"));
}
END_TEST

START_TEST(s21_truncate_negmax) {
    s21_decimal dec = get_max_decimal();
    set_exp(&dec, 20);
    set_bit(&dec, 1, SIGN_BIT);
    dec = s21_truncate(dec);
    ck_assert(cmp_decimal_str(dec, "-792281625"));
}
END_TEST

START_TEST(s21_truncate_def1) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    dec.bits[DEC_LOW] = 1;
    dec.bits[DEC_MID] = 1;
    dec.bits[DEC_HIGH] = 1;
    set_exp(&dec, 17);
    dec = s21_truncate(dec);
    ck_assert(cmp_decimal_str(dec, "184"));
}
END_TEST

START_TEST(s21_truncate_zero) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    dec = s21_truncate(dec);
    ck_assert(cmp_decimal_str(dec, "0"));
}
END_TEST

START_TEST(s21_truncate_nan) {
    s21_decimal dec = init_decimal(s21_NAN);
    dec = s21_truncate(dec);
    ck_assert(cmp_decimal_str(dec, "nan"));
}
END_TEST

START_TEST(s21_truncate_inf) {
    s21_decimal dec = init_decimal(s21_INFINITY);
    dec = s21_truncate(dec);
    ck_assert(cmp_decimal_str(dec, "inf"));
}
END_TEST

START_TEST(s21_truncate_neginf) {
    s21_decimal dec = init_decimal(s21_NEGATIVE_INFINITY);
    dec = s21_truncate(dec);
    ck_assert(cmp_decimal_str(dec, "-inf"));
}
END_TEST

START_TEST(s21_truncate_def2) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    dec.bits[DEC_LOW] = 1234;
    dec.bits[DEC_MID] = 4643;
    dec.bits[DEC_HIGH] = 83672;
    set_exp(&dec, 20);
    set_bit(&dec, 1, SIGN_BIT);
    dec = s21_truncate(dec);
    ck_assert(cmp_decimal_str(dec, "-15434"));
}

START_TEST(s21_truncate_def3) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    s21_from_int_to_decimal(1124, &dec);
    set_exp(&dec, 20);
    dec = s21_truncate(dec);
    ck_assert(cmp_decimal_str(dec, "0"));
}
END_TEST

START_TEST(s21_truncate_def4) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    s21_from_int_to_decimal(1234, &dec);
    dec = s21_truncate(dec);
    ck_assert(cmp_decimal_str(dec, "1234"));
}
END_TEST

START_TEST(s21_floor_normal) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    dec.bits[DEC_LOW] = 123456;
    set_bit(&dec, 1, SIGN_BIT);
    set_exp(&dec, 2);
    dec = s21_round(dec);
    ck_assert(cmp_decimal_str(dec, "-1235"));
}
END_TEST

START_TEST(s21_floor_max) {
    s21_decimal dec = get_max_decimal();
    set_exp(&dec, 20);
    dec = s21_floor(dec);
    ck_assert(cmp_decimal_str(dec, "792281625"));
}
END_TEST

START_TEST(s21_floor_negmax) {
    s21_decimal dec = get_max_decimal();
    set_exp(&dec, 20);
    set_bit(&dec, 1, SIGN_BIT);
    dec = s21_floor(dec);
    ck_assert(cmp_decimal_str(dec, "-792281626"));
}
END_TEST

START_TEST(s21_floor_def1) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    dec.bits[DEC_LOW] = 1;
    dec.bits[DEC_MID] = 0;
    dec.bits[DEC_HIGH] = 100;
    set_exp(&dec, 5);
    set_bit(&dec, 1, SIGN_BIT);
    dec = s21_floor(dec);
    ck_assert(cmp_decimal_str(dec, "-18446744073709552"));
}
END_TEST

START_TEST(s21_floor_zero) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    dec = s21_floor(dec);
    ck_assert(cmp_decimal_str(dec, "0"));
}
END_TEST

START_TEST(s21_floor_nan) {
    s21_decimal dec  = init_decimal(s21_NAN);
    dec = s21_floor(dec);
     ck_assert(cmp_decimal_str(dec, "nan"));
}
END_TEST

START_TEST(s21_floor_inf) {
    s21_decimal dec = init_decimal(s21_INFINITY);
    dec = s21_floor(dec);
    ck_assert(cmp_decimal_str(dec, "inf"));
}
END_TEST

START_TEST(s21_floor_neginf) {
     s21_decimal dec = init_decimal(s21_NEGATIVE_INFINITY);
    dec = s21_floor(dec);
    ck_assert(cmp_decimal_str(dec, "-inf"));
}
END_TEST

START_TEST(s21_floor_def2) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    dec.bits[DEC_LOW] = 1234;
    dec.bits[DEC_MID] = 4643;
    dec.bits[DEC_HIGH] = 83672;
    set_exp(&dec, 5);
    dec = s21_floor(dec);
    ck_assert(cmp_decimal_str(dec, "15434759701553671359"));
}
END_TEST

START_TEST(s21_floor_def3) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    s21_from_int_to_decimal(15, &dec);
    set_exp(&dec, 1);
    dec = s21_floor(dec);
    ck_assert(cmp_decimal_str(dec, "1"));
}
END_TEST

START_TEST(s21_floor_def4) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    s21_from_int_to_decimal(11, &dec);
    set_exp(&dec, 1);
    set_bit(&dec, 1, SIGN_BIT);
    dec = s21_floor(dec);
    ck_assert(cmp_decimal_str(dec, "-2"));
}
END_TEST

START_TEST(s21_negate_default) {
    s21_decimal dec;
    int num = 1234;
    int num2 = -1234;
    s21_from_int_to_decimal(num, &dec);
    dec = s21_negate(dec);
    s21_from_decimal_to_int(dec, &num);
    ck_assert_int_eq(num, num2);
}
END_TEST

START_TEST(s21_negate_float) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    dec.bits[DEC_LOW] = 1234;
    dec.bits[DEC_MISC] = 1 << DEC_MISC_EXP_START;
    dec = s21_negate(dec);
    ck_assert(get_bit(dec, SIGN_BIT) == 1);
}
END_TEST

START_TEST(s21_negate_inf) {
    s21_decimal dec = init_decimal(s21_INFINITY);
    dec = s21_negate(dec);
    ck_assert(dec.value_type == s21_NEGATIVE_INFINITY);
}
END_TEST

START_TEST(s21_negate_neginf) {
    s21_decimal dec = init_decimal(s21_NEGATIVE_INFINITY);
    dec = s21_negate(dec);
    ck_assert(dec.value_type == s21_INFINITY);
}
END_TEST

START_TEST(s21_negate_nan) {
    s21_decimal dec = init_decimal(s21_NAN);
    dec = s21_negate(dec);
    ck_assert(is_nan(dec));
}
END_TEST

START_TEST(s21_negate_max) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    dec.bits[DEC_LOW] = UINT_MAX;
    dec.bits[DEC_MID] = UINT_MAX;
    dec.bits[DEC_HIGH] = UINT_MAX;
    dec = s21_negate(dec);
    ck_assert(get_bit(dec, SIGN_BIT) == 1);
}
END_TEST

START_TEST(s21_round_normal) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    dec.bits[DEC_LOW] = 123456;
    set_exp(&dec, 2);
    dec = s21_round(dec);
    ck_assert(cmp_decimal_str(dec, "1235"));
}
END_TEST

START_TEST(s21_round_max) {
    s21_decimal dec = get_max_decimal();
    set_exp(&dec, 10);
    dec = s21_round(dec);
    ck_assert(cmp_decimal_str(dec, "7922816251426433759"));
}
END_TEST

START_TEST(s21_round_negmax) {
    s21_decimal dec = get_max_decimal();
    set_exp(&dec, 10);
    set_bit(&dec, 1, SIGN_BIT);
    dec = s21_round(dec);
    ck_assert(cmp_decimal_str(dec, "-7922816251426433759"));
}
END_TEST

START_TEST(s21_round_def1) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    dec.bits[DEC_LOW] = 1;
    dec.bits[DEC_MID] = 1;
    dec.bits[DEC_HIGH] = 1;
    set_exp(&dec, 2);
    dec = s21_round(dec);
    ck_assert(cmp_decimal_str(dec, "184467440780045189"));
}
END_TEST

START_TEST(s21_round_zero) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    dec = s21_round(dec);
    ck_assert(cmp_decimal_str(dec, "0"));
}
END_TEST

START_TEST(s21_round_nan) {
    s21_decimal dec = init_decimal(s21_NAN);
    dec = s21_round(dec);
    ck_assert(cmp_decimal_str(dec, "nan"));
}
END_TEST

START_TEST(s21_round_inf) {
    s21_decimal dec = init_decimal(s21_INFINITY);
    dec = s21_round(dec);
    ck_assert(cmp_decimal_str(dec, "inf"));
}
END_TEST

START_TEST(s21_round_neginf) {
    s21_decimal dec = init_decimal(s21_NEGATIVE_INFINITY);
    dec = s21_round(dec);
    ck_assert(cmp_decimal_str(dec, "-inf"));
}
END_TEST

START_TEST(s21_round_def2) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    dec.bits[DEC_LOW] = 1234;
    dec.bits[DEC_MID] = 4643;
    dec.bits[DEC_HIGH] = 83672;
    set_exp(&dec, 5);
    set_bit(&dec, 1, SIGN_BIT);
    dec = s21_round(dec);
    ck_assert(cmp_decimal_str(dec, "-15434759701553671360"));
}
END_TEST

START_TEST(s21_round_def3) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    s21_from_int_to_decimal(999999, &dec);
    set_exp(&dec, 1);
    dec = s21_round(dec);
    ck_assert(cmp_decimal_str(dec, "100000"));
}
END_TEST

START_TEST(s21_round_def4) {
    s21_decimal dec = init_decimal(s21_NORMAL_VALUE);
    s21_from_int_to_decimal(999999, &dec);
    set_exp(&dec, 6);
    dec = s21_round(dec);
    ck_assert(cmp_decimal_str(dec, "1"));
}
END_TEST

// Функция создания набора тестов.
Suite *miscellaneous_suite(void) {
    Suite *suite = suite_create("Miscellaneous");
    TCase *TRU = tcase_create("Truncate");
    tcase_add_test(TRU, s21_truncate_normal);
    tcase_add_test(TRU, s21_truncate_max);
    tcase_add_test(TRU, s21_truncate_negmax);
    tcase_add_test(TRU, s21_truncate_def1);
    tcase_add_test(TRU, s21_truncate_zero);
    tcase_add_test(TRU, s21_truncate_nan);
    tcase_add_test(TRU, s21_truncate_inf);
    tcase_add_test(TRU, s21_truncate_neginf);
    tcase_add_test(TRU, s21_truncate_def2);
    tcase_add_test(TRU, s21_truncate_def3);
    tcase_add_test(TRU, s21_truncate_def4);
    suite_add_tcase(suite, TRU);
    TCase *FLO = tcase_create("FLO");
    tcase_add_test(TRU, s21_floor_normal);
    tcase_add_test(FLO, s21_floor_max);
    tcase_add_test(FLO, s21_floor_negmax);
    tcase_add_test(FLO, s21_floor_def1);
    tcase_add_test(FLO, s21_floor_zero);
    tcase_add_test(FLO, s21_floor_nan);
    tcase_add_test(FLO, s21_floor_inf);
    tcase_add_test(FLO, s21_floor_neginf);
    tcase_add_test(FLO, s21_floor_def2);
    tcase_add_test(FLO, s21_floor_def3);
    tcase_add_test(FLO, s21_floor_def4);
    suite_add_tcase(suite, FLO);
    TCase *NEG = tcase_create("Negate");
    tcase_add_test(NEG, s21_negate_default);
    tcase_add_test(NEG, s21_negate_float);
    tcase_add_test(NEG, s21_negate_inf);
    tcase_add_test(NEG, s21_negate_neginf);
    tcase_add_test(NEG, s21_negate_nan);
    tcase_add_test(NEG, s21_negate_max);
    suite_add_tcase(suite, NEG);
    TCase *ROU = tcase_create("Round");
    tcase_add_test(TRU, s21_round_normal);
    tcase_add_test(ROU, s21_round_max);
    tcase_add_test(ROU, s21_round_negmax);
    tcase_add_test(ROU, s21_round_def1);
    tcase_add_test(ROU, s21_round_zero);
    tcase_add_test(ROU, s21_round_nan);
    tcase_add_test(ROU, s21_round_inf);
    tcase_add_test(ROU, s21_round_neginf);
    tcase_add_test(ROU, s21_round_def2);
    tcase_add_test(ROU, s21_round_def3);
    tcase_add_test(ROU, s21_round_def4);
    suite_add_tcase(suite, ROU);

    return suite;
}
