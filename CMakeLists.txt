cmake_minimum_required(VERSION 3.3)
project(requindesfils)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(CMAKE_C_STANDARD 99)
add_definitions("-Wall -Wextra")

set(SOURCE_FILES main.c 2_link/ethernet.c 2_link/ethernet.h 4_transport/tcp.c 4_transport/tcp.h 5_application/dns.c 5_application/dns.h 5_application/bootp.h 5_application/ascii.c 5_application/ascii.h 3_network/ip.c 3_network/ip.h print.c print.h 4_transport/udp.c 4_transport/udp.h 5_application/bootp.c 5_application/arp.c 5_application/arp.h 5_application/http.c 5_application/http.h 5_application/telnet.c 5_application/telnet.h)
add_executable(requindesfils ${SOURCE_FILES})
target_link_libraries(requindesfils pcap)