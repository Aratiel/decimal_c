// Copyright 2022 <lwolmer, tfelton, stogg, jgerrick>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "s21_decimal.h"
#include "utilities.h"


int s21_from_str_to_decimal(char *str, s21_decimal *dst) {
    int conv_status = OPR_SUCCESS;
    if (dst) {
        size_t str_len = strlen(str);
        if (str_len) {
            if (strcmp("inf", str) == 0) {
                *dst = init_decimal(s21_INFINITY);
            } else if (strcmp("-inf", str) == 0) {
                *dst = init_decimal(s21_NEGATIVE_INFINITY);
            } else if (strcmp("nan", str) == 0) {
                *dst = init_decimal(s21_NAN);
            } else {
                *dst = init_decimal(s21_NORMAL_VALUE);
                bool dot_found = false;
                bool sign_found = false;
                unsigned char exp = 0;
                bool is_negative = 0;
                s21_decimal dec_ten;
                s21_from_int_to_decimal(10, &dec_ten);
                int index = 0;
                for (int i = str_len - 1; i >= 0 && conv_status == OPR_SUCCESS; i--) {
                    if (!dot_found && str[i] == '.') {
                        dot_found = true;
                        exp = str_len - 1 - i;
                    } else if (!sign_found && (str[i] == '-' || str[i] == '+')) {
                        sign_found = true;
                        if (str[i] == '-')
                            is_negative = true;
                    } else {
                        int digit = str[i] - '0';
                        if (digit < 0 || digit > 9) {
                            conv_status = OPR_ERROR;
                        } else {
                            s21_decimal dec_result;
                            s21_from_int_to_decimal(digit, &dec_result);
                            for (int j = 0; j < index; j++)
                                dec_result = s21_mul(dec_result, dec_ten);
                            *dst = s21_add(*dst, dec_result);
                            index++;
                        }
                    }
                }
                if (exp)
                    set_exp(dst, exp);
                if (is_negative)
                    *dst = s21_negate(*dst);
                if (conv_status == OPR_ERROR)
                    *dst = init_decimal(s21_NAN);
            }
        } else {
            conv_status = OPR_ERROR;
        }
    }
    return conv_status;
}


int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    int success = OPR_ERROR;
    if (dst != NULL) {
        *dst = init_decimal(s21_NORMAL_VALUE);
        if (src < 0) {
            set_bit(dst, 1, SIGN_BIT);
            src = ~src + 1;
        }
        dst->bits[DEC_LOW] = src;
        success = OPR_SUCCESS;
    }
    return success;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    int conv_status = OPR_ERROR;
    if (dst && is_normal(src)) {
        src = s21_truncate(src);
        int sign = get_sign(src);
        if (!src.bits[DEC_MID] && !src.bits[DEC_HIGH] && \
             ((sign == -1 && src.bits[DEC_LOW] <= (unsigned int)INT_MAX + 1U) || \
                src.bits[DEC_LOW] <= (unsigned int)INT_MAX)) {
            *dst = src.bits[DEC_LOW];
            if (sign == -1) {
                *dst = ~*dst + 1;
            }
            conv_status = OPR_SUCCESS;
        }
    }
    return conv_status;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    int success = OPR_SUCCESS;
    if (dst) {
        *dst = init_decimal(s21_NORMAL_VALUE);
        if (isinf(src)) {
            *dst = init_decimal(s21_INFINITY);
        }
        if (isinf(src) && src < 0) {
            *dst = init_decimal(s21_NEGATIVE_INFINITY);
        }
        if (isnan(src)) {
            *dst = init_decimal(s21_NAN);
        }
        if (is_normal(*dst)) {
            char buf[100] = {0};
            int printed = snprintf(buf, sizeof(buf), "%.7f", src);
            size_t buf_len = strlen(buf);
            if (buf_len && (strchr(buf, '.') || strchr(buf, ','))) {
                bool dot_reached = false;
                for (int i = buf_len - 1; i >= 0 && !dot_reached &&
                    (buf[i] == '0' || buf[i] == '.' || buf[i] == ','); i--) {
                    if (buf[i] == '.' || buf[i] == ',')
                        dot_reached = true;
                    buf[i] = 0;
                }
            }
            if (printed < 0) {
                success = OPR_ERROR;
            } else {
                success = s21_from_str_to_decimal(buf, dst);
            }
        }
        if (success == OPR_ERROR) {
            *dst = init_decimal(s21_NORMAL_VALUE);
        }
    } else {
        success = OPR_ERROR;
    }
    return success;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    int conv_status = OPR_SUCCESS;
    if (dst) {
        if (is_normal(src)) {
            *dst = 0;
            dynamic_string *str = decimal_to_str(src);
            sscanf(str->string, "%f", dst);
            DS_free(str);
            if (*dst == 0 && !is_zero(src))
                conv_status = OPR_ERROR;
        } else if (src.value_type == s21_INFINITY) {
            *dst = INFINITY;
        } else if (src.value_type == s21_NEGATIVE_INFINITY) {
            *dst = -INFINITY;
        } else if (is_nan(src)) {
            *dst = NAN;
        } else {
            conv_status = OPR_ERROR;
        }
    }
    return conv_status;
}
