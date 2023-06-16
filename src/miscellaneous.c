// Copyright 2022 <lwolmer, tfelton, stogg, jgerrick>
#include <limits.h>
#include "s21_decimal.h"
#include "utilities.h"

s21_decimal truncate_floor_round(s21_decimal src, TFR_SELECT spec) {
    if (is_normal(src)) {
        int scale = get_exp(src);
        int sign = get_bit(src, SIGN_BIT);
        bool flag_add = false;
        for (int i = 0; i < scale; i++) {
            unsigned int truncated = truncate_auxiliary(src.bits, DEC_NUMBER_SIZE);
            if (truncated)
                flag_add = true;
            if (i == scale - 1 && spec) {
                if ((spec == TFR_FLOOR && flag_add && sign) || (spec == TFR_ROUND && truncated > 4)) {
                    round_auxiliary(src.bits, DEC_NUMBER_SIZE);
                }
            }
        }
        src.bits[DEC_MISC] = 0;
        set_bit(&src, sign, SIGN_BIT);
    }
    return src;
}

s21_decimal s21_floor(s21_decimal src) {
    return truncate_floor_round(src, TFR_FLOOR);
}

s21_decimal s21_round(s21_decimal src) {
    return truncate_floor_round(src, TFR_ROUND);
}

s21_decimal s21_truncate(s21_decimal src) {
    return truncate_floor_round(src, TFR_TRUNCATE);
}

s21_decimal s21_negate(s21_decimal a) {
    if (a.value_type != s21_NORMAL_VALUE) {
        if (a.value_type == s21_INFINITY) {
            a.value_type = s21_NEGATIVE_INFINITY;
        } else if (a.value_type == s21_NEGATIVE_INFINITY) {
            a.value_type = s21_INFINITY;
        }
    } else {
        a.bits[DEC_MISC] ^= (1U << (DEC_BITS - 1));
    }
    return a;
}
