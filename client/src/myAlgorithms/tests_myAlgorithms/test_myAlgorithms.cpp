// client/src/myAlgorithms/test_myAlgorithms.cpp

#include <check.h>
#include <stdlib.h>

// TEST 1.0 Port

START_TEST(test_Checking_if_a_port_value_is_valid)
{

    // Test 1.1. Permissible values (positive tests 1, 80, 443, 8080, 49152, 65535)
    ck_assert_int_eq(Checking_if_a_port_value_is_valid("1"), 1);
    ck_assert_int_eq(Checking_if_a_port_value_is_valid("80"), 80);
    ck_assert_int_eq(Checking_if_a_port_value_is_valid("8080"), 8080);
    ck_assert_int_eq(Checking_if_a_port_value_is_valid("49152"), 49152);
    ck_assert_int_eq(Checking_if_a_port_value_is_valid("65535"), 65535);
    ck_assert_int_eq(Checking_if_a_port_value_is_valid("    34565"), 34565);
    ck_assert_int_eq(Checking_if_a_port_value_is_valid("  54365 "), 54365);

    // Test 1.2 Unshakable values (negative tests "-1","0", "65536", "-1000000000", "1000000000", "w124a", "1sd23s", "12 43f de01")
    ck_assert_int_eq(Checking_if_a_port_value_is_valid("0"), NULL);
    ck_assert_int_eq(Checking_if_a_port_value_is_valid("-1"), -1);
    ck_assert_int_eq(Checking_if_a_port_value_is_valid("65536"), -1);
    ck_assert_int_eq(Checking_if_a_port_value_is_valid("-1000000000"), -1);
    ck_assert_int_eq(Checking_if_a_port_value_is_valid("1000000000"), -1);
    ck_assert_int_eq(Checking_if_a_port_value_is_valid("w124a"), -1);
    ck_assert_int_eq(Checking_if_a_port_value_is_valid("1sd23s"), -1);
    ck_assert_int_eq(Checking_if_a_port_value_is_valid("12 43f de01"), -1);
}
END_TEST


// TEST 2.0 Ip

START_TEST(test_Checking_the_validity_of_the_IP_value)
{
    // Test 2.1. Permissible values (positive tests "0.0.0.0", "127.0.0.1", "192.168.1.1", "255.255.255.255", "8.8.8.8", "169.254.1.1")
    ck_assert_int_eq(Checking_the_validity_of_the_IP_value("0.0.0.0"), true);
    ck_assert_int_eq(Checking_the_validity_of_the_IP_value("127.0.0.1"), true);
    ck_assert_int_eq(Checking_the_validity_of_the_IP_value("192.168.1.1"), true);
    ck_assert_int_eq(Checking_the_validity_of_the_IP_value("255.255.255.255"), true);
    ck_assert_int_eq(Checking_the_validity_of_the_IP_value("8.8.8.8"), true);
    ck_assert_int_eq(Checking_the_validity_of_the_IP_value("169.254.1.1"), true);

    // Test 2.2 Unshakable values (negative tests "256.1.1.1", "192.168.1", "192.168.1.1.1", "192.168.1. ", "a.b.c.d", "192. .168.1.1", "300.400.500.600", "localhost", "")
    ck_assert_int_eq(Checking_the_validity_of_the_IP_value("256.1.1.1"), false);
    ck_assert_int_eq(Checking_the_validity_of_the_IP_value("192.168.1"), false);
    ck_assert_int_eq(Checking_the_validity_of_the_IP_value("192.168.1.1.1"), false);
    ck_assert_int_eq(Checking_the_validity_of_the_IP_value("192.168.1. "), false);
    ck_assert_int_eq(Checking_the_validity_of_the_IP_value("a.b.c.d"), false);
    ck_assert_int_eq(Checking_the_validity_of_the_IP_value("192. .168.1.1"), false);
    ck_assert_int_eq(Checking_the_validity_of_the_IP_value("300.400.500.600"), false);
    ck_assert_int_eq(Checking_the_validity_of_the_IP_value("localhost"), false);
    ck_assert_int_eq(Checking_the_validity_of_the_IP_value(""), false);
}
END_TEST


Suite *Algorithms_suite(void)
{
    Suite *s = suite_create("Algorithms Tests");

    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_Cheking_if_a_port_value_is_valid);
    tcase_add_test(tc_core, test_Checking_the_validity_of_the_IP_value);

    cuite_add_tcase(s, tc_core);
}


int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = Algorithms_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;

}
