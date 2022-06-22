cmake_minimum_required(VERSION 3.21)
project(LinearSketches)

set(CMAKE_CXX_STANDARD 14)

add_executable(LinearSketches main.cpp catch.hpp counting_sketches.cpp counting_sketches.h count_min_sketch.cpp count_min_sketch.h)
