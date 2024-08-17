cmake_minimum_required(VERSION 3.10)
project(DistributedTaskScheduler)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/inc)

# Source files
set(SOURCES
    src/main.cpp
    src/NetworkUtils.cpp
)

# Add executable
add_executable(DistributedTaskScheduler ${SOURCES})

