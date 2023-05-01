cmake_minimum_required(VERSION 3.24)
project(practical3)

set(CMAKE_CXX_STANDARD 17)

add_executable(practical3 main.cpp graph/graph.cpp graph/graph.h ui/ui.cpp ui/ui.h graphworker/graphtraverser.cpp graphworker/graphtraverser.h graph/undirected/undirectedgraph.h graph/undirected/undirectedgraph.cpp)
