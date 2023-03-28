cmake_minimum_required(VERSION 3.24)
project(practical1)

set(CMAKE_CXX_STANDARD 17)

add_executable(practical1 main.cpp graph/graph.cpp graph/graph.h ui/ui.cpp ui/ui.h)
