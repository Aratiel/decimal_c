// Copyright 2022 <lwolmer, tfelton, stogg, jgerrick>
#include <string.h>
#include "s21_decimal.h"
#include "utilities.h"

s21_decimal s21_add(s21_decimal a, s21_decimal b) {
    s21_decimal result = init_decimal(s21_NAN);
    if (!is_nan(a) && !is_nan(b) && (is_infinity(a) || is_infinity(b))) {
        if ((a.value_type == s21_INFINITY && b.value_type == s21_NEGATIVE_INFINITY) || \
        (a.value_type == s21_NEGATIVE_INFINITY && b.value_type == s21_INFINITY)) {
            result.value_type = s21_NAN;
        } else {
            if (is_infinity(a))
                result.value_type = a.value_type;
            else if (is_infinity(b))
                result.value_type = b.value_type;
        }
    } else if (is_normal(a) && is_normal(b)) {
        if (get_sign(a) != get_sign(b)) {
            bool sign;
            if (s21_is_greater(s21_decimal_abs(a), s21_decimal_abs(b)) == OPR_SUCCESS) {
                sign = get_bit(a, SIGN_BIT);
            } else {
                sign = get_bit(b, SIGN_BIT);
            }
            result = s21_sub(s21_decimal_abs(a), s21_decimal_abs(b));
            set_bit(&result, sign, SIGN_BIT);
        } else {
            unsigned char a_exp = get_exp(a);
            unsigned char b_exp = get_exp(b);
            unsigned char exp = max_long(a_exp, b_exp);
            unsigned int a_temp[BUFFER_SIZE] = {0};
            unsigned int b_temp[BUFFER_SIZE] = {0};
            memcpy(a_temp, a.bits, DEC_NUMBER_SIZE * sizeof(unsigned int));
            memcpy(b_temp, b.bits, DEC_NUMBER_SIZE * sizeof(unsigned int));
            equalise_exp(a_temp, b_temp, a_exp, b_exp);
            unsigned int buffer[BUFFER_SIZE] = {0};
            add_auxiliary(buffer, BUFFER_SIZE, a_temp, b_temp, false);
            decrement_exp(buffer, BUFFER_SIZE, &exp);
            if (!is_overflow(buffer, BUFFER_SIZE)) {
                memcpy(result.bits, buffer, DEC_NUMBER_SIZE * sizeof(unsigned int));
                set_exp(&result, exp);
                if (!is_zero(result))
                    set_bit(&result, get_bit(a, SIGN_BIT), SIGN_BIT);
                result.value_type = s21_NORMAL_VALUE;
            } else if (get_sign(a) == 1) {
                result.value_type = s21_INFINITY;
            } else if (get_sign(a) == -1) {
                result.value_type = s21_NEGATIVE_INFINITY;
            }
        }
    }
    return result;
}

s21_decimal s21_sub(s21_decimal a, s21_decimal b) {
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    if (!is_nan(a) && !is_nan(b) && (is_infinity(a) || is_infinity(b))) {
        if (a.value_type == s21_INFINITY && b.value_type == s21_NEGATIVE_INFINITY) {
            result.value_type = s21_INFINITY;
        } else if (a.value_type == s21_NEGATIVE_INFINITY && b.value_type == s21_INFINITY) {
            result.value_type = s21_NEGATIVE_INFINITY;
        } else if (is_infinity(a) && is_infinity(b)) {
            result.value_type = s21_NAN;
        } else if (is_infinity(a)) {
            result.value_type = a.value_type;
        } else if (b.value_type == s21_INFINITY) {
            result.value_type = s21_NEGATIVE_INFINITY;
        } else if (b.value_type == s21_NEGATIVE_INFINITY) {
            result.value_type = s21_INFINITY;
        }
    } else if (is_normal(a) && is_normal(b)) {
        if (get_sign(a) != get_sign(b)) {
            bool sign = get_bit(a, SIGN_BIT);
            result = s21_add(s21_decimal_abs(a), s21_decimal_abs(b));
            if (is_normal(result))
                set_bit(&result, sign, SIGN_BIT);
            else if (result.value_type == s21_INFINITY && sign)
                result.value_type = s21_NEGATIVE_INFINITY;
        } else {
            unsigned char exp = max_long(get_exp(a), get_exp(b));
            bool sign;
            s21_decimal minuend, subtrahend;
            if (s21_is_greater(s21_decimal_abs(a), s21_decimal_abs(b)) == OPR_SUCCESS) {
                minuend = s21_decimal_abs(a);
                subtrahend = s21_decimal_abs(b);
                sign = get_bit(a, SIGN_BIT);
            } else {
                minuend = s21_decimal_abs(b);
                subtrahend = s21_decimal_abs(a);
                sign = !get_bit(a, SIGN_BIT);
            }
            unsigned int minuend_temp[BUFFER_SIZE] = {0};
            unsigned int subtrahend_temp[BUFFER_SIZE] = {0};
            memcpy(minuend_temp, minuend.bits, DEC_NUMBER_SIZE * sizeof(unsigned int));
            memcpy(subtrahend_temp, subtrahend.bits, DEC_NUMBER_SIZE * sizeof(unsigned int));
            unsigned char minuend_exp = get_exp(minuend);
            unsigned char subtrahend_exp = get_exp(subtrahend);
            equalise_exp(minuend_temp, subtrahend_temp, minuend_exp, subtrahend_exp);
            unsigned int buffer[BUFFER_SIZE] = {0};
            add_auxiliary(buffer, BUFFER_SIZE, minuend_temp, subtrahend_temp, true);
            decrement_exp(buffer, BUFFER_SIZE, &exp);
            if (!is_overflow(buffer, BUFFER_SIZE)) {
                memcpy(result.bits, buffer, DEC_NUMBER_SIZE * sizeof(unsigned int));
                set_exp(&result, exp);
                if (!is_zero(result))
                    set_bit(&result, sign, SIGN_BIT);
            }
        }
    } else {
        result.value_type = s21_NAN;
    }
    return result;
}

value_type_t define_mul_value_type(s21_decimal a, s21_decimal b) {
    value_type_t result_type = s21_NORMAL_VALUE;
    if (is_nan(a) || is_nan(b)) {
        result_type = s21_NAN;
    } else if (is_infinity(a) || is_infinity(b)) {
        if (is_zero(a) || is_zero(b))
            result_type = s21_NAN;
        else if (is_negative(a) == is_negative(b))
            result_type = s21_INFINITY;
        else
            result_type = s21_NEGATIVE_INFINITY;
    }
    return result_type;
}

s21_decimal s21_mul(s21_decimal a, s21_decimal b) {
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    value_type_t result_type = define_mul_value_type(a, b);
    if (result_type == s21_NORMAL_VALUE) {
        int sign_a = get_sign(a);
        int sign_b = get_sign(b);
        int flag_sign = 0;
        if (sign_a != sign_b)
            flag_sign = 1;
        unsigned int buffer[BUFFER_SIZE] = {0};
        unsigned int tmp_a[BUFFER_SIZE] = {0};
        unsigned int tmp_b[BUFFER_SIZE] = {0};
        memcpy(tmp_a, a.bits, DEC_NUMBER_SIZE * sizeof(unsigned int));
        memcpy(tmp_b, b.bits, DEC_NUMBER_SIZE * sizeof(unsigned int));
        mul_auxiliary(buffer, BUFFER_SIZE, tmp_a, tmp_b);
        unsigned int result_exp = get_exp(a) + get_exp(b);
        bool can_round = 1;
        unsigned int truncated = 0;
        while ((result_exp > 28 || is_overflow(buffer, BUFFER_SIZE)) && can_round) {
            if (result_exp) {
                result_exp--;
                truncated = truncate_auxiliary(buffer, BUFFER_SIZE);
            } else {
                if (flag_sign == 1)
                    result = init_decimal(s21_NEGATIVE_INFINITY);
                else
                    result = init_decimal(s21_INFINITY);
                can_round = 0;
            }
        }
        if (is_normal(result)) {
            if (truncated > 4)
                round_auxiliary(buffer, BUFFER_SIZE);
            memcpy(result.bits, buffer, DEC_NUMBER_SIZE * sizeof(unsigned int));
            if (!is_zero(result)) {
                set_exp(&result, result_exp);
                if (flag_sign)
                    set_bit(&result, 1, SIGN_BIT);
            }
        }
    } else {
        result = init_decimal(result_type);
    }
    return result;
}

s21_decimal s21_mod(s21_decimal dividend, s21_decimal divisor) {
    int sign = 0;
    if (get_bit(dividend, SIGN_BIT)) {
        sign = 1;
        dividend = s21_negate(dividend);
    }
    if (get_bit(divisor, SIGN_BIT)) {
        divisor = s21_negate(divisor);
    }
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    if (is_normal(dividend) && is_normal(divisor) && !is_zero(divisor)) {
        if (!is_zero(dividend)) {
            if (!s21_is_greater(dividend, divisor)) {
                int scale = 0;
                int result_scale = 0;
                if (get_exp(dividend) >= get_exp(divisor)) {
                    result_scale = get_exp(dividend);
                } else {
                    result_scale = get_exp(divisor);
                }
                unsigned int tmp_dividend[BUFFER_SIZE] =  {0};
                unsigned int tmp_divisor[BUFFER_SIZE] =  {0};
                unsigned int tmp_result_div[BUFFER_SIZE] =  {0};
                unsigned int tmp_result_mul[BUFFER_SIZE] =  {0};
                unsigned int tmp_result[BUFFER_SIZE] =  {0};
                unsigned int ten[BUFFER_SIZE] =  {10};
                memcpy(tmp_dividend, dividend.bits, DEC_NUMBER_SIZE * sizeof(unsigned int));
                memcpy(tmp_divisor, divisor.bits, DEC_NUMBER_SIZE * sizeof(unsigned int));
                div_auxiliary(tmp_result_div, tmp_dividend, tmp_divisor, &scale);
                while (scale > result_scale) {
                    scale--;
                    truncate_auxiliary(tmp_result_div, BUFFER_SIZE);
                }
                mul_auxiliary(tmp_result_mul, BUFFER_SIZE, tmp_result_div, tmp_divisor);
                int mul_scale = result_scale;
                while (mul_scale) {
                    mul_scale--;
                    mul_auxiliary(tmp_dividend, BUFFER_SIZE, tmp_dividend, ten);
                }
                add_auxiliary(tmp_result, BUFFER_SIZE, tmp_dividend, tmp_result_mul, 1);
                memcpy(result.bits, tmp_result, DEC_NUMBER_SIZE * sizeof(unsigned int));
                set_exp(&result, result_scale);
            } else {
                result = dividend;
            }
        }
    } else if (is_nan(divisor) || is_infinity(dividend) || is_nan(dividend)
    || is_zero(divisor)) {
        result.value_type = s21_NAN;
    } else if (is_infinity(divisor)) {
        result = dividend;
    }
    if (sign) {
        result = s21_negate(result);
    }
    return result;
}

s21_decimal s21_div(s21_decimal dividend, s21_decimal divisor) {
    int sign = 0;
    if (get_bit(dividend, SIGN_BIT) || dividend.value_type == s21_NEGATIVE_INFINITY) {
        sign = 1;
        dividend = s21_negate(dividend);
    }
    if (get_bit(divisor, SIGN_BIT) || divisor.value_type == s21_NEGATIVE_INFINITY) {
        sign = !sign;
    }
    s21_decimal result = init_decimal(s21_NORMAL_VALUE);
    unsigned int tmp_dividend[BUFFER_SIZE] = {0};
    unsigned int tmp_divisor[BUFFER_SIZE] = {0};
    memcpy(tmp_dividend, dividend.bits, DEC_NUMBER_SIZE * sizeof(unsigned int));
    memcpy(tmp_divisor, divisor.bits, DEC_NUMBER_SIZE * sizeof(unsigned int));
    int result_scale = get_exp(dividend) - get_exp(divisor);
    int scale = 0;
    if (is_normal(dividend) && is_normal(divisor) && !is_zero(divisor) && !is_zero(dividend)) {
        unsigned int tmp_result[BUFFER_SIZE] = {0};
        div_auxiliary(tmp_result, tmp_dividend, tmp_divisor, &scale);
        unsigned int rem = 0;
        scale += result_scale;
        while (scale > 28 || is_overflow(tmp_result, BUFFER_SIZE)) {
            scale--;
            rem = truncate_auxiliary(tmp_result, BUFFER_SIZE);
        }
        if (rem > 4) {
            round_auxiliary(tmp_result, BUFFER_SIZE);
        }
        memcpy(result.bits, tmp_result, DEC_NUMBER_SIZE * sizeof(unsigned int));
        if (scale >= 0) {
            set_exp(&result, scale);
        } else {
            result.value_type = s21_INFINITY;
        }
    } else if (is_nan(dividend) || is_nan(divisor) || (is_infinity(dividend) && is_infinity(divisor))
    || (is_zero(dividend) && is_zero(divisor))) {
        result.value_type = s21_NAN;
    } else if ((is_infinity(dividend) && is_normal(divisor)) || (is_normal(dividend) && is_zero(divisor))) {
        result.value_type = s21_INFINITY;
    }
    if (sign) {
        result = s21_negate(result);
    }
    return result;
}
