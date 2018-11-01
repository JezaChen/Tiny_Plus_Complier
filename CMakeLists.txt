cmake_minimum_required(VERSION 3.9.6)
project(tiny_lus)

set(CMAKE_CXX_STANDARD 11)

add_executable(tiny_lus main.cpp scan2.cpp scan.h util.cpp util.h globals.h scan2.cpp errors.h parser.h parser.cpp)