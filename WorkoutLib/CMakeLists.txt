cmake_minimum_required(VERSION 3.30)

set(LIBRARY_NAME "WorkoutLib")
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
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/Garmin SDK/cpp")
  file(DOWNLOAD https://developer.garmin.com/downloads/fit/sdk/FitSDKRelease_21.158.00.zip
       "${CMAKE_BINARY_DIR}/FitSDKRelease_21.158.00.zip"
      )

  file(ARCHIVE_EXTRACT INPUT "${CMAKE_BINARY_DIR}/FitSDKRelease_21.158.00.zip"
                         DESTINATION "${PROJECT_SOURCE_DIR}/Garmin\ SDK")
endif()

file(GLOB GarminSources "${PROJECT_SOURCE_DIR}/Garmin\ SDK/cpp/*.cpp")
add_library(GarminSdk ${GarminSources})

add_library(${LIBRARY_NAME} ${LIBRARY_TYPE}
  ${SOURCE_FILES}
)

target_include_directories(${LIBRARY_NAME} PUBLIC
  ${PROJECT_SOURCE_DIR}/Garmin\ SDK/cpp
  ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(${LIBRARY_NAME} PUBLIC
  GarminSdk
)

install(TARGETS ${LIBRARY_NAME} 
  DESTINATION ${CMAKE_BINARY_DIR}/build
  COMPONENT lib
)