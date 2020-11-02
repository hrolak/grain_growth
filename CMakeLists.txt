cmake_minimum_required(VERSION 3.0.0)
project(naive_grain_growth VERSION 0.1.0)

include_directories(${CMAKE_SOURCE_DIR}/inc)

set(CMAKE_CXX_COMPILER C:/MinGW/bin/g++.exe)
set(CMAKE_C_COMPILER C:/MinGW/bin/gcc.exe)

set(SOURCE_PATH src)

set(SOURCES
   ${SOURCE_PATH}/Board.cpp
   ${SOURCE_PATH}/FileParser.cpp
   ${SOURCE_PATH}/Simulation.cpp
   ${SOURCE_PATH}/main.cpp
)

add_executable(naive_grain_growth ${SOURCES})

