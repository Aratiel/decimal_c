// Copyright 2022 <lwolmer, tfelton, stogg, jgerrick>
#ifndef SRC_UTILITIES_H_
#define SRC_UTILITIES_H_
#include <stdbool.h>
#include <limits.h>
#include "s21_decimal.h"
#include "lw_utils.h"

#define MAX_LAST_DIGIT ((unsigned long long)UINT_MAX + 1) % 10
#define BUFFER_SIZE 6
#define DEC_NUMBER_SIZE 3

#define DEC_MISC_EXP_START 16
#define SIGN_BIT 127  // Additionaly the highest bit in s21_decimal->bits
#define DEC_BITS (CHAR_BIT * sizeof(unsigned int))

#define DEC_LOW_BITS DEC_BITS
#define DEC_MID_BITS (DEC_BITS * 2)
#define DEC_HIGH_BITS (DEC_BITS * 3)
#define DEC_MISC_BITS (DEC_BITS * 4)

/**
    @brief Enum that indicates corresponding index of s21_decimal->bits[i]
*/
typedef enum {
    DEC_LOW,  // Low bit element of s21_decimal (bits[0])
    DEC_MID,  // Middle bit element of s21_decimal (bits[1])
    DEC_HIGH,  // High bit element of s21_decimal (bits[2])
    DEC_MISC  // Scale factor and sign bit element of s21_decimal (bits[3])
} decimal_bit_element;

typedef enum {
    TFR_TRUNCATE,   // Truncate
    TFR_FLOOR,      // Floor
    TFR_ROUND       // Round
} TFR_SELECT;

/**
    @brief Enum needed for comparsion operators
*/
typedef enum {
    CR_E = 0,  // Equal
    CR_L = 1,  // Less
    CR_G = 2  // Greater
} compare_result_flag;

/**
    @brief Returns the value of the integer number with the given bit

    @param d s21_decimal structure to get bit from
    @param index bit index
    @return bool : Value of the integer number with the given bit i 
*/
bool get_bit(s21_decimal d, unsigned char index);

/**
    @brief Extracts k bits starting from index
    
    @param source integer from which bits will be extracted
    @param k amount of bits to extract
    @param index starting index where extraction starts
    @return int : extracted value
 */
unsigned int extract_bits(unsigned int number, unsigned char k, unsigned char index);

/**
    @brief Set k bits starting from index from source to dest
    
    @param dest pointer to destination integer into which source will be pasted 
    @param source integer from which bits will be extracted for pasting
    @param k amount of bits to paste
    @param index starting bit index where pasting will start
 */
void set_bits(unsigned int *dest, unsigned int source, unsigned char k, unsigned char index);

/**
    @brief Returns zero initialized s21_decimal and sets s21_decimal.value_type to given state
    
    @param state state of the number
 */
s21_decimal init_decimal(value_type_t state);

/**
    @brief Sets the value of the given bit

    @param d s21_decimal structure with decimal number where bit value is set
    @param value bit value to assign to
    @param index bit index
*/
void set_bit(s21_decimal *d, bool value, unsigned char index);

/**
    @brief Returns the sign of the decimal number

    @param d s21_decimal structure with decimal number
    @return int : 1 if positive, -1 if negative, 0 if s21_decimal's value_type set to s21_NAN
*/
int get_sign(s21_decimal d);

/**
    @brief Get the power of 10 that can be used to divide the integer number its also corresponds the amount of decimal places in floating point number.
    
    @param d s21_decimal structure with decimal number
    @return unsigned char : power of 10 for division or number of decimal places
*/
unsigned char get_exp(s21_decimal d);

/**
    @brief Set the exponent in s21_decimal to specific value
    
    @param d s21_decimal to modify
    @param exp Exponent to set
*/
void set_exp(s21_decimal *d, unsigned char exp);

/** 
    @brief Prints s21_decimal to stdout

    @param d s21_decimal to print
*/
void print_decimal(s21_decimal d);

/** 
    @brief Prints s21_decimal to stdout in binary

    @param d s21_decimal to print
*/
void print_decimal_binary(s21_decimal d);

/**
    @brief Adds or subtracts one array with the other
    
    @param result Array that will hold result of the operation
    @param arr_len Length of arrays
    @param x First array
    @param y Second array
    @param subtract If true, then instead of addition second array will be subtracted from first
*/
void add_auxiliary(unsigned int *result, size_t arr_len, unsigned int *x, unsigned int *y, bool subtract);

/**
    @brief Multiplies one array by another

    @param result Pointer to the product of multiplication
    @param arr_len Size of result array
    @param a Pointer to the first array
    @param b Pointer to the second array
*/
void mul_auxiliary(unsigned int *result, size_t arr_len, unsigned int *a, unsigned int *b);

/**
    @brief Returns the absolute value of the decimal
    
    @param d Decimal to use
    @return s21_decimal : Absolute decimal value
*/
s21_decimal s21_decimal_abs(s21_decimal d);

/**
    @brief Compares two arrays bit by bit and returns one of the flags depending on the comparison result
    
    @param a Decimal that is being compared
    @param b Decimal to compare with
    @param size Size of arrays
    @return compare_result_flag : CR_E if equal, CR_G if greater than, CR_L if less than
    @param sign Sign of decimal numbers
*/
compare_result_flag arr_compare(unsigned int *a, unsigned int *b, size_t size, int sign);

/**
    @brief Decreases exponent of number in buffer
    
    @param buffer Unsigned int array with the number
    @param arr_len Size of buffer
    @param exp Exponent of the number from the buffer
 */
void decrement_exp(unsigned int *buffer, size_t arr_len, unsigned char *exp);

/**
    @brief The function of equalise the exponent of numbers
    
    @param a Unsigned int array with the number
    @param b Unsigned int array with the number
    @param a_exp Exponent of the number a
    @param b_exp Exponent of the number b
*/
void equalise_exp(unsigned int *a, unsigned int *b, unsigned char a_exp, unsigned char b_exp);

/**
    @brief Checks if s21_decimal is any type of infinity
    
    @param d s21_decimal
    @return bool : true if d is infinity or negative infinity
 */
bool is_infinity(s21_decimal d);

/**
    @brief Returns true if decimal's value_type is equal to s21_NAN
    
    @param d s21_decimal to check
    @return bool : true if decimal's value_type is equal to s21_NAN
*/
bool is_nan(s21_decimal d);

/**
    @brief Returns true if low, mid and high bits of decimal are equal to zero and decimal's value_type is equal to s21_NORMAL_VALUE.
    
    @param d s21_decimal to check
    @return bool : true if low, mid and high bits of decimal equals zero.
*/
bool is_zero(s21_decimal d);

/**
    @brief Returns true if decimal's value_type is equal to s21_NORMAL_VALUE
    
    @param d s21_decimal to check
    @return bool : true if decimal's value_type equals s21_NORMAL_VALUE
*/
bool is_normal(s21_decimal d);

/**
    @brief Returns true if decimal is negative
    
    @param d s21_decimal to check
    @return bool : true if decimal is negative or decimal's value_type is equal to s21_NEGATIVE_INFINITY
*/
bool is_negative(s21_decimal d);

/**
    @brief Checks if array can fit in s21_decimal
    
    @param array Array to check
    @param size Size of the array
    @return bool : true if value overflows
*/
bool is_overflow(unsigned int *array, size_t size);

/**
    @brief Convert decimal to dynamic_string (it should be free'd after use)
    
    @param d Decimal
    @return dynamic_string* : Pointer to dynamic_string containing decimal value
*/
dynamic_string *decimal_to_str(s21_decimal d);

/**
    @brief Adds 1 to buffer if remainder of division is equal or greater than 5
    
    @param buffer Unsigned int array to be rounded 
    @param arr_len Size of buffer
    @param banker If true, banker's rounding is used
    @param truncated The last truncated digit
*/
void round_auxiliary(unsigned int *buffer, size_t arr_len);

/**
    @brief Trancates buffer by 1 digit
    
    @param buffer Unsigned int array to be truncated
    @param arr_len Size of buffer
    @return Trancated digit
*/
unsigned int truncate_auxiliary(unsigned int *buffer, size_t arr_len);

/**
    @brief Division with BUFFER_SIZE = 6
    
    @param result buffer result
    @param dividend buffer dividend
    @param divisor buffer divisor
    @param scale scale need for truncate
*/
void div_auxiliary(unsigned int *result, unsigned int *dividend, unsigned int *divisor, int *scale);

#endif  // SRC_UTILITIES_H_
