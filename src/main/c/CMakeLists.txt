cmake_minimum_required(VERSION 3.10)
project(NetworKit_Graphalytics)

set(CMAKE_CXX_STANDARD 17)

link_directories(/usr/local/lib64)
include_directories(include)

add_library(
        benchmark_utils
        SHARED
        src/utils.cpp
)

link_libraries(benchmark_utils networkit)
add_executable(bfs src/algorithms/bfs.cpp)
#add_executable(cdlp src/algorithms/cdlp.cpp)
add_executable(lcc src/algorithms/lcc.cpp)
#add_executable(pr src/algorithms/pr.cpp)
add_executable(sssp src/algorithms/sssp.cpp)
#add_executable(wcc src/algorithms/wcc.cpp)
