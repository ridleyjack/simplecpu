add_library(cpu4
        src/Memory.cpp
        src/Memory.h
        src/ALU.cpp
        src/ALU.h
        src/CPUDefs.h
        src/CPU.cpp
        src/CPU.h
)

target_include_directories(cpu4 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(cpu4bit main.cpp
        test/Test.h test/Test.cpp)

target_link_libraries(cpu4bit PRIVATE cpu4)