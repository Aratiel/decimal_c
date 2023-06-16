// Copyright 2022 <lwolmer, tfelton, stogg, jgerrick>
#include <stdbool.h>
#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#define OPR_ERROR 1
#define OPR_SUCCESS 0
typedef enum {
    s21_NORMAL_VALUE = 0,
    s21_INFINITY = 1,
    s21_NEGATIVE_INFINITY = 2,
    s21_NAN = 3
} value_type_t;

typedef struct {
    unsigned int bits[4];
    value_type_t value_type;
} s21_decimal;

/**
    @brief Multyplies two numbers of type s21_decimal

    @param a factor
    @param b factor
    @return result of multiplication
*/
s21_decimal s21_mul(s21_decimal a, s21_decimal b);

/**
    @brief Converts from type integer to s21_decimal

    @param src source integer number
    @param dst pointer to s21_decimal structure which will hold convertion result
    @return 0 - Success 1 - Converting error 
*/
int s21_from_int_to_decimal(int src, s21_decimal *dst);

/**
    @brief Converts from type s21_decimal to integer
    
    @param src s21_decimal variable to be converted
    @param dst pointer to integer which will hold convertion result
    @return 0 - Success 1 - Converting error
*/
int s21_from_decimal_to_int(s21_decimal src, int *dst);

/**
    @brief Converts from type float to s21_decimal
    
    @param src float variable to be converted
    @param dst pointer to float which will hold converton result
    @return int: 0 - Success, 1 - Converting error.
*/
int s21_from_float_to_decimal(float src, s21_decimal *dst);

/**
    @brief Converts from type s21_decimal to float
    
    @param src s21_decimal variable to be converted
    @param dst pointer to float which will hold convertion result
    @return int: 0 - Success, 1 - Converting error.
*/
int s21_from_decimal_to_float(s21_decimal src, float *dst);

/**
    @brief Checks if a equal to b
    
    @param a Decimal that is being compared
    @param b Decimal to compare with
    @return int :  0 - Equal  1 - Not Equal
*/
int s21_is_equal(s21_decimal a, s21_decimal b);

/**
    @brief Checks if a is not equal to b
    
    @param a Decimal that is being compared
    @param b Decimal to compare with
    @return int : 0 - Not Equal 1 - Equal
*/
int s21_is_not_equal(s21_decimal a, s21_decimal b);

/**
    @brief Checks if a is less than b
    
    @param a Decimal that is being compared
    @param b Decimal to compare with
    @return int : 0 - a is less than b, 1 - other
*/
int s21_is_less(s21_decimal a, s21_decimal b);

/**
    @brief Checks if a is greater than b
    
    @param a Decimal that is being compared
    @param b Decimal to compare with
    @return int : 0 - a is greater than b, 1 other
*/
int s21_is_greater(s21_decimal a, s21_decimal b);

/**
    @brief Checks if a is less than or equal to b
    
    @param a Decimal that is being compared
    @param b Decimal to compare with
    @return int : 0 - a is less than or equal to b, 1 other
*/
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);

/**
    @brief Checks if a is greater than or equal b
    
    @param a Decimal that is being compared
    @param b Decimal to compare with
    @return int : 0 - a is greater than or equal b, 1 other
*/
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);

/**
    @brief Returns the result of multiplying the decimal by -1.

    @param s21_decimal Decimal
    @return s21_decimal: Decimal * -1
    
*/
s21_decimal s21_negate(s21_decimal a);

/**
    @brief Adds two decimals
    
    @param a 1st summand
    @param b 2nd summand
    @return s21_decimal : Result of addition
*/
s21_decimal s21_add(s21_decimal a, s21_decimal b);

/**
    @brief Substacts one decimal from another
    
    @param a Minuend
    @param b Subtrahend
    @return s21_decimal : Result of subtraction
*/
s21_decimal s21_sub(s21_decimal a, s21_decimal b);

/**
    @brief Returns the integral digits of the specified Decimal; any fractional digits are discarded.
    
    @param src Decimal
    @return s21_decimal integral digits
*/
s21_decimal s21_truncate(s21_decimal src);

/**
    @brief Round to negate infinity
    
    @param src Decimal
    @return s21_decimal Decimal value rounded to the closest integer toward negative infinity
*/
s21_decimal s21_floor(s21_decimal src);

/**
    @brief Rounds decimal
    
    @param src decimal
    @return s21_decimal Decimal value rounded to the nearest integer
*/
s21_decimal s21_round(s21_decimal src);

/**
    @brief Division decimal

    @param dividend Dividend decimal
    @param divisor Divisor decimal
    @return s21_decimal: Division result 
    
*/
s21_decimal s21_div(s21_decimal dividend, s21_decimal divisor);
/**
    @brief Remaider from decimal division
    
    @param dividend Devidend decimal
    @param divisor Divisor decimal
    @return s21_decimal Remainder
*/
s21_decimal s21_mod(s21_decimal dividend, s21_decimal divisor);
#endif  // SRC_S21_DECIMAL_H_
