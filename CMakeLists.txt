cmake_minimum_required(VERSION 3.12)


# Use hunter to download and build some third party dependencies
include ("cmake/HunterGate.cmake")

HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.260.tar.gz"
    SHA1 "13775235910a3fa85644568d1c5be8271de72e1c"
)

project(CppHighPerformanceCodeExamples)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-variable -Wno-unused-but-set-variable")
endif()
if(MSVC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif()

set(CMAKE_CXX_STANDARD 20)

hunter_add_package(Boost)
hunter_add_package(GTest)
hunter_add_package(benchmark)

find_package(Boost CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)

add_subdirectory("Chapter01")
add_subdirectory("Chapter02")
add_subdirectory("Chapter02/benchmarks")
add_subdirectory("Chapter03")
add_subdirectory("Chapter03/benchmarks")
add_subdirectory("Chapter05")
add_subdirectory("Chapter05/benchmarks")
add_subdirectory("Chapter06")
add_subdirectory("Chapter07")
add_subdirectory("Chapter08")
add_subdirectory("Chapter09")
add_subdirectory("Chapter10")
add_subdirectory("Chapter10/benchmarks")
add_subdirectory("Chapter11")
add_subdirectory("Chapter14")
add_subdirectory("Chapter14/benchmarks")

enable_testing()
add_test(NAME Chapter01-A_Brief_Introduction_to_C++ COMMAND Chapter01-A_Brief_Introduction_to_C++)
add_test(NAME Chapter02-Essential_CPP_Techniques COMMAND Chapter02-Essential_CPP_Techniques)
add_test(NAME Chapter02-Essential_CPP_Techniques_Benchmarks COMMAND Chapter02-Essential_CPP_Techniques_Benchmarks)
add_test(NAME Chapter03-Measuring_Performance COMMAND Chapter03-Measuring_Performance)
add_test(NAME Chapter03-Measuring_Performance_Benchmarks COMMAND Chapter03-Measuring_Performance_Benchmarks)
add_test(NAME Chapter05-Algorithms COMMAND Chapter05-Algorithms)
add_test(NAME Chapter06-Algorithms COMMAND Chapter06-Ranges_And_Views)
add_test(NAME Chapter07-Memory_Management COMMAND Chapter07-Memory_Management)
add_test(NAME Chapter08-Compile_Time_Programming COMMAND Chapter08-Compile_Time_Programming)
add_test(NAME Chapter09-Essential_Utilities COMMAND Chapter09-Essential_Utilities)
add_test(NAME Chapter10-Proxy_Objects_And_Lazy_Evaluation COMMAND Chapter10-Proxy_Objects_And_Lazy_Evaluation)
add_test(NAME Chapter11-Concurrency COMMAND Chapter11-Concurrency)
add_test(NAME Chapter14-Parallel_Algorithms COMMAND Parallel_Algorithms)
