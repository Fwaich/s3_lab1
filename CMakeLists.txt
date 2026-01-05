cmake_minimum_required(VERSION 3.10)
project(lab1 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0 -Wall -Wextra)
else()
    add_compile_options(-O2)
endif()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(
    include
    include/Sequence
    include/SmartPtrs
    include/Stream
    include/Utils
)

add_library(${PROJECT_NAME}_lib
    src/exceptions.cpp
    src/LazyInit.cpp
)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)

option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    enable_testing()
    

    add_executable(LazyReadOnlyStreamTests
        tests/LazyReadOnlyStreamTests.cpp
    )

    target_link_libraries(LazyReadOnlyStreamTests 
        ${PROJECT_NAME}_lib 
        GTest::gtest_main 
    )
    add_test(NAME LazyReadOnlyStreamTests COMMAND LazyReadOnlyStreamTests)
    

    add_executable(LazySequenceTests
        tests/LazySequenceTests.cpp
    )
    target_link_libraries(LazySequenceTests 
        ${PROJECT_NAME}_lib 
        GTest::gtest_main 
    )
    add_test(NAME LazySequenceTests COMMAND LazySequenceTests)
    

    add_executable(SmartPtrsTests
        tests/SmartPtrsTests.cpp
    )
    target_link_libraries(SmartPtrsTests 
        ${PROJECT_NAME}_lib 
        GTest::gtest_main 
    )
    add_test(NAME SmartPtrsTests COMMAND SmartPtrsTests)
endif()