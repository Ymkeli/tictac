cmake_minimum_required(VERSION 3.5.0)
project(Tic VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Board library
add_library(BoardLib STATIC src/board.cpp)

# Find the libcurl package
find_package(CURL REQUIRED)
find_package(Boost REQUIRED)

# Add main executable
add_executable(Tic src/main.cpp src/open_ai.cpp)

# Link Tic with BoardLib
target_link_libraries(Tic PRIVATE BoardLib CURL::libcurl)

# Include Boost headers (for PropertyTree and others)
target_include_directories(Tic PRIVATE ${Boost_INCLUDE_DIRS})

# Include the `test/` directory
add_subdirectory(test)
