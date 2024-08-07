cmake_minimum_required(VERSION 3.5.0)
project(Tic VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Tic src/main.cpp src/Board.cpp)

include(CTest)
enable_testing()

