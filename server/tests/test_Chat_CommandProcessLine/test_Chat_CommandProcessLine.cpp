// server/tests/test_Chat_CommandProcessLine/test_Chat_CommandProcessLine.cpp


#include <check.h>
#include <stdlib.h>


#include "../include/Chat.hpp"

const char *commands[] = {  "Commands: /help /users /name_users /change_name /quit",
                            "Unknow command. Write /help for commands list." };

// Tests 1.
START_TEST(test_help_unknow)
{
    char *result;

    result = CommandProcessLine("/help");
    ck_assert_str_eq(resutl, commands[0]);
    delete[] result;

    result = CommandProcessLine("/sal");
    ck_assert_str_eq(result, commands[1]);
    delete[] result;


}
END_TEST


Suite *Commands_suite(void)
{
    Suite *s = suite_create("Commands ProcessLine Tests");

    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, tes_help_unknow);

    suite_add_tcase(s, tc_core);
    return s;
}


int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = Commands_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;

}
