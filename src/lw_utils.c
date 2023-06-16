// Copyright 2021 <lwolmer>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "lw_utils.h"

#if (defined(_WIN32) || defined(__linux__))
// taken from glibc/stdlib/rand_r.c
int rand_r(unsigned int *seed) {
    unsigned int next = *seed;
    int result;

    next *= 1103515245;
    next += 12345;
    result = (unsigned int) (next / 65536) % 2048;

    next *= 1103515245;
    next += 12345;
    result <<= 10;
    result ^= (unsigned int) (next / 65536) % 1024;

    next *= 1103515245;
    next += 12345;
    result <<= 10;
    result ^= (unsigned int) (next / 65536) % 1024;

    *seed = next;
    return result;
}
#endif

char *data_to_binary_string(const void *data, unsigned char bits) {
    char *bits_str = malloc(bits + 1);
    bits_str[bits] = 0;
    for (int i = bits - 1; i >= 0 ; i--)
        bits_str[bits - 1 - i] = *((unsigned long long*)data) & (1U << i) ? '1' : '0';
    return bits_str;
}

void print_binary(const void *data, unsigned char bits) {
    char *bits_str = data_to_binary_string(data, bits);
    printf("[%s]\n", bits_str);
    free(bits_str);
}

long max_long(long a, long b) {
    return a > b ? a : b;
}

long min_long(long a, long b) {
    return a < b ? a : b;
}


bool is_number(const char *str) {
    bool error = 0;
    size_t index = 0;
    while (str && *str && !error) {
        if (!isdigit(*str)) {
            if (!(index == 0 && (*str == '-' || *str == '+')))
                error = 1;
        }
        str++;
    }
    return !error;
}

void str_reverse(char *str) {
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char t = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = t;
    }
}

dynamic_string *DS_realloc(dynamic_string *dest, const size_t mem_size) {
    dest->string = realloc(dest->string, mem_size);
    if (dest->string == NULL)
        fprintf(stderr, "ERROR (dynamic_string): Cant realloc.\n");
    else
        dest->mem_size = mem_size;
    return dest;
}

void DS_free(dynamic_string *ds) {
    free(ds->string);
    free(ds);
}

dynamic_string *DS_set_text(dynamic_string *dest, char *src) {
    if (dest) {
        size_t src_len = strlen(src);
        size_t new_mem_size = max_long((src_len + 1), dest->mem_size);
        if (new_mem_size > dest->mem_size)
            dest = DS_realloc(dest, new_mem_size);
        if (dest) {
            dest->length = src_len;
            dest->string = strncpy(dest->string, src, dest->mem_size);
        }
    }
    return dest;
}

dynamic_string *DS_init(char *str) {
    dynamic_string *string = malloc(sizeof(dynamic_string));
    if (string) {
        string->mem_size = 20;
        string->string = malloc(string->mem_size);
        if (string->string == NULL) {
            fprintf(stderr, "ERROR (dynamic_string): Couldnt malloc.\n");
            free(string);
            string = NULL;
        } else {
            if (str)
                string = DS_set_text(string, str);
            else
                string = DS_set_text(string, "");
        }
    }
    return string;
}

dynamic_string *DS_insert_text(dynamic_string *dest, const char *src, const size_t pos) {
    size_t src_len = strlen(src);
    if (dest && src_len) {
        size_t new_mem_size = src_len + dest->length + 1;
        if (new_mem_size > dest->mem_size)
            dest = DS_realloc(dest, new_mem_size);
        memmove(dest->string + pos + src_len, dest->string + pos, dest->length - pos + 1);
        memcpy(dest->string + pos, src, src_len);
        dest->length = new_mem_size - 1;
    } else if (dest) {
        if (!src_len) {
            fprintf(stderr, "ERROR (dynamic_string):[%s][%zu] trying to insert string '%s'" \
            " that is empty.\n", dest->string, dest->length, src);
        } else {
            fprintf(stderr, "ERROR (dynamic_string):[%s][%zu] trying to insert string '%s'" \
            " at position '%zu' that is out of bounds.\n", dest->string, dest->length, src, pos);
        }
    }
    return dest;
}

dynamic_string *DS_append_char(dynamic_string *dest, const char src) {
    return DS_set_char(dest, src, dest->length);
}

dynamic_string *DS_set_char(dynamic_string *dest, const char src, const size_t pos) {
    if (dest) {
        // Trying to put char inside string
        if (pos < dest->length) {
            dest->string[pos] = src;
            if (src == 0)
                dest->length = pos;
        // Trying to put char at end of the string and have enough memory
        } else if (pos == dest->length && dest->length < dest->mem_size - 1) {
            if (!(src == 0 && dest->string[pos] == 0)) {
                dest->string[pos] = src;
                dest->length++;
            }
        // Trying to put char at end of the string and need to allocate more memory
        } else if (pos == dest->length && dest->length == dest->mem_size - 1) {
            if (!(src == 0 && dest->string[pos] == 0)) {
                dest = DS_realloc(dest, dest->mem_size + sizeof(char));
                if (dest) {
                    dest->string[pos] = src;
                    dest->length++;
                    dest->string[dest->length] = 0;
                }
            }
        } else if (pos > dest->length) {
            fprintf(stderr, "ERROR (dynamic_string):[%s][%zu] trying to set character '%c'" \
            " at position '%zu' that is out of bounds.\n", dest->string, dest->length, src, pos);
        }
    }
    return dest;
}

dynamic_string *sum_strings(dynamic_string *result, const char *str1, const size_t len1,
                    const char *str2, const size_t len2, size_t offset, bool using_reversed) {
    if (offset > len1 - 1) {
        fprintf(stderr, "ERROR (sum_strings): offset cant be more than str1 lenght.\n");
    } else {
        char *buf1 = malloc(len1 + 1);
        char *buf2 = malloc(len2 + 1);
        if (!buf1 || !buf2) {
            fprintf(stderr, "ERROR (sum_strings): error during malloc\n");
        } else {
            buf1 = strncpy(buf1, str1, len1 + 1);
            buf2 = strncpy(buf2, str2, len2 + 1);
            if (!using_reversed) {
                str_reverse(buf1);
                str_reverse(buf2);
            }
            size_t index = 0;
            int carryover = 0;
            char *str1_p = buf1;
            char *str2_p = buf2;
            while (*str1_p || *str2_p) {
                if (offset) {
                    DS_set_char(result, *str1_p, index);
                    index++;
                    str1_p++;
                    offset--;
                } else {
                    int d = carryover;
                    if (*str1_p) {
                        d += *str1_p - '0';
                        str1_p++;
                    }
                    if (*str2_p) {
                        d += *str2_p - '0';
                        str2_p++;
                    }
                    result = DS_set_char(result, d % 10 + '0', index);
                    carryover = d / 10;
                    index++;
                }
            }
            if (carryover) {
                result = DS_set_char(result, carryover + '0', index);
                index++;
            }
            result = DS_set_char(result, 0, index);
            if (!using_reversed) {
                str_reverse(result->string);
            }
            free(buf1);
            free(buf2);
        }
    }
    return result;
}
