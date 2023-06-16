// Copyright 2022 <lwolmer, tfelton, stogg, jgerrick>
#include "test.h"

int main(void) {
    Suite *suite_convertion = convertion_suite();
    Suite *suite_comparison = comparison_suite();
    Suite *suite_arithmetic = arithmetic_suite();
    Suite *suite_miscellaneous = miscellaneous_suite();
    SRunner *suite_runner = srunner_create(suite_convertion);
    srunner_add_suite(suite_runner, suite_comparison);
    srunner_add_suite(suite_runner, suite_arithmetic);
    srunner_add_suite(suite_runner, suite_miscellaneous);

    srunner_set_fork_status(suite_runner, CK_NOFORK);

    srunner_run_all(suite_runner, CK_NORMAL);
    // Получаем количество проваленных тестов.
    int failed_count = srunner_ntests_failed(suite_runner);
    srunner_free(suite_runner);

    return failed_count ? EXIT_FAILURE : EXIT_SUCCESS;
}

bool cmp_decimal_str(s21_decimal d1, const char* str) {
    dynamic_string *dstr = decimal_to_str(d1);
    int cmp_result = !strcmp(dstr->string, str);
    DS_free(dstr);
    return cmp_result;
}

s21_decimal get_max_decimal() {
    s21_decimal a = init_decimal(s21_NORMAL_VALUE);
    a.bits[DEC_LOW] = UINT_MAX;
    a.bits[DEC_MID] = UINT_MAX;
    a.bits[DEC_HIGH] = UINT_MAX;
    return a;
}

bool is_zeroed(s21_decimal d) {
    bool is_zeroed = true;
    if (is_normal(d)) {
        fprintf(stderr, "WARNING (is_zeroed()): Should be used with non normal values.\n");
    } else if (d.bits[DEC_LOW] || d.bits[DEC_MID] || d.bits[DEC_HIGH] || d.bits[DEC_MISC]) {
        is_zeroed = false;
    }
    return is_zeroed;
}
