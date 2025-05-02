# server/tests/CMakeLists.txt

set(CHECK_INCLUDE_DIR $ENV{HOME}/my_program/local_check/include)
set(CHECK_LIBRARY     $ENV{HOME}/my_program/local_check/lib/libcheck.a)

if(NOT EXISTS ${CHECK_INCLUDE_DIR}/check.h)
    message(FATAL_ERROR "Check.h not found: ${CHECK_INCLUDE_DIR}/check.h")
endif()

if(NOT EXISTS ${CHECK_LIBRARY})
    message(FATAL_ERROR "libcheck.a not found: ${CHECK_LIBRARY}")
endif()

include_directories(${CHECK_INCLUDE_DIR})
include_directories(${CHECK_INCLUDE_DIR}/../include)

set(TEST_SOURCES
    ../src/Client.cpp
    ../src/IPv4Address.cpp
    ../src/ServerSocket.cpp
    ../src/Session.cpp
    ../src/Socket.cpp
    ../src/TCPSocket.cpp
    ../src/ExceptionError/Error.cpp
    test_server.cpp
)

# list(REMOVE_ITEM TEST_SOURCES "../src/main.cpp")

add_executable(server_tests ${TEST_SOURCES})

target_link_libraries(server_tests PRIVATE ${CHECK_LIBRARY} pthread)
