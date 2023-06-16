// Copyright 2021 <lwolmer>
#ifndef SRC_LW_UTILS_H_
#define SRC_LW_UTILS_H_
#include <stdbool.h>
#include <stdio.h>
#if defined(__linux__)
#include <sys/types.h>
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#if (defined(_WIN32) || defined(__linux__))
/**
    @brief Thread safe random int generator (Shameless plug for Windows)
    
    @param seed Seed value that is used to generate number (changed internaly every call)
    @return int : Random int
*/
int rand_r(unsigned int *seed);
#endif
/**
    @brief Structure for holding dynamic string
*/
typedef struct {
    char *string;  /**< raw string, !DO NOT MODIFY DIRECTLY!*/
    size_t mem_size;  /**< size of the string array*/
    size_t length;  /**< length of the string without terminating zero*/
} dynamic_string;


/**
    @brief Returns !dynamic string containing bit representation of passed variable
    
    @param data Pointer containing variable to process
    @param bits Amount of bits to process
    @return char* : Dynamicly allocated string containing bit representation
*/
char *data_to_binary_string(const void *data, unsigned char bits);

/**
    @brief Prints to stdout bit representation of passed variable
    
    @param data Pointer containing variable to process
    @param bits Amount of bits to process
*/
void print_binary(const void *data, unsigned char bits);

/**
    @brief Returns the maximum of given numbers

    @param a first number to compare
    @param b second number to compare
    @return long : The largest number from numbers passed as args
*/
long max_long(long a, long b);

/**
    @brief Returns the minimum of given numbers

    @param a first number to compare
    @param b second number to compare
    @return long : smallest number from numbers passed as args
*/
long min_long(long a, long b);
/**
    @brief Clamps the number in given limits

    @param d number that going to be clamped
    @param min minimum limit
    @param max maximum limit
    @return long : Clamped number
*/
long clamp_long(long d, long min, long max);

/**
    @brief Reverses string

    @param str string that will reversed
*/
void str_reverse(char *str);

dynamic_string *DS_init(char *str);

/**
    @brief Reallocs dynamic_string using new size

    @param dest Dynamic string that will be resized
    @return dynamic_string* : Resized dest
*/
dynamic_string *DS_realloc(dynamic_string *dest, const size_t mem_size);

/**
    @brief Frees memory for dynamic string

    @param ds Dynamic string that will free'd
*/
void DS_free(dynamic_string *ds);

/**
    @brief Puts text into dynamic_string and resizes it accordingly

    @param dest Dynamic string that will be modified
    @param src Raw text that will be used
    @return dynamic_string* : Modified dynamic string
*/
dynamic_string *DS_set_text(dynamic_string *dest, char *src);

/**
    @brief Puts char into dynamic_string and resizes it accordingly (note that if char is placed in postion of terminating zero it will put char there and resize string)

    @param dest Dynamic string that will be modified
    @param src Char that will be used
    @param pos Position in which char is placed
    @return dynamic_string* : Modified dynamic string
*/
dynamic_string *DS_set_char(dynamic_string *dest, const char src, const size_t pos);
/**
    @brief Appends char at the end of dynamic string
    
    @param dest Dynamic string that will be modified
    @param src Char to append
    @return dynamic_string* : Modified dynamic string
*/
dynamic_string *DS_append_char(dynamic_string *dest, const char src);
/**
    @brief Inserts text into dynamic string.

    @param dest  Dynamic string that will be modified
    @param src text to insert
    @param pos Position in which text is placed
    @return dynamic_string* 
*/
dynamic_string *DS_insert_text(dynamic_string *dest, const char *src, const size_t pos);

/**
    @brief Sums two strings (uses buffers, so you can pass the string from 'result', DOES NOT SUPPORT NEGATIVE NUMBERS YET)
    
    @param result Dynamic string that will hold results of addition
    @param str1 First string containing number
    @param len1 Length of the str1
    @param str2 Second string containing number
    @param len2 Length of the str2
    @param offset Starts adding str2 to str1 from this postion from the right
    @param using_reversed if true then string must be in reverse order and result will be returned in reverse order
    @return dynamic_string* : result of addition that is reversed if using_reversed set to true
*/
dynamic_string *sum_strings(dynamic_string *result, const char *str1, const size_t len1,
                            const char *str2, const size_t len2, size_t offset, bool using_reversed);
#endif  // SRC_LW_UTILS_H_
