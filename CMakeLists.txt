cmake_minimum_required(VERSION 3.8)
project(postFixCalc)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c main.h helper.c functionsToassembly.c)
add_executable(postFixCalc ${SOURCE_FILES} main.h helper.c functionsToassembly.c)