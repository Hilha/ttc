cmake_minimum_required(VERSION 3.17)
project(sir_model)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/graph)
include_directories(src/utils)

add_executable(sir_model
        src/graph/graph.cpp
        src/graph/graph.h
        src/graph/vertice.cpp
        src/graph/vertice.h
        src/utils/utils.cpp
        src/utils/utils.h
        src/grafo_matriz.cpp src/heuristica/tabusearch.h src/graph/solucao.h src/graph/solucao.cpp src/graph/aresta.h)
