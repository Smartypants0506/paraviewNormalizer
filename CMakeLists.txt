cmake_minimum_required(VERSION 3.30)
project(paraviewNormalizer)

set(CMAKE_CXX_STANDARD 20)


add_executable(paraviewNormalizer rotate.cpp
        TileArrangement.cpp
        SimilarSides.cpp
        CompiledGrid.cpp
        Converter.cpp
        DisplayPixels.cpp
)
