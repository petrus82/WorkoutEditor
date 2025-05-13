cmake_minimum_required(VERSION 3.30)

set(LIBRARY_NAME "WorkoutLib")
set(GARMIN_DIR "${CMAKE_BINARY_DIR}/Garmin SDK")
set(LIBRARY_TYPE STATIC)

file (GLOB SOURCE_FILES
  workout.hpp
  workout.cpp
  fitworkout.hpp
  fitworkout.cpp
  planworkout.hpp
  planworkout.cpp
  ergworkout.hpp
  ergworkout.cpp
  mrcworkout.hpp
  mrcworkout.cpp
)

# Check if the Garmin SDK directory exists
include(FetchContent)

FetchContent_Declare(
  GarminSdk
  URL https://developer.garmin.com/downloads/fit/sdk/FitSDKRelease_21.158.00.zip
  SOURCE_DIR "${GARMIN_DIR}"
)

FetchContent_MakeAvailable(GarminSdk)
FetchContent_GetProperties(GarminSdk SOURCE_DIR GarminSdk_SOURCE_DIR)
message("Garmin Dir: ${GarminSdk_SOURCE_DIR}")
file(GLOB GarminSources "${GarminSdk_SOURCE_DIR}/cpp/*.cpp")

add_library(GarminSdk STATIC ${GarminSources})

add_library(${LIBRARY_NAME} ${LIBRARY_TYPE}
  ${SOURCE_FILES}
)

target_include_directories(${LIBRARY_NAME} PUBLIC
  "${GarminSdk_SOURCE_DIR}/cpp"
  ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(${LIBRARY_NAME} PUBLIC
  GarminSdk
)

install(TARGETS ${LIBRARY_NAME} 
  DESTINATION ${CMAKE_BINARY_DIR}/build
  COMPONENT lib
)