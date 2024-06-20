cmake_minimum_required(VERSION 3.28)
project(proj_data_structures)

set(CMAKE_CXX_STANDARD 17)

add_executable(proj_data_structures main.cpp
        huffman.hpp
        performance.hpp
        lorem_ipsum_supplier.hpp
        trie.hpp
        lempel_ziv_fast.hpp
        lempel_ziv_gfg.hpp
)
