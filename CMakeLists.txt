cmake_minimum_required(VERSION 3.28)
project(proj_data_structures)

set(CMAKE_CXX_STANDARD 17)

add_executable(proj_data_structures main.cpp
        huffman_greedy.hpp
        performance.hpp
        lempel_ziv_gfg.hpp
        tests.hpp
        char_supplier.hpp
)
