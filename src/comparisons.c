// Copyright 2022 <lwolmer, tfelton, stogg, jgerrick>
#include <stdio.h>
#include <string.h>
#include "s21_decimal.h"
#include "utilities.h"
#include "lw_utils.h"

compare_result_flag compare(s21_decimal a, s21_decimal b) {
    compare_result_flag comp_result = CR_E;
    if (is_infinity(a) || is_infinity(b)) {
        if (a.value_type == s21_INFINITY && \
                (b.value_type == s21_NEGATIVE_INFINITY || is_normal(b))) {
            comp_result = CR_G;
        } else if (a.value_type == s21_NEGATIVE_INFINITY && \
                (b.value_type == s21_INFINITY || is_normal(b))) {
            comp_result = CR_L;
        } else if (is_normal(a)) {
            if (b.value_type == s21_NEGATIVE_INFINITY)
                comp_result = CR_G;
            else if (b.value_type == s21_INFINITY)
                comp_result = CR_L;
        }
        // else is equal
    } else if (!(is_zero(a) && is_zero(b))) {
        int sign_a = get_sign(a);
        int sign_b = get_sign(b);
        if (sign_a > sign_b) {
            comp_result = CR_G;
        } else if (sign_a < sign_b) {
            comp_result = CR_L;
        } else {
            unsigned char a_exp = get_exp(a);
            unsigned char b_exp = get_exp(b);
            unsigned int a_temp[BUFFER_SIZE] = {0};
            unsigned int b_temp[BUFFER_SIZE] = {0};
            memcpy(a_temp, a.bits, DEC_NUMBER_SIZE * sizeof(unsigned int));
            memcpy(b_temp, b.bits, DEC_NUMBER_SIZE * sizeof(unsigned int));
            if (a_exp - b_exp)
                equalise_exp(a_temp, b_temp, a_exp, b_exp);
            comp_result = arr_compare(a_temp, b_temp, BUFFER_SIZE, sign_a);
        }
    }
    return comp_result;
}

int s21_is_equal(s21_decimal a, s21_decimal b) {
    int comp_result = OPR_ERROR;
    if (!is_nan(a) && !is_nan(b) && compare(a, b) == CR_E)
        comp_result = OPR_SUCCESS;
    return comp_result;
}

int s21_is_not_equal(s21_decimal a, s21_decimal b) {
    int comp_result = OPR_ERROR;
    if (!is_nan(a) && !is_nan(b) && compare(a, b) != CR_E)
        comp_result = OPR_SUCCESS;
    return comp_result;
}

int s21_is_less(s21_decimal a, s21_decimal b) {
    int comp_result = OPR_ERROR;
    if (!is_nan(a) && !is_nan(b) && compare(a, b) == CR_L)
        comp_result = OPR_SUCCESS;
    return comp_result;
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
    int comp_result = OPR_ERROR;
    if (!is_nan(a) && !is_nan(b) && compare(a, b) == CR_G)
        comp_result = OPR_SUCCESS;
    return comp_result;
}

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
    int comp_result = OPR_ERROR;
    if (!is_nan(a) && !is_nan(b)) {
        compare_result_flag flag = compare(a, b);
        if (flag == CR_L || flag == CR_E)
            comp_result = OPR_SUCCESS;
    }
    return comp_result;
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
    int comp_result = OPR_ERROR;
    if (!is_nan(a) && !is_nan(b)) {
        compare_result_flag flag = compare(a, b);
        if (flag == CR_G || flag == CR_E)
            comp_result = OPR_SUCCESS;
    }
    return comp_result;
}
