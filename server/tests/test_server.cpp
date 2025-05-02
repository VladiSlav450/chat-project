// /server/tests/test_server.cpp

#include <check.h>
#include <arpa/inet.h>
#include "../include/TCPSocket.h"
#include "../include/IPv4Address.h"
#include "../include/ServerSocket.h"
#include "mocks.h"

// TEST: class IPv4Address.
//
// Test 1.1: ipv4 address create
START_TEST(test_default_constructor_sets_address)
{
    IPv4Address addr;

    ck_assert_ptr_ne(addr.GetAddr(), NULL);
    ck_assert_int_eq(addr.GetAddrLen(), sizeof(struct sockaddr_in));
}
END_TEST

// Test 1.2: test custom ip
START_TEST(test_constructor_with_port_sets_correct_port)
{
    IPv4Address addr(8080);
    const sockaddr_in *sa = (const sockaddr_in *)addr.GetAddr();

    ck_assert_int_eq(ntohs(sa->sin_port), 8080);
    ck_assert_int_eq(sa->sin_addr.s_addr, INADDR_ANY);
}
END_TEST
 
// Test 1.3 
START_TEST(test_constructor_with_ip_sets_correct_ip)
{
    IPv4Address addr(8080, "127.0.0.1");
    const struct sockaddr_in *sa = (const struct sockaddr_in *)addr.GetAddr();

    ck_assert_int_eq(ntohs(sa->sin_port), 8080);
    ck_assert_str_eq(inet_ntoa(sa->sin_addr), "127.0.0.1");
}
END_TEST

// Test 1.4 
START_TEST(test_set_ipv4_address_changes_values)
{
    IPv4Address addr(0, "192.168.0.1");
    addr.SetIPv4Address(3000, "8.8.8.8");
    const struct sockaddr_in *sa = (const strucst sockaddr_in *)addr.GetAddr();

    ck_assert_int_eq(ntohs(sa->sin_port), 300);
    ck_assert_str_eq(inet_ntoa(sa->sin_addr), "8.8.8.8"); 
}
END_TEST

// Test 1.6
START_TEST(test_set_invalid_ip_returns_error)
{
    IPv4Address addr;
    int result;

    result = addr.SetIPv4Address(8080, "256.256.256.256");
    ck_assert_int_lt(result, 0);
    result = addr.SetIPv4Address(8080, "invalid.ip address");
    ck_assert_int_lt(result, 0);
}
END_TEST

// Test 1.7
START_TEST(test_min_and_max_port_are_valid)
{
    IPv4Address addr_min(0);
    IPv4Address addr_max(65535);

    const struct sockaddr_in *am1 = (const struct sockaddr_in *)addr_min.GetAddr();
    const struct sockaddr_in *am2 = (const struct sockaddr_in *)addr_max.GetAddr();

    ck_assert_int_eq(ntohs(am1->sin_port), 0);
    ck_assert_int_eq(ntohs(am2->sin_port), 65535);
}
END_TEST

// Test 1.5: test ipv4 zero port
START_TEST(test_setting_zero_port_is_allowed)
{
    IPv4Address addr;
    addr.SetIPv4Address(0, "127.0.0.1");

    const struct sockaddr_in *sa = (const struct sockaddr_in *)addr.GetAddr();
    ck_assert_int_eq(ntohs(sa->sin_port), 0);
}
END_TEST

// TEST: class TCPSocket.
//
// Test 2.1: tcp socket bind
START_TEST(test_tcp_socket_bind)
{
    bind = mock_bind;
    TCPSocket socket;
    struct sockaddr_in addr= {0};
    ck_assert_int_eq(socket.Bind((const sockaddr *)&addr, sizeof(addr)), 0);
    ck_assert_int_eq(socket.Bind(NULL, 0), -1); 
    bind = original_bind;
}
END_TEST

// Test 2.2: tcp socket listen
START_TEST(test_tcp_socket_listen)
{
    listen = mock_listen;
    TCPSocket socket;
    struct sockaddr_in addr= {0};
    ck_assert_int_eq(socket.Listen(5), 0);
    listen = original_listen;
}
END_TEST


// TEST: class TCPServer.
//
// tetst 3.1: server start and stop
START_TEST(test_server_start_stop)
{
    TCPServer server(8080);
    ck_assert_int_eq(server.Start(), 0);
    ck_assert_int_eq(server.Stop(), 0);
}
END_TEST


// Test for IPv4Address
Suite* ipv4_suite(void)
{
    Suite *s = suite_create("IPv4Address");
    TCase *tc = tcase_create("Core");
    tcase_add_test(tc, test_ipv4_address_creation);
    tcase_add_test(tc, test_ipv4_custom_ip);
    tcase_add_test(tc, test_ipv4_address_boundary_ports);
    tcase_add_test(tc, test_ipv4_invalid_ip);

    suite_add_tcase(s, tc);
    return s;
}

// Test for TCPSocket
Suite* tcp_socket_suite(void)
{
    Suite *s = suite_create("TCPSocket");
    TCase *tc = tcase_create("Core");
    tcase_add_test(tc, test_tcp_socket_bind);
    tcase_add_test(tc, test_tcp_socket_listen);

    suite_add_test(s, tc);
    return s;
}

// Test for TCPServer
Suite* server_suite(void)
{
    Suite *s = suite_create("TCPServer");
    TCase *tc = tcase_create("Core");
    tcase_add_test(tc, test_server_start_stop);

    suite_add_test(s, tc); 
    return s;
}

int main()
{
    int failed;
    SRunner *sr = srunner_create(ipv4_suite());
    srunner_add_suite(sr, tcp_socket_suite()); 
    srunner_add_suite(sr, server_suite()); 

    srunner_run_all(sr, CK_VERBOSE);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
