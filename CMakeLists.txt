cmake_minimum_required(VERSION 3.5.0)
project(Tic VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Board library
add_library(BoardLib STATIC src/board.cpp)

# Add main executable
add_executable(Tic src/main.cpp)

# Link Tic with BoardLib
target_link_libraries(Tic PRIVATE BoardLib)

# Include the `test/` directory
add_subdirectory(test)
