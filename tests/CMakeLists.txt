# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.1)

project(dim CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(
    ../dim
    deps/doctest/doctest
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wconversion -Wshadow")
endif()

add_executable(run_unittests
    unittests.cc
    test_dim.cc
    test_scalar.cc
    test_vector.cc
    test_point.cc
)
add_test(unittests run_unittests)
