cmake_minimum_required(VERSION 3.3)
project(requindesfils)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(CMAKE_C_STANDARD 99)
add_definitions("-Wall")

set(SOURCE_FILES main.c ether.c ether.h tcp.c tcp.h dns.c dns.h bootp.h ascii.c ascii.h)
add_executable(requindesfils ${SOURCE_FILES})
target_link_libraries(requindesfils pcap)