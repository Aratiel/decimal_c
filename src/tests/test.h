// Copyright 2022 <lwolmer, tfelton, stogg, jgerrick>
#ifndef SRC_TESTS_TEST_H_
#define SRC_TESTS_TEST_H_
#include <check.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "s21_decimal.h"
#include "utilities.h"

Suite *arithmetic_suite(void);
Suite *comparison_suite(void);
Suite *convertion_suite(void);
Suite *miscellaneous_suite(void);

/**
    @brief Converts s21_decimal to string and compares it with another string
    
    @param d1 s21_decimal to convert and compare
    @param str String to compare with
    @return bool : true if they matches
*/

bool cmp_decimal_str(s21_decimal d1, const char* str);

/**
    @brief Get the max integer decimal
    
    @return s21_decimal : max possible integer decimal
*/
s21_decimal get_max_decimal();

/**
    @brief Checks if decimal was zero'ed when value_type is set to something other than s21_NORMAL_VALUE 
    
    @param d Decimal to check
    @return bool : true if it zero'ed or false if not
*/
bool is_zeroed(s21_decimal d);
#endif  // SRC_TESTS_TEST_H_
