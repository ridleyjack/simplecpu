cmake_minimum_required(VERSION 3.31)
project(simplecpu)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(cpu4
        ./src/Memory.cpp
        ./src/Memory.h
        ./src/ALU.cpp
        ./src/ALU.h
        ./src/CPUDefs.h
        ./src/CPU.cpp
        ./src/CPU.h
        ./src/Nibble.h
)

target_include_directories(cpu4 PUBLIC ./src)

add_executable(cpu4bitsim main.cpp
        ./test/CPUTest.cpp
        ./test/MemTest.cpp
        ./test/Test.h
        ./test/ALUTest.cpp)

target_link_libraries(cpu4bitsim PRIVATE cpu4)