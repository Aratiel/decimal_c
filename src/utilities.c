// Copyright 2022 <lwolmer, tfelton, stogg, jgerrick>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utilities.h"

bool arr_get_bit(const unsigned int *array, size_t index) {
    return array[index / DEC_BITS] & (1U << (index % DEC_BITS));
}

void arr_clear_bit(unsigned int *array, size_t index) {
    array[index / DEC_BITS] &= ~(1U << index % DEC_BITS);
}

void arr_set_bit(unsigned int *array, bool value, size_t index) {
    arr_clear_bit(array, index);
    if (value)
        array[index / DEC_BITS] |= 1U << (index % DEC_BITS);
}

bool get_bit(s21_decimal d, unsigned char index) {
    if (index > SIGN_BIT)
        fprintf(stderr, "ERROR (get_bit()): passed index is out of range.\n");
    return arr_get_bit(d.bits, index);
}

void set_bit(s21_decimal *d, bool value, unsigned char index) {
    if (index > SIGN_BIT)
        fprintf(stderr, "ERROR (set_bit()): passed index is out of range.\n");
    arr_set_bit(d->bits, value, index);
}

unsigned int extract_bits(unsigned int source, unsigned char k, unsigned char index) {
    return (((1 << k) - 1) & (source >> index));
}

void set_bits(unsigned *dest, unsigned int source, unsigned char k, unsigned char index) {
    unsigned int paste_mask = (1U << k) - 1;
    unsigned int preped_paste = (source & paste_mask) << index;
    unsigned int shifted_mask = paste_mask << index;
    *dest &= ~(shifted_mask);
    *dest |= preped_paste;
}

int get_sign(s21_decimal d) {
    if (is_nan(d))
        fprintf(stderr, "ERROR (get_sign()): passed decimal with nan value.\n");
    return get_bit(d, SIGN_BIT) ? -1 : 1;
}

void r_bit_shift(unsigned int *array, size_t size, size_t steps) {
    if (steps && array) {
        while (steps) {
            // 31 because shifting by more than 31 is undefined behaviour
            unsigned char mini_steps = min_long(steps, 31);
            steps -= mini_steps;
            unsigned int remainder_mask = (1U << mini_steps) - 1U;
            unsigned int remainder_prev = 0;
            for (long i = size - 1; i >= 0; --i) {
                unsigned int remainder = 0;
                remainder = array[i] & remainder_mask;
                remainder <<= (DEC_BITS - mini_steps);
                array[i] >>= mini_steps;
                if (remainder_prev)
                    array[i] |= remainder_prev;
                remainder_prev = remainder;
            }
        }
    }
}

void l_bit_shift(unsigned int *array, size_t size, size_t steps) {
    if (steps && array) {
        while (steps) {
            // 31 because shifting by more than 31 is undefined behaviour
            unsigned char mini_steps = min_long(steps, 31);
            steps -= mini_steps;
            unsigned int remainder_prev = 0;
            unsigned int remainder_mask = ((1U << mini_steps) - 1U) << (DEC_BITS - mini_steps);
            for (unsigned long i = 0; i < (unsigned long)size; i++) {
                unsigned int remainder = 0;
                remainder = array[i] & remainder_mask;
                remainder >>= DEC_BITS - mini_steps;
                array[i] <<= mini_steps;
                if (remainder_prev)
                    array[i] |= remainder_prev;
                remainder_prev = remainder;
            }
        }
    }
}

unsigned char get_exp(s21_decimal d) {
    return (unsigned char)extract_bits(d.bits[DEC_MISC], 8, 16);
}

void set_exp(s21_decimal *d, unsigned char exp) {
    if (exp > 28)
        fprintf(stderr, "ERROR (set_exp()): passed exponent is out of range.\n");
    else
        set_bits(&(d->bits[DEC_MISC]), exp, 8, DEC_MISC_EXP_START);
}

s21_decimal init_decimal(value_type_t state) {
    s21_decimal d;
    d.bits[DEC_LOW] = 0;
    d.bits[DEC_MID] = 0;
    d.bits[DEC_HIGH] = 0;
    d.bits[DEC_MISC] = 0;
    d.value_type = state;
    return d;
}

void print_decimal_binary(s21_decimal d) {
    // Printing in blocks for each int
    printf("[%s]", data_to_binary_string(&d.bits[DEC_HIGH], DEC_BITS));
    printf("[%s]", data_to_binary_string(&d.bits[DEC_MID], DEC_BITS));
    printf("[%s]", data_to_binary_string(&d.bits[DEC_LOW], DEC_BITS));
    printf("[%s]", data_to_binary_string(&d.bits[DEC_MISC], DEC_BITS));
    printf("\n");
}

dynamic_string *decimal_to_str(s21_decimal d) {
    dynamic_string *result = DS_init("0");
    if (is_nan(d)) {
        DS_set_text(result, "nan");
    } else if (d.value_type == s21_INFINITY) {
        DS_set_text(result, "inf");
    } else if (d.value_type == s21_NEGATIVE_INFINITY) {
        DS_set_text(result, "-inf");
    } else {
        if (is_zero(d)) {
            DS_set_text(result, "0");
        } else {
            dynamic_string *pow = DS_init("1");
            if (get_bit(d, 0))
                sum_strings(result, result->string, result->length, pow->string, pow->length, 0, false);
            for (size_t i = 1; i < DEC_HIGH_BITS; i++) {
                pow = sum_strings(pow, pow->string, pow->length, pow->string, pow->length, 0, false);
                if (get_bit(d, i))
                    sum_strings(result, result->string, result->length, pow->string, pow->length, 0, false);
            }
            DS_free(pow);
        }
        unsigned char exp = get_exp(d);
        if (exp) {
            if (exp >= result->length) {
                size_t extra_len = exp - result->length + 2;
                char *extra = calloc(1, extra_len + 1);
                extra[0] = '0';
                extra[1] = '.';
                if (exp > result->length) {
                    for (size_t i = 2; i < extra_len; i++)
                        extra[i] = '0';
                }
                DS_insert_text(result, extra, 0);
                free(extra);
            } else {
                DS_insert_text(result, ".", result->length - exp);
            }
        }
        if (get_sign(d) == -1)
            DS_insert_text(result, "-", 0);
    }
    return result;
}

void print_decimal(s21_decimal d) {
    dynamic_string* str = decimal_to_str(d);
    printf("%s\n", str->string);
    DS_free(str);
}

void arr_complement(unsigned int *result, size_t arr_len, const unsigned int *array) {
    for (size_t i = 0; i < arr_len; i++)
        result[i] = ~array[i];
}

void arr_and(unsigned int *result, size_t arr_len, const unsigned int *array1, const unsigned int *array2) {
    for (size_t i = 0; i < arr_len; i++)
        result[i] = array1[i] & array2[i];
}

void arr_xor(unsigned int *result, size_t arr_len, const unsigned int *array1, const unsigned int *array2) {
    for (size_t i = 0; i < arr_len; i++)
        result[i] = array1[i] ^ array2[i];
}

bool arr_is_zero(const unsigned int *array, size_t arr_len) {
    bool is_zero_flag = true;
    for (size_t i = 0; i < arr_len && is_zero_flag; i++)
        if (array[i] != 0)
            is_zero_flag = false;
    return is_zero_flag;
}

void add_auxiliary(unsigned int *result, size_t arr_len, unsigned int *x, unsigned int *y, bool subtract) {
    unsigned int *carry = (unsigned int*) malloc(arr_len * sizeof(unsigned int));
    unsigned int *y_temp = (unsigned int*) malloc(arr_len * sizeof(unsigned int));
    memcpy(result, x, arr_len * sizeof(unsigned int));
    memcpy(y_temp, y, arr_len * sizeof(unsigned int));
    while (!arr_is_zero(y_temp, arr_len)) {
        if (subtract) {
            arr_complement(carry, arr_len, result);
            arr_and(carry, arr_len, carry, y_temp);
        } else {
            arr_and(carry, arr_len, result, y_temp);
        }
        arr_xor(result, arr_len, result, y_temp);
        l_bit_shift(carry, arr_len, 1);
        memcpy(y_temp, carry, arr_len * sizeof(unsigned int));
    }
    free(y_temp);
    free(carry);
}

void mul_auxiliary(unsigned int *result, size_t arr_len, unsigned int *a, unsigned int *b) {
    unsigned int *a_temp = (unsigned int *) malloc(arr_len * sizeof(unsigned int));
    memcpy(a_temp, a, arr_len * sizeof(unsigned int));
    memset(result, 0, arr_len * sizeof(unsigned int));
    for (size_t i = 0; i < DEC_HIGH_BITS; i++) {
        if (arr_get_bit(b, i)) {
            add_auxiliary(result, arr_len, result, a_temp, false);
        }
        l_bit_shift(a_temp, arr_len, 1);
    }
    free(a_temp);
}

void decrement_exp(unsigned int *buffer, size_t arr_len, unsigned char *exp) {
    unsigned int truncated = 0;
    while (*exp && is_overflow(buffer, arr_len)) {
        truncated = truncate_auxiliary(buffer, arr_len);
        (*exp)--;
    }
    if (truncated > 4)
        round_auxiliary(buffer, arr_len);
}

void increment_exp(unsigned int *buffer, size_t arr_len, unsigned char delta_exp) {
    unsigned int *ten = (unsigned int *) malloc(arr_len * sizeof(unsigned int));
    memset(ten, 0, arr_len * sizeof(unsigned int));
    ten[0] = 10;
    while (delta_exp) {
        mul_auxiliary(buffer, arr_len, buffer, ten);
        delta_exp--;
    }
    free(ten);
}

void equalise_exp(unsigned int *a, unsigned int *b, unsigned char a_exp, unsigned char b_exp) {
    unsigned char delta_exp = 0;
    if (a_exp > b_exp) {
        delta_exp = a_exp - b_exp;
        increment_exp(b, BUFFER_SIZE, delta_exp);
    } else {
        delta_exp = b_exp - a_exp;
        increment_exp(a, BUFFER_SIZE, delta_exp);
    }
}

unsigned int truncate_auxiliary(unsigned int *buffer, size_t arr_len) {
    unsigned int *tmp_buffer = (unsigned int *) malloc(arr_len * sizeof(unsigned int));
    unsigned int truncated = 0;
    tmp_buffer[arr_len - 1] = buffer[arr_len - 1] % 10;
    for (int i = arr_len - 2; i >= 0; i--) {
        tmp_buffer[i] = buffer[i] % 10;
        tmp_buffer[i] = (tmp_buffer[i] + tmp_buffer[i + 1] * MAX_LAST_DIGIT) % 10;
    }
    buffer[arr_len - 1] /= 10;
    for (int i = arr_len - 2; i >= 0; i--) {
        buffer[i] = ((tmp_buffer[i + 1] * ((unsigned long long)UINT_MAX + 1)) + buffer[i]) / 10;
    }
    truncated = tmp_buffer[0];
    free(tmp_buffer);
    return truncated;
}

void round_auxiliary(unsigned int *buffer, size_t arr_len) {
    unsigned int *one = (unsigned int *) malloc(arr_len * sizeof(unsigned int));
    memset(one, 0, arr_len * sizeof(unsigned int));
    one[0] = 1;
    add_auxiliary(buffer, arr_len, buffer, one, false);
    free(one);
}

bool is_overflow(unsigned int *array, size_t size) {
    bool is_overflow = false;
    for (int i = size - 1; i >= DEC_NUMBER_SIZE && !is_overflow; i--) {
        if (array[i])
            is_overflow = true;
    }
    return is_overflow;
}

bool is_infinity(s21_decimal d) {
    return d.value_type == s21_INFINITY || d.value_type == s21_NEGATIVE_INFINITY ? true : false;
}

bool is_normal(s21_decimal d) {
    return d.value_type == s21_NORMAL_VALUE ? true : false;
}

bool is_negative(s21_decimal d) {
    return d.value_type == s21_NEGATIVE_INFINITY || (is_normal(d) && get_sign(d) == -1) ? true : false;
}

bool is_zero(s21_decimal d) {
    return is_normal(d) && !d.bits[DEC_LOW] && !d.bits[DEC_MID] && !d.bits[DEC_HIGH] ? true : false;
}

bool is_nan(s21_decimal d) {
    return d.value_type == s21_NAN ? true : false;
}

s21_decimal s21_decimal_abs(s21_decimal d) {
    set_bit(&d, 0, SIGN_BIT);
    return d;
}

compare_result_flag arr_compare(unsigned int *a, unsigned int *b, size_t size, int sign) {
    compare_result_flag comp_result = CR_E;
    for (int i = size * DEC_BITS - 1; i >= 0 && !comp_result; i--) {
        bool bit_a = arr_get_bit(a, i);
        bool bit_b = arr_get_bit(b, i);
        if (bit_a > bit_b) {
            if (sign > 0)
                comp_result = CR_G;
            else if (sign < 0)
                comp_result = CR_L;
        } else if (bit_b > bit_a) {
            if (sign > 0)
                comp_result = CR_L;
            else if (sign < 0)
                comp_result = CR_G;
        }
    }
    return comp_result;
}

int find_first_not_zero_element(unsigned int *src, size_t size) {
    int find = 0;
    int i = size * DEC_BITS;
    while (!find && i) {
        i--;
        find = arr_get_bit(src, i);
    }
    return i;
}

void normalize(unsigned int *dividend, unsigned int *divisor, unsigned int *result,
int *size, int *change_size) {
    int dividend_index = find_first_not_zero_element(dividend, BUFFER_SIZE);
    int divisor_index = find_first_not_zero_element(divisor, BUFFER_SIZE);
    int steps = dividend_index - divisor_index;
    while (steps < 0) {
        r_bit_shift(divisor, BUFFER_SIZE, 1);
        if (arr_compare(dividend, divisor, BUFFER_SIZE, 1) == CR_L) {
            l_bit_shift(result, BUFFER_SIZE, 1);
            (*size)--;
        }
        (steps)++;
    }
    l_bit_shift(divisor, BUFFER_SIZE, steps);
    if (arr_compare(dividend, divisor, BUFFER_SIZE, 1) == CR_L) {
        r_bit_shift(divisor, BUFFER_SIZE, 1);
        *change_size = 1;
    }
}

void div_int_and_fract(unsigned int *remainder, unsigned int *divisor, unsigned int *result) {
    memset(result, 0, BUFFER_SIZE * sizeof(unsigned int));
    unsigned int tempdivisor[BUFFER_SIZE] = {0};
    memcpy(tempdivisor, divisor, BUFFER_SIZE * sizeof(unsigned int));
    int divisor_index = find_first_not_zero_element(divisor, BUFFER_SIZE);
    int dividend_index = find_first_not_zero_element(remainder, BUFFER_SIZE);
    int change_size = 0;
    int size_tmp = dividend_index - divisor_index;
    int size = 0;
    normalize(remainder, tempdivisor, result, &size, &change_size);
    size = size_tmp - change_size + 1;
    while (arr_compare(remainder, divisor, BUFFER_SIZE, 1) == CR_G
        || arr_compare(remainder, divisor, BUFFER_SIZE, 1) == CR_E) {
        normalize(remainder, tempdivisor, result, &size, &change_size);
        add_auxiliary(remainder, BUFFER_SIZE, remainder, tempdivisor, true);
        l_bit_shift(result, BUFFER_SIZE, 1);
        result[0]++;
        size--;
        if (arr_compare(remainder, divisor, BUFFER_SIZE, 1) == CR_L) {
            l_bit_shift(result, BUFFER_SIZE, size);
        }
    }
}
void div_auxiliary(unsigned int *result, unsigned int *dividend, unsigned int *divisor, int *scale) {
    unsigned int tmp_dividend[BUFFER_SIZE] = {0};
    unsigned int tmp_divisor[BUFFER_SIZE] = {0};
    memcpy(tmp_dividend, dividend, BUFFER_SIZE * sizeof(unsigned int));
    memcpy(tmp_divisor, divisor, BUFFER_SIZE * sizeof(unsigned int));
    unsigned int tmp_result[BUFFER_SIZE] = {0};
    unsigned int ten[BUFFER_SIZE] = {10};
    int flag_stop = 0;
    while (!flag_stop) {
        unsigned int tmpres[BUFFER_SIZE + 1] = {0};
        memcpy(tmpres, result, BUFFER_SIZE * sizeof(unsigned int));
        mul_auxiliary(tmpres, BUFFER_SIZE + 1, tmpres, ten);
        if (tmpres[BUFFER_SIZE]) {
            flag_stop = 1;
        } else {
            mul_auxiliary(result, BUFFER_SIZE, result, ten);
            div_int_and_fract(tmp_dividend, tmp_divisor, tmp_result);
            mul_auxiliary(tmp_dividend, BUFFER_SIZE, tmp_dividend, ten);
            add_auxiliary(result, BUFFER_SIZE, result, tmp_result, false);
            (*scale)++;
        }
    }
    (*scale)--;
}
