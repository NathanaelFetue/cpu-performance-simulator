cmake_minimum_required(VERSION 3.10)
project(cpu_performance_simulator CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Warning flags differ between compilers: GCC/Clang use -Wall/-Wextra,
# MSVC (Visual Studio) uses /W4. This keeps the project buildable on
# both Linux/Mac and Windows without editing CMakeLists.txt per platform.
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -O2)
endif()

include_directories(include)

add_executable(cpu_sim
    src/main.cpp
    src/simulator.cpp
)

# Basic test binary -- kept separate from the main experiment runner
# so `ctest` or a manual run of test_simulator doesn't print the
# experiment output.
enable_testing()
add_executable(test_simulator
    tests/test_simulator.cpp
    src/simulator.cpp
)
add_test(NAME simulator_tests COMMAND test_simulator)