cmake_minimum_required(VERSION 3.10)
project(tpch_query5 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set source files
set(SOURCES
    src/main.cpp
    src/query5.cpp
)

# Set include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Create executable
add_executable(tpch_query5 ${SOURCES})

# Find and link pthread for multithreading
find_package(Threads REQUIRED)
target_link_libraries(tpch_query5 PRIVATE Threads::Threads)
