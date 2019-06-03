cmake_minimum_required(VERSION 3.13)

project(ip_tester_v3)
link_libraries("-pthread")
set(CMAKE_CXX_STANDARD 14)

add_executable(ip_tester_v3 main.cpp fonctions.cpp)