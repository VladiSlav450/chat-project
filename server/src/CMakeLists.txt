# server/src/CMakeLists.txt

file(GLOB SOURCE *.cpp)

add_executable(chat_server ${SOURCE})

target_link_libraries(chat_server PRIVATE pthread)
