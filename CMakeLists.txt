cmake_minimum_required(VERSION 3.30)
project(WorkoutEditor VERSION "0.0.1" LANGUAGES CXX)

set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG  "-g -ggdb")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(WorkoutLib)
add_subdirectory(WorkoutChart)