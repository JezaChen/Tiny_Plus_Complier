cmake_minimum_required(VERSION  3.9.6)
project(tiny_lus)

set(CMAKE_CXX_STANDARD 11)

add_executable(tiny_lus main.cpp scan2.cpp scan.h util.cpp util.h parser.h parser.cpp globals.h scan2.cpp errors.h cgen.h cgen.cpp)

set(CMAKE_EXE_LINKER_FLAGS -static)